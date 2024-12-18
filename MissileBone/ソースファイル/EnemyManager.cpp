#include "EnemyManager.h"
#include "Texture.h"
#include "Polygon.h"



namespace {
	const float g_PlateSizeX = 300.0f;
	const float g_PlateSizeY = 200.0f;
	const float g_PlatePosX = -SCREEN_WIDTH / 2 + 150.0f;
	const float g_PlatePosY = SCREEN_HEIGHT / 2 - 100.0f;
	const XMFLOAT3 g_PlateColor = XMFLOAT3(0.0f, 0.0f, 1.0f);

	const LPCWSTR g_pszPathTexNum = L"data/texture/Number.png";

	const float g_SizeX = 45.0f; // サイズX
	const float g_SizeY = 100.0f; // サイズY
	const float g_PosX = g_PlatePosX  + g_SizeX * 2.0f; // 位置X
	const float g_PosY = g_PlatePosY - g_SizeY / 2; // 位置Y
	const XMFLOAT3 g_Color = XMFLOAT3(1.0f, 0.0f, 0.0f);

	const int g_NumU = 10; // 数字テクスチャ横分割数
	const int g_NumV = 1; // 数字テクスチャ縦分割数


	const LPCWSTR g_pszPathTexEnemy = L"data/texture/zanteki.png";
	const LPCWSTR g_pszPathTexWave = L"data/texture/Wave.png";
	const LPCWSTR g_pszPathTexSrush = L"data/texture/surush.png";

	

};


CEnemyManager::CEnemyManager(CScene* pScene) : CGameObj(pScene)
{
	SetID(GOT_ENEMYMANAGER);
	m_pScene = pScene;
	m_pPlayer = nullptr;
	m_pGameObj = nullptr;

	m_nWaveNum = 0;
	m_nWaveCurNum = 0;
	m_nDeadCnt = 0;
	m_vCameraTargetPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fFreeBulletMode = XMFLOAT2(0.0f, 0.0f);
}

CEnemyManager::~CEnemyManager()
{

}

HRESULT CEnemyManager::Init()
{
	m_nDeadCnt = 0;
	

	HRESULT hr = S_OK;

	ID3D11Device* pDevice = GetDevice();

	// テクスチャの読み込み
	hr = CreateTextureFromFile(
		pDevice,					// デバイスへのポインタ
		g_pszPathTexEnemy,		// ファイルの名前
		&m_pTextureEnemy);					// 読み込むメモリー
	if (FAILED(hr)) {
		Fin();
		return false;
	}

	hr = CreateTextureFromFile(
		pDevice,					// デバイスへのポインタ
		g_pszPathTexNum,		// ファイルの名前
		&m_pTextureNum);					// 読み込むメモリー
	if (FAILED(hr)) {
		Fin();
		return false;
	}

	hr = CreateTextureFromFile(
		pDevice,					// デバイスへのポインタ
		g_pszPathTexWave,		// ファイルの名前
		&m_pTextureWave);					// 読み込むメモリー
	if (FAILED(hr)) {
		Fin();
		return false;
	}

	hr = CreateTextureFromFile(
		pDevice,					// デバイスへのポインタ
		g_pszPathTexSrush,		// ファイルの名前
		&m_pTextureSrush);					// 読み込むメモリー
	if (FAILED(hr)) {
		Fin();
		return false;
	}

	

	return hr;
}

void CEnemyManager::Fin()
{
	SAFE_RELEASE(m_pTextureSrush);
	SAFE_RELEASE(m_pTextureWave);
	SAFE_RELEASE(m_pTextureEnemy);
	SAFE_RELEASE(m_pTextureNum);
	m_pWave.clear();
}

void CEnemyManager::Update()
{
	if (m_nWaveCurNum <= 0 || m_nWaveNum < m_nWaveCurNum)return;

	int WaveSize = m_pWave.size();
	m_nDeadCnt = 0;
	for (int i = 0; i < WaveSize; ++i) {
		if (m_pWave[i]->GetVisible())continue;
		++m_nDeadCnt;
		if (WaveSize <= m_nDeadCnt) {
			++m_nWaveCurNum;
			m_pWave.clear();
			//CreateWave(m_nWaveCurNum);
		}
	}



}

