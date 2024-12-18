// 敵機 [Boss.cpp]
#include "Boss.h"
#include "Input.h"
#include "Player.h"
#include "Game.h"
#include "Missile.h"

#include "Explosion.h"
#include "Scene.h"
#include "Fade.h"
#include "Sound.h"
#include "EventManager_StartEnd.h"
#include "EffectManager.h"
#include "EffekseerManager.h"

// 定数
namespace {
	const float g_fSpeed = 1.0f;
	const float g_fRollSpeed = 3.0f;//単位:度
	const float g_fPitchSpeed = 1.5f;//単位:度
	const int   g_nLife = 100;

	const int g_nFireCnt = 120;
	const float g_fBS = 5000.0f;
	const float g_fAtkBS = 2000.0f;
}


//static TPartsInfo g_partsInit[MAX_ENEMY_PARTS] =
//{ // モデル番号, 親パーツNo., 位置, 角度
//{MODEL_BOSS , -1, { 0.0f, 3000.0f, 3000.0f }, {0.0f, 180.0f, 0.0f}, true},
//{MODEL_PLAYER_CANON, MODEL_BOSS, { 0.0f, -50.0f, -75.0f}, {0.0f, 0.0f, 0.0f}, true},
////{MODEL_MISSILE, MODEL_PLAYER_CANON, { 200.0f, -40.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, true},
////{MODEL_MISSILE, MODEL_PLAYER_CANON, { 165.0f, -40.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, true},
////{MODEL_MISSILE, MODEL_PLAYER_CANON, { 130.0f, -40.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, true},
////{MODEL_MISSILE, MODEL_PLAYER_CANON, { -130.0f, -40.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, true},
////{MODEL_MISSILE, MODEL_PLAYER_CANON, { -165.0f, -40.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, true},
////{MODEL_MISSILE, MODEL_PLAYER_CANON, { -200.0f, -40.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, true},
//};


// コンストラクタ
CBoss::CBoss(CScene* pScene) : CEnemy(pScene)
{
	//SetID(GOT_ENEMY);
	SetID(GOT_BOSS);

	m_pPlayer = nullptr;
	
}

// デストラクタ
CBoss::~CBoss()
{
}

// 初期化
HRESULT CBoss::Init()
{
	m_nEffectCnt = 0;
	m_nFireCnt = g_nFireCnt;
	m_nLife = g_nLife;
	m_nMaxLife = g_nLife;

	SetModel(MODEL_BOSS);
	CGameObj::SetRadius(400.0f);
	CGameObj::SetBBox(XMFLOAT3(65.f, 30.f, 90.0f));



	XMFLOAT4X4 mW;
	XMStoreFloat4x4(&mW, XMMatrixRotationY(XMConvertToRadians(180.0f)));

	XMMATRIX mtxScl,mtxTranslation;
	m_vScale = XMFLOAT3(10.0f, 10.0f, 10.0f);
	mtxScl = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	XMStoreFloat4x4(&mW, XMMatrixMultiply(XMLoadFloat4x4(&mW), mtxScl));

	m_vPos = XMFLOAT3(0.0f, 3000.0f, 3000.0f);
	mtxTranslation = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMStoreFloat4x4(&mW, XMMatrixMultiply(XMLoadFloat4x4(&mW), mtxTranslation));

	SetWorld(mW);

	//m_Effect.Init(m_pScene);

	//if (m_pWeaponManager) {
	//	m_pWeaponManager->AllDelete();
	//	delete m_pWeaponManager;
	//	m_WeaponParts.clear();
	//}
	//// 初期武器設定
	//m_nWeaponType = WEAPON_MINE/*WEAPON_MISSILE*/;
	//TPartsInfo* pWeaponPartsInitInf;
	//// 装備武器切替
	//switch (m_nWeaponType)
	//{
	//case WEAPON_MISSILE:
	//	m_pWeaponManager = new CMSManager(m_pScene);
	//	pWeaponPartsInitInf = GetMissileInitInfo();
	//	break;
	//case WEAPON_MINE:
	//	m_pWeaponManager = new CMineManager(m_pScene);
	//	pWeaponPartsInitInf = GetMineInitInfo();
	//	break;
	//case WEAPON_MULTIMS:
	//	m_pWeaponManager = new CMultiMSManager(m_pScene);
	//	pWeaponPartsInitInf = GetMultiMSInitInfo();
	//	break;
	//default:
	//	break;
	//}

	//// 武器初期化
	//m_pWeaponManager->Init();

	//// 武器サイズ分のvector作成
	//for (int i = 0; i < m_pWeaponManager->GetWeaponSize(); ++i, ++pWeaponPartsInitInf) {

	//	TParts Weapon;

	//	Weapon.nModel = pWeaponPartsInitInf->nModel;
	//	Weapon.nParent = pWeaponPartsInitInf->nParent;
	//	Weapon.vPos = pWeaponPartsInitInf->vPos;
	//	Weapon.vAngle = pWeaponPartsInitInf->vAngle;
	//	Weapon.bUnit = pWeaponPartsInitInf->bUnit;

	//	XMMATRIX mtx = XMMatrixRotationRollPitchYaw(
	//		XMConvertToRadians(Weapon.vAngle.x),
	//		XMConvertToRadians(Weapon.vAngle.y),
	//		XMConvertToRadians(Weapon.vAngle.z)) *
	//		XMMatrixTranslation(Weapon.vPos.x,
	//			Weapon.vPos.y, Weapon.vPos.z);

	//	if (Weapon.nParent >= 0) {
	//		mtx *= XMLoadFloat4x4(&m_parts[Weapon.nParent].mWorld);
	//	}
	//	XMStoreFloat4x4(&Weapon.mWorld, mtx);

	//	m_WeaponParts.push_back(Weapon);
	//}

	//m_pWeaponManager->Create(m_WeaponParts, GOT_ENEMY);

	return CModel::Init();
}

