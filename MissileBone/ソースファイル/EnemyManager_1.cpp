// 敵管理クラス1(基本とほぼ同じ)(CEnemyManager_1.cpp)
#include "EnemyManager_1.h"
#include "Boss.h"
#include "EffekseerManager.h"
#include "Enemy_Strate.h"

namespace {
	EfkPath g_pszEfkPath = {
			u"Explosion_1.efk",
	};
}

CEnemyManager_1::CEnemyManager_1(CScene* pScene) : CEnemyManager(pScene)
{
}


HRESULT CEnemyManager_1::Init()
{
	//HRESULT hr = S_OK;

	m_nWaveNum = 3;
	m_nWaveCurNum = 1;

	m_vCameraTargetPos = XMFLOAT3(0.0f, 0.0f, 8000.0f);
	m_fFreeBulletMode = XMFLOAT2(6000.0f, 8000.0f);

	// エフェクシアファイルを読み込み
	if (!CEffekseerManager::Load(g_pszEfkPath.t_Path[0]))
	{
		return false;
	}

	return CEnemyManager::Init();
}


void CEnemyManager_1::CreateWave(int WaveNum)
{
	if (m_nWaveCurNum <= 0 || m_nWaveNum < m_nWaveCurNum)return;

	m_pWave.clear();

	switch (WaveNum)
	{
	case 1:
	{
		for (int i = 0; i < 3; ++i) {
			CEnemy* pEnemy = new CEnemy_Strate(m_pScene);
			pEnemy->Init();
			m_pWave.push_back(pEnemy);
		}
		for (int i = 0; i < m_pWave.size(); ++i) {
			m_pWave[i]->SetPlayer(m_pPlayer);
			m_pWave[i]->SetTarget(m_pGameObj);
			m_pWave[i]->SetPos(XMFLOAT3((i - (int)m_pWave.size() / 2) * 2000.0f, 2000.0f, 8000.0f));
			m_pWave[i]->SetEfkHandle(&g_pszEfkPath);
			XMFLOAT3 vPos = m_pWave[i]->GetPos();
			XMFLOAT4X4& mW = m_pWave[i]->GetWorld();
			mW._41 = vPos.x;
			mW._42 = vPos.y;
			mW._43 = vPos.z;
			//SetWorld(mW);
		}

		m_vCameraTargetPos = XMFLOAT3(0.0f, 0.0f, 8000.0f);
		m_fFreeBulletMode = XMFLOAT2(6000.0f, 8000.0f);
	}
	break;
	case 2:
	{
		for (int i = 0; i < 5; ++i) {
			CEnemy* pEnemy = new CEnemy_Strate(m_pScene);
			pEnemy->Init();
			m_pWave.push_back(pEnemy);
		}
		for (int i = 0; i < m_pWave.size(); ++i) {
			m_pWave[i]->SetPlayer(m_pPlayer);
			m_pWave[i]->SetTarget(m_pGameObj);
			m_pWave[i]->SetPos(XMFLOAT3((i - (int)m_pWave.size() / 2) * 2000.0f + 950.0f, 2000.0f, 8000.0f));
			m_pWave[i]->SetEfkHandle(&g_pszEfkPath);
			XMFLOAT3 vPos = m_pWave[i]->GetPos();
			XMFLOAT4X4& mW = m_pWave[i]->GetWorld();
			mW._41 = vPos.x;
			mW._42 = vPos.y;
			mW._43 = vPos.z;
			//SetWorld(mW);
		}
		m_vCameraTargetPos = XMFLOAT3(0.0f, 0.0f, 8000.0f);
		m_fFreeBulletMode = XMFLOAT2(6000.0f, 8000.0f);

		break;
	}
	case 3:
	{
		for (int i = 0; i < 10; ++i) {
			CEnemy* pEnemy = new CEnemy_Strate(m_pScene);
			pEnemy->Init();
			m_pWave.push_back(pEnemy);
		}

		CBoss* pBoss = new CBoss(m_pScene);
		m_pWave.push_back(pBoss);


		for (int i = 0; i < m_pWave.size(); ++i) {

			m_pWave[i]->SetPlayer(m_pPlayer);
			m_pWave[i]->SetTarget(m_pGameObj);
			m_pWave[i]->SetPos(XMFLOAT3(((i - (int)m_pWave.size() / 2)) * 2000.0f, 2000.0f, 8000.0f));
			m_pWave[i]->SetEfkHandle(&g_pszEfkPath);
			XMFLOAT3 vPos = m_pWave[i]->GetPos();
			XMFLOAT4X4& mW = m_pWave[i]->GetWorld();
			mW._41 = vPos.x;
			mW._42 = vPos.y;
			mW._43 = vPos.z;
			//SetWorld(mW);
		}


		//　ボス初期化
		m_pWave[m_pWave.size() - 1]->Init();

		m_vCameraTargetPos = XMFLOAT3(0.0f, 0.0f, 5000.0f);
		m_fFreeBulletMode = XMFLOAT2(6000.0f, 8000.0f);
		break;
	}
	default:
		break;
	}


}