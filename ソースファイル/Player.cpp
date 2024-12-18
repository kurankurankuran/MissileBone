// プレイヤー [Player.cpp]
#include "Player.h"
#include "Input.h"
//#include "Missile.h"
#include "DebugProc.h"
#include "Scene.h"
#include "MSManager.h"
#include "MineManager.h"
#include "MultiMSManager.h"
#include "MGManager.h"
#include "Land.h"
#include "Sound.h"
#include "Explosion.h"
#include "Fade.h"
#include "Polygon.h"
#include "Texture.h"
#include "EffectManager.h"
#include "EffekseerManager.h"

// 定数
namespace {
	const float g_fSpeed = 45.0f;
	const float g_fRollSpeed = 4.0f;//単位:度
	const float g_fPitchSpeed = 2.0f;//単位:度

	const float DASH_SPEED_RATE = 2.5f;
	const float DASH_RATE = 3 * 60.0f;

	const LPCWSTR g_pszPathTex[MAX_WEAPON] = {
		L"data/texture/MISSILE.png",
		L"data/texture/MINE.png",
		L"data/texture/MULTI.png",
		L"data/texture/MG.png",
	};
	const LPCWSTR g_pszPathTexNum = L"data/texture/Number.png";
	const int g_NumU = 10; // 数字テクスチャ横分割数
	const int g_NumV = 1; // 数字テクスチャ縦分割数


	/*EfkPath g_pszEfkPath = {
		u"FiterSmoke.efk",
	};*/
}


static TPartsInfo g_partsInit[MAX_PARTS] =
{ // モデル番号, 親パーツNo., 位置, 角度
{MODEL_PLAYER , -1, { PLAYER_POS.x, PLAYER_POS.y, PLAYER_POS.z}, {0.0f, 0.0f, 0.0f}, true},
{MODEL_PLAYER_CANON, MODEL_PLAYER, { 0.0f, -50.0f, -75.0f}, {0.0f, 0.0f, 0.0f}, true},
//{MODEL_MISSILE, MODEL_PLAYER_CANON, { 200.0f, -40.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, true},
//{MODEL_MISSILE, MODEL_PLAYER_CANON, { 165.0f, -40.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, true},
//{MODEL_MISSILE, MODEL_PLAYER_CANON, { 130.0f, -40.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, true},
//{MODEL_MISSILE, MODEL_PLAYER_CANON, { -130.0f, -40.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, true},
//{MODEL_MISSILE, MODEL_PLAYER_CANON, { -165.0f, -40.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, true},
//{MODEL_MISSILE, MODEL_PLAYER_CANON, { -200.0f, -40.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, true},
};

// コンストラクタ
CPlayer::CPlayer(CScene* pScene) : CModel(pScene)
{
	SetID(GOT_PLAYER);
	m_pWeaponManager = nullptr;
	m_pStageSpher = nullptr;
	m_pLand = nullptr;
}

// デストラクタ
CPlayer::~CPlayer()
{
}

