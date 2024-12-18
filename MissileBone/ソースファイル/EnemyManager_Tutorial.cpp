// 敵管理クラス(チュートリアル用)(CEnemyManager_Tutorial.cpp)
#include "EnemyManager_Tutorial.h"
#include "Enemy_Sphere.h"

#include "Fade.h"
#include "Scene.h"
#include "EventManager.h"
#include "EffekseerManager.h"
namespace {
	EfkPath g_pszEfkPath = {
			u"Explosion_1.efk",
	};
}
CEnemyManager_Tutorial::CEnemyManager_Tutorial(CScene* pScene) : CEnemyManager(pScene)
{
}


HRESULT CEnemyManager_Tutorial::Init()
{
	/* チュートリアル内容
	１；デモモデル（停止）に突進させる（移動・ダッシュ）　　
	２：デモモデル（停止）に武器発射・リロード・射角変更・武器変更
	３：実際の敵を３体討伐してクリアー
	*/
	m_nWaveNum = 3; 
	m_nWaveCurNum = 1;

	m_vCameraTargetPos = XMFLOAT3(0.0f, 0.0f, 5000.0f);
	m_fFreeBulletMode = XMFLOAT2(6000.0f, 8000.0f);
	// エフェクシアファイルを読み込み
	if (!CEffekseerManager::Load(g_pszEfkPath.t_Path[0]))
	{
		return false;
	}
	return CEnemyManager::Init();
}

void CEnemyManager_Tutorial::Update()
{
	if (m_nWaveNum < m_nWaveCurNum) {
		CFade::Out(SCENE_GAMECLEAR);
	}

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

			// 次のチュートリアルに進行
			//m_pScene->GetEventManager(TUTORIAL_EVENT_ID)->StartEventType(m_pScene->GetEventManager(TUTORIAL_EVENT_ID)->GetEventType() + 1);
		}
	}
}


void CEnemyManager_Tutorial::CreateWave(int WaveNum)
{
	if (m_nWaveCurNum <= 0 || m_nWaveNum < m_nWaveCurNum)return;

	
	m_pWave.clear();

	switch (WaveNum)
	{
	case 1:
	{
		for (int i = 0; i < 4; ++i) {
			CEnemy_Sphere* pEnemy = new CEnemy_Sphere(m_pScene, PLAYER_COL);
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
			SetWorld(mW);
		}
		m_vCameraTargetPos = XMFLOAT3(0.0f, 0.0f, 5000.0f);
		m_fFreeBulletMode = XMFLOAT2(6000.0f, 8000.0f);
	}
	break;
	case 2:
	{
		for (int i = 0; i < 7; ++i) {
			CEnemy_Sphere* pEnemy = new CEnemy_Sphere(m_pScene, MISSILE_COL);
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
			SetWorld(mW);
		}
		m_vCameraTargetPos = XMFLOAT3(0.0f, 0.0f, 5000.0f);
		m_fFreeBulletMode = XMFLOAT2(6000.0f, 8000.0f);
		break;
	}
	case 3:
	{
		for (int i = 0; i < 3; ++i) {
			CEnemy* pEnemy = new CEnemy(m_pScene);
			pEnemy->Init();
			m_pWave.push_back(pEnemy);
		}
	
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
			SetWorld(mW);
		}
		m_vCameraTargetPos = XMFLOAT3(0.0f, 0.0f, 5000.0f);
		m_fFreeBulletMode = XMFLOAT2(6000.0f, 8000.0f);
		break;
	}
	default:
		break;
	}
}