void CEnemyManager::Draw()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);								// Zバッファ更新しない


	// プレート
	CPolygon::SetTexture(nullptr);
	CPolygon::SetSize(g_PlateSizeX, g_PlateSizeY);
	CPolygon::SetPos(g_PlatePosX, g_PlatePosY);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetColor(g_PlateColor.x, g_PlateColor.y, g_PlateColor.z);
	CPolygon::SetAlpha(0.4f);
	CPolygon::Draw(pDeviceContext);

	// 縦線
	CPolygon::SetTexture(nullptr);
	CPolygon::SetSize(g_PlateSizeX / 20.0f, g_PlateSizeY);
	CPolygon::SetPos(g_PlatePosX, g_PlatePosY);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetColor(1.0f, 0.0f, 0.0f);
	CPolygon::SetAlpha(0.4f);
	CPolygon::Draw(pDeviceContext);


	// Wave
	CPolygon::SetTexture(m_pTextureWave);
	CPolygon::SetSize(100.0f, 100.0f);
	CPolygon::SetPos(g_PlatePosX - 100.0f / 1.5, g_PlatePosY + 50.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetColor(1.0f, 1.0f, 1.0f);
	CPolygon::SetAlpha(1.0f);
	CPolygon::Draw(pDeviceContext);

	// 残りWave数
	CPolygon::SetTexture(m_pTextureNum);
	CPolygon::SetSize(g_SizeX / 1.5f, g_SizeY / 1.5f);
	CPolygon::SetFrameSize(1.0f / (float)g_NumU, 1.0f / (float)g_NumV);
	CPolygon::SetColor(1.0f, 0.0f, 0.0f);
	CPolygon::SetAlpha(1.0f);

	int nCurrentWaveNum = /*(int)m_nWaveNum - (*/m_nWaveCurNum /*- 1)*/;
	for (int i = 0; i < 2/*最大桁数*/; ++i) {
		CPolygon::SetPos(g_PosX - g_PlateSizeX / 1.6f - (i * g_SizeX / 1.5f), g_PlatePosY - g_PlateSizeY / 8.0f);
		int nNum = nCurrentWaveNum % 10;
		float u, v;
		u = (1.0f / (float)g_NumU) * nNum;
		v = 1.0f / (float)g_NumV;
		CPolygon::SetUV(u, v);
		nCurrentWaveNum /= 10;

		CPolygon::Draw(pDeviceContext);
	}
	
	// スラッシュ
	CPolygon::SetTexture(m_pTextureSrush);
	CPolygon::SetSize(100.0f, 100.0f);
	CPolygon::SetPos(g_PlatePosX - g_PlateSizeX / 4, g_PlatePosY - g_PlateSizeY / 4);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetColor(1.0f, 1.0f, 1.0f);
	CPolygon::SetAlpha(1.0f);
	CPolygon::Draw(pDeviceContext);

	
	// 全Wave 数
	CPolygon::SetTexture(m_pTextureNum);
	CPolygon::SetSize(g_SizeX / 1.5f, g_SizeY / 1.5f);
	CPolygon::SetFrameSize(1.0f / (float)g_NumU, 1.0f / (float)g_NumV);
	CPolygon::SetColor(1.0f, 0.0f, 0.0f);
	CPolygon::SetAlpha(1.0f);

	int nWaveNum = (int)m_nWaveNum;
	for (int i = 0; i < 2/*最大桁数*/; ++i) {
		CPolygon::SetPos(g_PosX - g_PlateSizeX / 2.5f - (i * g_SizeX / 1.5f), g_PlatePosY - (g_PlateSizeY / 8.0f) * 3 );
		int nNum = nWaveNum % 10;
		float u, v;
		u = (1.0f / (float)g_NumU) * nNum;
		v = 1.0f / (float)g_NumV;
		CPolygon::SetUV(u, v);
		nWaveNum /= 10;

		CPolygon::Draw(pDeviceContext);
	}

	// 残敵
	CPolygon::SetTexture(m_pTextureEnemy);
	CPolygon::SetSize(75.0f, 75.0f);
	CPolygon::SetPos(g_PlatePosX + 75.0f/1.15f, g_PlatePosY + 50.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetColor(1.0f, 1.0f, 1.0f);
	CPolygon::SetAlpha(1.0f);
	CPolygon::Draw(pDeviceContext);


	// 敵数
	CPolygon::SetTexture(m_pTextureNum);
	CPolygon::SetSize(g_SizeX, g_SizeY);
	CPolygon::SetColor(g_Color.x, g_Color.y, g_Color.z);
	CPolygon::SetAlpha(1.0f);
	CPolygon::SetFrameSize(1.0f / (float)g_NumU, 1.0f / (float)g_NumV);

	int nEnemyNum = GetWaveEnemyNum();
	for (int i = 0; i < 2/*最大桁数*/; ++i) {
		CPolygon::SetPos(g_PosX - i * g_SizeX, g_PosY );
		int nNum = nEnemyNum % 10;
		float u, v;
		u = (1.0f / (float)g_NumU) * nNum;
		v = 1.0f / (float)g_NumV;
		CPolygon::SetUV(u, v);
		nEnemyNum /= 10;

		CPolygon::Draw(pDeviceContext);
	}
	

	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetColor(0.0f, 0.0f, 0.0f);
	CPolygon::SetAlpha(1.0f);
	SetZWrite(true);							// Zバッファ更新する
	SetBlendState(BS_NONE);		// アルファブレンド無効
}

void CEnemyManager::SetPlayer(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
}
void CEnemyManager::SetTarget(CGameObj* pObj)
{
	m_pGameObj = pObj;
}

void CEnemyManager::CreateWave(int WaveNum)
{
	if (m_nWaveCurNum <= 0 || m_nWaveNum < m_nWaveCurNum)return;


	m_pWave.clear();
	
	switch (WaveNum)
	{
	case 1:
	{	
		for (int i = 0; i < 3; ++i) {
			CEnemy* pEnemy = new CEnemy(m_pScene);
			pEnemy->Init();
			m_pWave.push_back(pEnemy);
		}
	    for (int i = 0; i < m_pWave.size(); ++i) {
		    m_pWave[i]->SetPlayer(m_pPlayer);
		    m_pWave[i]->SetTarget(m_pGameObj);
		    m_pWave[i]->SetPos(XMFLOAT3( (i - (int)m_pWave.size() / 2) * 2000.0f, 2000.0f, 8000.0f));
			
			XMFLOAT3 vPos = m_pWave[i]->GetPos();
			XMFLOAT4X4& mW = m_pWave[i]->GetWorld();
			mW._41 = vPos.x;
			mW._42 = vPos.y;
			mW._43 = vPos.z;
			SetWorld(mW);
	    }
	}
	    break;
	case 2:
	{
		for (int i = 0; i < 5; ++i) {
			CEnemy* pEnemy = new CEnemy(m_pScene);
			pEnemy->Init();
			m_pWave.push_back(pEnemy);
		}
		for (int i = 0; i < m_pWave.size(); ++i) {
			m_pWave[i]->SetPlayer(m_pPlayer);
			m_pWave[i]->SetTarget(m_pGameObj);
			m_pWave[i]->SetPos(XMFLOAT3((i - (int)m_pWave.size() / 2) * 2000.0f, 2000.0f, 8000.0f));
			
			XMFLOAT3 vPos = m_pWave[i]->GetPos();
			XMFLOAT4X4& mW = m_pWave[i]->GetWorld();
			mW._41 = vPos.x;
			mW._42 = vPos.y;
			mW._43 = vPos.z;
			SetWorld(mW);
		}
		break;
	}
	case 3:
	{
		for (int i = 0; i < 10; ++i) {
			CEnemy* pEnemy = new CEnemy(m_pScene);
			pEnemy->Init();
			m_pWave.push_back(pEnemy);
		}

		for (int i = 0; i < m_pWave.size(); ++i) {

			m_pWave[i]->SetPlayer(m_pPlayer);
			m_pWave[i]->SetTarget(m_pGameObj);
			m_pWave[i]->SetPos(XMFLOAT3(((i - (int)m_pWave.size() / 2)) * 2000.0f, 2000.0f, 8000.0f));
			
			XMFLOAT3 vPos = m_pWave[i]->GetPos();
			XMFLOAT4X4& mW = m_pWave[i]->GetWorld();
			mW._41 = vPos.x;
			mW._42 = vPos.y;
			mW._43 = vPos.z;
			SetWorld(mW);
		}
		break;
	}
	default:
		break;
	}

	
}


int CEnemyManager::GetWaveEnemyNum()
{
	return (int)m_pWave.size() - m_nDeadCnt;
}

void CEnemyManager::DestroyEnemy()
{
	for (int i = 0; i < m_pWave.size(); ++i) {
		m_pWave[i]->SetVisible(false);
	}
}