// 初期化
HRESULT CPlayer::Init()
{
	m_pLand = (CLand*)m_pScene->Find(GOT_LAND);

	m_bDash = false;
	m_nDashCnt = 0;
	m_nEffectCnt = 0;
	
	SetModel(MODEL_PLAYER);
	CGameObj::SetRadius(PLAYER_BS_RAD);
	CGameObj::SetBBox(XMFLOAT3(150.f, 70.f, 200.0f));
	//m_vCenter = g_partsInit[0].vPos;
	//m_vCenter.y -= 50.0f;
	
	m_fCanonAngle = g_partsInit[MODEL_PLAYER_CANON].vAngle.x;
	
	// プレイヤー自身のパーツ処理
	for (int i = 0; i < MAX_PARTS; ++i) {
		m_parts[i].nModel = g_partsInit[i].nModel;
		m_parts[i].nParent = g_partsInit[i].nParent;
		m_parts[i].vPos = g_partsInit[i].vPos;
		m_parts[i].vAngle = g_partsInit[i].vAngle;
		m_parts[i].bUnit = g_partsInit[i].bUnit;

		XMMATRIX mtx = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(m_parts[i].vAngle.x),
			XMConvertToRadians(m_parts[i].vAngle.y),
			XMConvertToRadians(m_parts[i].vAngle.z)) *
			XMMatrixTranslation(m_parts[i].vPos.x,
				m_parts[i].vPos.y, m_parts[i].vPos.z);

		if (m_parts[i].nParent >= 0) {
			mtx *= XMLoadFloat4x4(&m_parts[m_parts[i].nParent].mWorld);
		}
		XMStoreFloat4x4(&m_parts[i].mWorld, mtx);
	}

	m_vPos = m_parts[MODEL_PLAYER].vPos;
	XMFLOAT4X4 mW = GetWorld();
	mW = m_parts[MODEL_PLAYER].mWorld;
	SetWorld(mW);

	// 武器処理
	if (m_pWeaponManager) {
		m_pWeaponManager->AllDelete();
		delete m_pWeaponManager;
		/*m_WeaponParts.clear();*/
	}
	// 初期武器設定
	m_nWeaponType = /*WEAPON_MINE*/WEAPON_MISSILE;
	TPartsInfo* pWeaponPartsInitInf;
	// 装備武器切替
	switch (m_nWeaponType)
	{
	case WEAPON_MISSILE:
		m_pWeaponManager = new CMSManager(m_pScene);
		m_pWeaponManager->SetParent(this);
		pWeaponPartsInitInf = GetMissileInitInfo();
		break;
	case WEAPON_MINE:
		m_pWeaponManager = new CMineManager(m_pScene);
		m_pWeaponManager->SetParent(this);
		pWeaponPartsInitInf = GetMineInitInfo();
		break;
	case WEAPON_MULTIMS:
		m_pWeaponManager = new CMultiMSManager(m_pScene);
		m_pWeaponManager->SetParent(this);
		pWeaponPartsInitInf = GetMultiMSInitInfo();
		break;
	default:
		break;
	}

	// 武器初期化
	m_pWeaponManager->Init(&m_parts[1].mWorld);
	
	

	m_pWeaponManager->Create(/*m_WeaponParts, */GOT_ENEMY);



	//// ワールド変換行列取得
	//XMFLOAT4X4 mW = GetWorld();
	//XMMATRIX mtxScl;
	//m_vScale = XMFLOAT3(5.0f, 5.0f, 5.0f);
	//mtxScl = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
 //   XMStoreFloat4x4(&mW, XMMatrixMultiply(XMLoadFloat4x4(&mW), mtxScl));

	//SetWorld(mW);

	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	for (int i = 0; i < MAX_WEAPON; i++)
	{
		hr = CreateTextureFromFile(
			pDevice,					// デバイスへのポインタ
			g_pszPathTex[i],		// ファイルの名前
			&m_pTextureWeaponType[i]);					// 読み込むメモリー

		if (FAILED(hr)) {
			Fin();
			return false;
		}
	}
	hr = CreateTextureFromFile(
		pDevice,					// デバイスへのポインタ
		g_pszPathTexNum,		// ファイルの名前
		&m_pTextureNum);					// 読み込むメモリー
	if (FAILED(hr)) {
		Fin();
		return false;
	}

	
	//// エフェクシアファイルを読み込み
	//if (!CEffekseerManager::Load(g_pszEfkPath.t_Path[0]))
	//{
	//	return false;
	//}
	

	return CModel::Init();
}

// 終了処理
void CPlayer::Fin()
{
	SAFE_RELEASE(m_pTextureNum);
	for (int i = 0; i < MAX_WEAPON; i++)
	{
		SAFE_RELEASE(m_pTextureWeaponType[i]);
	}

	//m_Effect.Fin();

	if (m_pWeaponManager) {
		delete m_pWeaponManager;
	}
	
	CModel::Fin();

	
}

