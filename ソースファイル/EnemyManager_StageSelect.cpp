// “GŠÇ—ƒNƒ‰ƒX1(Šî–{‚Æ‚Ù‚Ú“¯‚¶)(CEnemyManager_StageSelect.cpp)
#include "EnemyManager_StageSelect.h"
#include "Boss.h"

CEnemyManager_StageSelect::CEnemyManager_StageSelect(CScene* pScene) : CEnemyManager(pScene)
{
}


HRESULT CEnemyManager_StageSelect::Init()
{
	//HRESULT hr = S_OK;

	m_nWaveNum = 1;
	m_nWaveCurNum = 1;

	CreateWave(m_nWaveCurNum);


	return CEnemyManager::Init();
}

void CEnemyManager_StageSelect::Draw()
{
	// ‰½‚à‚µ‚È‚¢

}


void CEnemyManager_StageSelect::CreateWave(int WaveNum)
{
	if (m_nWaveNum < m_nWaveCurNum)m_nWaveCurNum = 1;

	m_pWave.clear();
	switch (m_nWaveCurNum)
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
			m_pWave[i]->SetPos(XMFLOAT3((i - (int)m_pWave.size() / 2) * 2000.0f, -1000.0f, 20000.0f));

			XMFLOAT3 vPos = m_pWave[i]->GetPos();
			XMFLOAT4X4& mW = m_pWave[i]->GetWorld();
			mW._41 = vPos.x;
			mW._42 = vPos.y;
			mW._43 = vPos.z;
			SetWorld(mW);
		}
	}
	break;
	
	default:
		break;
	}
}