// 終了処理
void CBoss::Fin()
{
	//m_Effect.Fin();
	CModel::Fin();
}

// 更新
void CBoss::Update()
{

	if (m_nLife <= 0)
	{
		m_nLife = g_nLife;

		//StartExplosion();
		m_bVisible = false;
		CSound::Play(SE_EXPLOSION);

		if (m_EfkHandle != nullptr) {
			int Handle = CEffekseerManager::Play(m_EfkHandle->t_Path[0], false, 100);
			XMFLOAT3 vScl(5.0f, 5.0f, 5.0f);
			XMMATRIX mtxWorld = XMMatrixIdentity();
			XMMATRIX mtxScl, mtxTrans;
			mtxScl = XMMatrixScaling(vScl.x, vScl.y, vScl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);
			mtxTrans = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTrans);
			XMFLOAT4X4 World;
			XMStoreFloat4x4(&World, mtxWorld);
			CEffekseerManager::SetMatrix(Handle, false, World);


			// 爆発開始時
			m_pScene->GetEventManager(STARTEND_EVENT_ID)->StartEventType(END_STARTEND);
			m_pScene->GetCamera()->SetTarget(m_vPos);
			m_pScene->GetCamera()->SetFreeBulletMode(XMFLOAT2(2000.0f, 500.0f));
			m_pScene->GetCamera()->SetMode(BULLET_FREE_CAMERAMODE);
		}
	}



	// ワールド変換行列取得
	XMFLOAT4X4 mW = GetWorld();
	// モデル座標軸取得
	XMFLOAT3 vX = XMFLOAT3(mW._11, mW._12, mW._13);
	XMFLOAT3 vY = XMFLOAT3(mW._21, mW._22, mW._23);
	XMFLOAT3 vZ = XMFLOAT3(mW._31, mW._32, mW._33);
	// 座標取得、クリア
	m_vPos = XMFLOAT3(mW._41, mW._42, mW._43);
	mW._41 = mW._42 = mW._43 = 0.0f;



	// ステージ外に出たとき中央に向けて反転する
	if (m_pTarget) {
		if (!CollisionBS(m_pTarget)) {
			// ターゲットへのベクトルを取得
			XMFLOAT3& vTargetPos = m_pTarget->GetPos();
			XMFLOAT3 vTarget;
			vTarget.x = vTargetPos.x - m_vPos.x;
			vTarget.y = vTargetPos.y - m_vPos.y;
			vTarget.z = vTargetPos.z - m_vPos.z;



			// ターゲットが上下左右どちらか
			XMMATRIX mR = XMMatrixIdentity();

			float fRight = vX.x * vTarget.x + vX.y * vTarget.y + vX.z * vTarget.z;	// 内積
			if (fRight > 0.0f)		// 右
				mR *= XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(-g_fRollSpeed));
			else if (fRight < 0.0f)	// 左
				mR *= XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(g_fRollSpeed));
			float fUp = vY.x * vTarget.x + vY.y * vTarget.y + vY.z * vTarget.z;		// 内積
			if (fUp > 0.0f)			// 上
				mR *= XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(-g_fPitchSpeed));
			else if (fUp < 0.0f)	// 下
				mR *= XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(g_fPitchSpeed));

			if (!fRight && !fUp) {
				mR *= XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(-g_fPitchSpeed));
			}

			XMStoreFloat4x4(&mW, XMMatrixMultiply(XMLoadFloat4x4(&mW), mR));
		}
		else
		{
			if (BS(m_vPos, m_pPlayer->GetPos(), g_fBS, m_pPlayer->GetRadius())) {
				if (m_pPlayer->GetVisible()) {
					// プレイヤーへのベクトルを取得
					XMFLOAT3& vPlayerPos = m_pPlayer->GetPos();
					XMFLOAT3 vPlayer;
					vPlayer.x = vPlayerPos.x - m_vPos.x;
					vPlayer.y = vPlayerPos.y - m_vPos.y;
					vPlayer.z = vPlayerPos.z - m_vPos.z;

					// ターゲットが上下左右どちらか
					XMMATRIX mR = XMMatrixIdentity();

					float fRight = vX.x * vPlayer.x + vX.y * vPlayer.y + vX.z * vPlayer.z;	// 内積
					if (fRight > 0.0f)		// 右
						mR *= XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(-g_fRollSpeed));
					else if (fRight < 0.0f)	// 左
						mR *= XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(g_fRollSpeed));
					float fUp = vY.x * vPlayer.x + vY.y * vPlayer.y + vY.z * vPlayer.z;		// 内積
					if (fUp > 0.0f)			// 上
						mR *= XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(-g_fPitchSpeed));
					else if (fUp < 0.0f)	// 下
						mR *= XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(g_fPitchSpeed));

					if (!fRight && !fUp) {
						mR *= XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(-g_fPitchSpeed));
					}

					//XMStoreFloat4x4(&mW, XMMatrixMultiply(XMLoadFloat4x4(&mW), mR));
				}
			}
		}
	}


	if (m_bVisible) {
		// 常に前進
		m_vPos.x += vZ.x * g_fSpeed;
		m_vPos.y += vZ.y * g_fSpeed;
		m_vPos.z += vZ.z * g_fSpeed;
	}

	// ワールド変換行列に反映
	mW._41 = m_vPos.x;
	mW._42 = m_vPos.y;
	mW._43 = m_vPos.z;
	SetWorld(mW);

	if (m_pPlayer) {
		if (BS(m_vPos, m_pPlayer->GetPos(), g_fBS, m_pPlayer->GetRadius())) {

			if (BS(m_vPos, m_pPlayer->GetPos(), g_fAtkBS, m_pPlayer->GetRadius())) {
				--m_nFireCnt;
				if (m_nFireCnt <= 0/*CInput::GetKeyTrigger(VK_E)*/)
				{
					//CMissile::Fire(&mW, GOT_PLAYER);
					m_nFireCnt = g_nFireCnt;
				}


				// 衝突判定
				int nBound = ((CGame*)m_pScene)->GetBound();
				/*if (nBound == 1) {
					if (CollisionBS(m_pPlayer)) {
						m_pPlayer->StartExplosion();
						StartExplosion();
					}
				}
				else {
					if (CollisionAABB(m_pPlayer)) {
						m_pPlayer->StartExplosion();
						StartExplosion();
					}
				}*/
				switch (nBound) {
				case BOUND_SPHERE:
					if (CollisionBS(m_pPlayer)) {
						
						m_pPlayer->StartExplosion();
						//StartExplosion();
						CSound::Play(SE_EXPLOSION);
					}
					break;
				case BOUND_AABB:
					if (CollisionAABB(m_pPlayer)) {
						m_pPlayer->StartExplosion();
						//StartExplosion();
						CSound::Play(SE_EXPLOSION);
					}
					break;
				default:
					if (CollisionOBB(m_pPlayer)) {
						m_pPlayer->StartExplosion();
						//StartExplosion();
						CSound::Play(SE_EXPLOSION);
					}
					break;
				}
			}
		}
	}

	//if (m_pExp) {
	//	if (m_uExpTimer) {
	//		if (m_uExpTimer == 3 * 60) {
	//			// 爆発開始時
	//			m_pScene->GetEventManager(STARTEND_EVENT_ID)->StartEventType(END_STARTEND);
	//			m_pScene->GetCamera()->SetTarget(m_vPos);
	//			m_pScene->GetCamera()->SetFreeBulletMode(XMFLOAT2(2000.0f, 500.0f));
	//			m_pScene->GetCamera()->SetMode(BULLET_FREE_CAMERAMODE);
	//		}

	//		--m_uExpTimer;
	//		if (!m_uExpTimer) {
	//			
	//			//m_pScene->GetEventManager()->SetEventType(END_EVENT);
	//			//CFade::Out(SCENE_GAMECLEAR);
	//		
	//			Fin();
	//			return;
	//		}
	//	}
	//	m_pExp->Update();
	//}

	// スモークエフェクト
	++m_nEffectCnt;
	if (!(m_nEffectCnt % 2)) {
		XMFLOAT3 EffePos;
		EffePos = m_vPos;
		XMFLOAT3 EffeZ = XMFLOAT3(m_mWorld._31, m_mWorld._32, m_mWorld._33);
		EffePos.x -= EffeZ.x * 100;
		EffePos.y -= EffeZ.y * 100;
		EffePos.z -= EffeZ.z * 100;
		//m_Effect.SetEffect(SMOKE_EFFECT, EffePos, XMFLOAT2(150.0f, 150.0f));
		CEffectManager::StartEffect(EFFECT_SMOKE, EffePos, XMFLOAT2(150.0f, 150.0f), true);
	}
	//m_Effect.Update();
	//CModel::Update();
}


void CBoss::Draw()
{
	//m_Effect.Draw();
	CModel::Draw();
}

// バウンディボックス
bool CBoss::BS(XMFLOAT3 vA, XMFLOAT3 vB, float RadA, float RadB)
{
	// 衝突判定
	float dx = vA.x - vB.x;
	float dy = vA.y - vB.y;
	float dz = vA.z - vB.z;
	float dr = RadA + RadB;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}