// 更新
void CPlayer::Update()
{


	do {
		if (!m_bVisible)
			break;

		// ワールド変換行列取得
		XMFLOAT4X4 mW = GetWorld();
		// モデル座標軸取得
		XMFLOAT3 vX = XMFLOAT3(mW._11, mW._12, mW._13);
		XMFLOAT3 vY = XMFLOAT3(mW._21, mW._22, mW._23);
		XMFLOAT3 vZ = XMFLOAT3(mW._31, mW._32, mW._33);
		// 座標取得、クリア
		m_vPos = XMFLOAT3(mW._41, mW._42, mW._43);
		mW._41 = mW._42 = mW._43 = 0.0f;



		// ピッチ
		XMMATRIX mR = XMMatrixIdentity();
		if (CInput::GetKeyPress(VK_W) /*|| CInput::GetKeyPress(VK_UP)*/) {
			mR = XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(g_fPitchSpeed));
		}
		if (CInput::GetKeyPress(VK_S) /*|| CInput::GetKeyPress(VK_DOWN)*/) {
			mR = XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(-g_fPitchSpeed));
		}
		// ロール
		if (CInput::GetKeyPress(VK_A) /*|| CInput::GetKeyPress(VK_LEFT)*/) {
			mR = XMMatrixMultiply(mR, XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(g_fRollSpeed)));
		}
		if (CInput::GetKeyPress(VK_D) /*|| CInput::GetKeyPress(VK_RIGHT)*/) {
			mR = XMMatrixMultiply(mR, XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(-g_fRollSpeed)));
		}
		XMStoreFloat4x4(&mW, XMMatrixMultiply(XMLoadFloat4x4(&mW), mR));



		XMFLOAT3 vMove = XMFLOAT3(vZ.x * g_fSpeed, vZ.y * g_fSpeed, vZ.z * g_fSpeed);

		

		// 前進
		// ダッシュ処理
		if (CInput::GetKeyTrigger(VK_SPACE) && !m_bDash) {
			m_bDash = true;
			CSound::Play(SE_ACCEL);
		}
		if (m_bDash) {
			++m_nDashCnt;
			if (m_nDashCnt >= DASH_RATE) {
				m_bDash = false;
				m_nDashCnt = 0;
			}
		}
		if (m_nDashCnt > 0) {
			XMVECTOR vecMove = XMLoadFloat3(&vMove);
			vecMove *= DASH_SPEED_RATE;

			XMStoreFloat3(&vMove, vecMove);
		}

		// 地形との当たり判定
		CollosionLand(&vMove);


		/*if (CInput::GetKeyPress(VK_LSHIFT)) {
			m_vPos.x += vMove.x;
			m_vPos.y += vMove.y;
			m_vPos.z += vMove.z;
		}*/

		m_vPos.x += vMove.x;
		m_vPos.y += vMove.y;
		m_vPos.z += vMove.z;


		// ステージ外に出たとき
		if (m_pStageSpher) {
			if (!CollisionBS(m_pStageSpher)) {
				// ターゲットへのベクトルを取得
				XMFLOAT3& vTargetPos = m_pStageSpher->GetPos();

				XMVECTOR vecPos = XMVectorSet(
					m_vPos.x - vTargetPos.x,
					m_vPos.y - vTargetPos.y,
					m_vPos.z - vTargetPos.z,
					1.0f);

				vecPos = XMVector3Normalize(vecPos);
				vecPos = vecPos * (m_pStageSpher->GetRadius() /*+ m_fRadius - 10.0f*/);

				XMStoreFloat3(&m_vPos, vecPos + XMLoadFloat3(&vTargetPos));
			}
		}


		// ワールド変換行列に反映
		mW._41 = m_vPos.x;
		mW._42 = m_vPos.y;
		mW._43 = m_vPos.z;

		SetWorld(mW);



		if (CInput::GetKeyPress(VK_I))
		{
			m_fCanonAngle -= 1.5f;
		}
		if (CInput::GetKeyPress(VK_K))
		{
			m_fCanonAngle += 1.5f;
		}
		if (m_fCanonAngle >= 180.0f)
		{
			m_fCanonAngle = 180.0f;
		}
		if (m_fCanonAngle < 0.0f)
		{
			m_fCanonAngle = 0.0f;
		}
		m_parts[MODEL_PLAYER_CANON].vAngle.x = m_fCanonAngle;


		// 武器変更
		ChangeWeapon();

		// 武器発射
		if (m_nWeaponType != WEAPON_MACHINEGUN) {
			if (CInput::GetKeyTrigger(VK_L)) {
				m_pWeaponManager->Fire();
				
			}
		}
		else
		{
			if (CInput::GetKeyPress(VK_L)) {
				m_pWeaponManager->Fire();

				
			}
		}
		// 武器装填
		if (CInput::GetKeyTrigger(VK_J)) {
			m_pWeaponManager->Create(/*m_WeaponParts*//*&m_parts[MODEL_MISSILE]*//*,*/ GOT_ENEMY);
			CSound::Play(SE_DEFEND);
		}




	} while (0);


	// 全パーツのワールドマトリックス更新
	for (int i = 0; i < MAX_PARTS; ++i) {
		if (m_parts[i].bUnit) {
			XMMATRIX mtx = XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(m_parts[i].vAngle.x),
				XMConvertToRadians(m_parts[i].vAngle.y),
				XMConvertToRadians(m_parts[i].vAngle.z)) *
				XMMatrixTranslation(m_parts[i].vPos.x, m_parts[i].vPos.y, m_parts[i].vPos.z);
			if (m_parts[i].nParent < 0) {
				mtx *= XMLoadFloat4x4(&GetWorld());
			}
			else {
				mtx *= XMLoadFloat4x4(&m_parts[m_parts[i].nParent].mWorld);
			}
			XMStoreFloat4x4(&m_parts[i].mWorld, mtx);

			/*	if (MODEL_MISSILE <= i && i < MODEL_MISSILE + m_pWeaponManager->GetWeaponSize()) {
					m_pWeaponManager->SetWorld(i - MODEL_MISSILE,m_parts[i].mWorld);
				}*/
		}
	}






	//CModel::Update();

	if (m_pExp) {
		if (m_uExpTimer) {
			--m_uExpTimer;
			if (!m_uExpTimer) {
				m_pExp->FinDeb();
				//SetVisible();
				CFade::Out(SCENE_GAMEOVER);
			}
		}
		m_pExp->Update();
	}

	m_pWeaponManager->Update();

	// デバッグ表示
	CDebugProc::Print("*** ﾌﾟﾚｲﾔｰ ***\n");
	CDebugProc::Print("Dash:[SPACE]\n");
	CDebugProc::Print("Pitch:[W][S]\n");
	CDebugProc::Print("Roll:[A][D]\n");
	CDebugProc::Print("Fire  Missile:[L]\n");
	CDebugProc::Print("Reload  Missile:[J]\n");
	CDebugProc::Print("AngleToF Missile:[I]\n");
	CDebugProc::Print("AngleToB Missile:[K]\n");
	CDebugProc::Print("Pos :X[%f] Y[%f] Z[%f]\n", m_vPos.x, m_vPos.y, m_vPos.z);
	CDebugProc::Print("\n");


	// スモークエフェクト
	++m_nEffectCnt;
	if (!(m_nEffectCnt % 2)) {
		XMFLOAT3 EffePos;
		EffePos.x = m_parts[MODEL_PLAYER].mWorld._41;
		EffePos.y = m_parts[MODEL_PLAYER].mWorld._42;
		EffePos.z = m_parts[MODEL_PLAYER].mWorld._43;
		XMFLOAT3 EffeZ = XMFLOAT3(m_parts[MODEL_PLAYER].mWorld._31, m_parts[MODEL_PLAYER].mWorld._32, m_parts[MODEL_PLAYER].mWorld._33);
		EffePos.x -= EffeZ.x * 500;
		EffePos.y -= EffeZ.y * 500;
		EffePos.z -= EffeZ.z * 500;
		//m_Effect.SetEffect(SMOKE_EFFECT, EffePos, XMFLOAT2(100.0f, 100.0f));
		CEffectManager::StartEffect(EFFECT_SMOKE, EffePos, XMFLOAT2(100.0f, 100.0f), true);
	}
	//m_Effect.Update();


}

// 描画
void CPlayer::Draw()
{
	//CModel::Draw();

	if (!m_bVisible) return;



	ID3D11DeviceContext* pDC = GetDeviceContext();
	for (int i = 0; i < MAX_PARTS; ++i) {
		if (m_parts[i].bUnit) {
			CAssimpModel* pModel = CModel::GetAssimp((EModel)m_parts[i].nModel);
			pModel->SetCamera(m_pScene->GetCamera());
			pModel->SetLight(m_pScene->GetLight());
			pModel->Draw(pDC, m_parts[i].mWorld, eOpacityOnly);
		}
	}

	//m_Effect.Draw();


	if (m_pScene->GetID() == SCENE_SELECT)return;

	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);								// Zバッファ更新しない
	if (m_nDashCnt > 0) {
		CPolygon::SetTexture(nullptr);
		CPolygon::SetSize(200.0f - (200.0f / DASH_RATE) * m_nDashCnt, 20.0f);
		CPolygon::SetPos(-SCREEN_WIDTH / 2 + 150.0f, SCREEN_HEIGHT / 2 - 350.0f - 10.0f);
		CPolygon::SetFrameSize(1.0f, 1.0f);
		CPolygon::SetUV(0.0f, 0.0f);
		CPolygon::SetColor(1.0f, 0.0f, 1.0f/* g_fCol, g_fCol, g_fCol */);
		CPolygon::SetAlpha(0.4f);
		CPolygon::Draw(pDC);
	}


	// プレート
	CPolygon::SetTexture(nullptr);
	CPolygon::SetSize(300.0f, 150.0f);
	CPolygon::SetPos(-SCREEN_WIDTH / 2 + 150.0f, SCREEN_HEIGHT / 2 - 275.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetColor(0.2f, 1.0f, 0.2f/* g_fCol, g_fCol, g_fCol */);
	CPolygon::SetAlpha(0.4f);
	CPolygon::Draw(pDC);
	// 武器種
	switch (m_nWeaponType)
	{
	case WEAPON_MISSILE:
		CPolygon::SetTexture(m_pTextureWeaponType[WEAPON_MISSILE]);
		CPolygon::SetSize(300.0f, 200.0f);
		CPolygon::SetPos(-SCREEN_WIDTH / 2 + 150.0f, SCREEN_HEIGHT / 2 - 275.0f);
		CPolygon::SetFrameSize(1.0f, 1.0f);
		CPolygon::SetUV(0.0f, 0.0f);
		CPolygon::SetColor(0.2f, 0.1f, 0.5f/* g_fCol, g_fCol, g_fCol */);
		CPolygon::SetAlpha(1.0f);
		CPolygon::Draw(pDC);
		break;
	case WEAPON_MINE:
		CPolygon::SetTexture(m_pTextureWeaponType[WEAPON_MINE]);
		CPolygon::SetSize(300.0f, 200.0f);
		CPolygon::SetPos(-SCREEN_WIDTH / 2 + 150.0f, SCREEN_HEIGHT / 2 - 275.0f);
		CPolygon::SetFrameSize(1.0f, 1.0f);
		CPolygon::SetUV(0.0f, 0.0f);
		CPolygon::SetColor(0.6f, 0.2f, 0.0f/* g_fCol, g_fCol, g_fCol */);
		CPolygon::SetAlpha(1.0f);
		CPolygon::Draw(pDC);
		break;
	case WEAPON_MULTIMS:
		CPolygon::SetTexture(m_pTextureWeaponType[WEAPON_MULTIMS]);
		CPolygon::SetSize(300.0f, 200.0f);
		CPolygon::SetPos(-SCREEN_WIDTH / 2 + 150.0f, SCREEN_HEIGHT / 2 - 275.0f);
		CPolygon::SetFrameSize(1.0f, 1.0f);
		CPolygon::SetUV(0.0f, 0.0f);
		CPolygon::SetColor(1.0f, 1.0f, 0.0f/* g_fCol, g_fCol, g_fCol */);
		CPolygon::SetAlpha(1.0f);
		CPolygon::Draw(pDC);
		break;
	case WEAPON_MACHINEGUN:
	{
		float fPosX, fPosY;
		fPosX = -SCREEN_WIDTH / 2 + 150.0f;
		fPosY = SCREEN_HEIGHT / 2 - 275.0f;
		CPolygon::SetTexture(m_pTextureWeaponType[WEAPON_MACHINEGUN]);
		CPolygon::SetSize(300.0f, 200.0f);
		CPolygon::SetPos(fPosX, fPosY);
		CPolygon::SetFrameSize(1.0f, 1.0f);
		CPolygon::SetUV(0.0f, 0.0f);
		CPolygon::SetColor(1.0f, 1.0f, 0.0f/* g_fCol, g_fCol, g_fCol */);
		CPolygon::SetAlpha(1.0f);
		CPolygon::Draw(pDC);



		CPolygon::SetTexture(m_pTextureNum);
		CPolygon::SetSize(30.0f, 50.0f);
		CPolygon::SetFrameSize(1.0f / (float)g_NumU, 1.0f / (float)g_NumV);
		int nMagazineNum = m_pWeaponManager->GetMagazineNum();
		if (nMagazineNum <= 0) {
			CPolygon::SetColor(1.0f, 0.0f, 0.0f);
		}
		else
		{
			CPolygon::SetColor(1.0f, 1.0f, 0.0f);
		}
		for (int i = 0; i < 3/*最大桁数*/; ++i) {
			CPolygon::SetPos(fPosX - (i * 30.0f) + 3/2*30.0f, fPosY - 60.0f);
			int nNum = nMagazineNum % 10;
			float u, v;
			u = (1.0f / (float)g_NumU) * nNum;
			v = 1.0f / (float)g_NumV;
			CPolygon::SetUV(u, v);
			nMagazineNum /= 10;

			CPolygon::Draw(pDC);
		}

	}
		break;
	default:
		break;
	}






	CPolygon::SetColor(0.0f, 0.0f, 0.0f);

	SetZWrite(true);							// Zバッファ更新する
	SetBlendState(BS_NONE);		// アルファブレンド無効
	


}

void CPlayer::ChangeWeapon()
{
	if (m_pWeaponManager) {
		if (!m_pWeaponManager->GetChangeFlg())return;
	}

	int nWeaponType = m_nWeaponType;
	// 武器変更
	

	if (CInput::GetKeyTrigger(VK_U)) {
		++m_nWeaponType;
		if (m_nWeaponType >= MAX_WEAPON) {
			m_nWeaponType = 0/*WEAPON_MISSILE*/;
		}
	}
	else if(CInput::GetKeyTrigger(VK_O))
	{
		--m_nWeaponType;
		if (0 > m_nWeaponType) {
			m_nWeaponType = MAX_WEAPON - 1;
		}
	}


	if (nWeaponType == m_nWeaponType) return;

	CSound::Play(SE_SHOT);

	if (m_pWeaponManager) {
		m_pWeaponManager->AllDelete();
		delete m_pWeaponManager;
		//m_WeaponParts.clear();
	}

	

	//m_nWeaponType = WEAPON_MINE/*WEAPON_MISSILE*/;
	TPartsInfo* pWeaponPartsInitInf;
	// 装備武器切替
	switch (m_nWeaponType)
	{
	case WEAPON_MISSILE:
		m_nWeaponType = WEAPON_MISSILE;
		m_pWeaponManager = new CMSManager(m_pScene);
		m_pWeaponManager->SetParent(this);
		pWeaponPartsInitInf = GetMissileInitInfo();
		break;
	case WEAPON_MINE:
		m_nWeaponType = WEAPON_MINE;
		m_pWeaponManager = new CMineManager(m_pScene);
		m_pWeaponManager->SetParent(this);
		pWeaponPartsInitInf = GetMineInitInfo();
		break;
	case WEAPON_MULTIMS:
		m_nWeaponType = WEAPON_MULTIMS;
		m_pWeaponManager = new CMultiMSManager(m_pScene);
		m_pWeaponManager->SetParent(this);
		pWeaponPartsInitInf = GetMultiMSInitInfo();
		break;
	case WEAPON_MACHINEGUN:
		m_nWeaponType = WEAPON_MACHINEGUN;
		m_pWeaponManager = new CMGManager(m_pScene);
		m_pWeaponManager->SetParent(this);
		pWeaponPartsInitInf = GetMachinegunInitInfo();
		break;
	default:
		break;
	}

	// 武器初期化
	m_pWeaponManager->Init(&m_parts[1].mWorld);

	
	m_pWeaponManager->Create(/*m_WeaponParts,*/ GOT_ENEMY);
}

void CPlayer::CollosionLand(XMFLOAT3* vMove)
{
	// 

	// 現在位置取得
	XMFLOAT3 vPos = GetPos();
	// 回転マトリックス生成
	/*XMFLOAT4X4 mW;
	mW = m_mWorld;*/
	// モデル座標軸取得
	//XMFLOAT3 vZ = XMFLOAT3(mW._31, mW._32, mW._33);

	bool bLand = false;

	XMFLOAT3 vCrosNor; // 交点の法線
	XMFLOAT3 vCrosPos; // 交点の座標
	if (m_pLand)
		bLand = m_pLand->Collision(vPos, *vMove, &vCrosPos, &vCrosNor);

	if (!bLand)
	{
		return; // 衝突無し
	}

	// 壁を貫通した分のベクトル
	XMFLOAT3 vPenet = XMFLOAT3(
		(vPos.x + vMove->x) - vCrosPos.x,
		(vPos.y + vMove->y) - vCrosPos.y,
		(vPos.z + vMove->z) - vCrosPos.z
		);

	vPenet = *vMove;
	
	/*XMVECTOR vecPenet = XMVector3Normalize(XMLoadFloat3(&vPenet));
	XMVECTOR vecNor = XMVector3Normalize(XMLoadFloat3(&vCrosNor));
	
	float fAngle = XMVectorGetX(XMVector3AngleBetweenNormals(vecPenet, vecNor));
	XMMATRIX mtxRotPenet = XMMatrixRotationAxis(XMVector3Cross(vecPenet, vecNor), fAngle);

	XMVECTOR newMove = XMVector3TransformCoord(XMLoadFloat3(&vPenet), mtxRotPenet);*/
	XMVECTOR vecPenet =XMLoadFloat3(&vPenet);
	XMVECTOR vecNor = XMVector3Normalize(XMLoadFloat3(&vCrosNor));
	XMVECTOR newMove = vecPenet - XMVector3Dot(vecPenet, vecNor) * vecNor;
	
	XMStoreFloat3(vMove, newMove);

	*vMove = XMFLOAT3((int)vMove->x, (int)vMove->y, (int)vMove->z);


}