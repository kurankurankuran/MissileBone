// 敵管理クラス1(基本とほぼ同じ)(CEnemyManager_2.cpp)
#include "EnemyManager_2.h"
#include "Boss.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "EffekseerManager.h"

namespace {
	EfkPath g_pszEfkPath = {
			u"Explosion_1.efk",
	};
}


CEnemyManager_2::CEnemyManager_2(CScene* pScene) : CEnemyManager(pScene)
{
}


HRESULT CEnemyManager_2::Init()
{
	//HRESULT hr = S_OK;

	m_nWaveNum = 5;
	m_nWaveCurNum = 1;

	m_vCameraTargetPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fFreeBulletMode = XMFLOAT2(15000.0f, 8000.0f);

	// エフェクシアファイルを読み込み
	if (!CEffekseerManager::Load(g_pszEfkPath.t_Path[0]))
	{
		return false;
	}

	return CEnemyManager::Init();
}


void CEnemyManager_2::CreateWave(int WaveNum)
{
	if (m_nWaveCurNum <= 0 || m_nWaveNum < m_nWaveCurNum)return;


	m_pWave.clear();

	switch (WaveNum)
	{
	case 1:
	{
		for (int i = 0; i < 5; ++i) {
			CEnemy* pEnemy = new CEnemy(m_pScene);
			pEnemy->Init();
			m_pWave.push_back(pEnemy);
		}

		float fRad = 10000.0f;
		for (int i = 0; i < m_pWave.size(); ++i) {
			m_pWave[i]->SetPlayer(m_pPlayer);
			m_pWave[i]->SetTarget(m_pGameObj);

			float fAngle;
			fAngle = (float)i / (float)m_pWave.size() * 2 * M_PI;

			m_pWave[i]->SetAngle(XMFLOAT3(0.0f, -fAngle + XMConvertToRadians(-90.0f), 0.0f));
			m_pWave[i]->SetPos(XMFLOAT3(fRad * cosf(fAngle),  2000.0f, fRad * sinf(fAngle)));
			m_pWave[i]->SetEfkHandle(&g_pszEfkPath);
			XMFLOAT3 vScl = m_pWave[i]->GetScale();
			XMFLOAT3 vRot = m_pWave[i]->GetAngle();
			XMFLOAT3 vPos = m_pWave[i]->GetPos();
			XMFLOAT4X4& mW = m_pWave[i]->GetWorld();

			XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTrans;
			mtxWorld = XMMatrixIdentity();

			mtxScl = XMMatrixScaling(vScl.x, vScl.y, vScl.z);
			mtxWorld = XMMatrixMultiply(mtxScl, mtxWorld);

			mtxRot = XMMatrixRotationRollPitchYaw(vRot.x, vRot.y, vRot.z);
			mtxWorld = XMMatrixMultiply(mtxRot, mtxWorld);

			mtxTrans = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTrans);

			XMStoreFloat4x4(&mW, mtxWorld);

			//SetWorld(mW);

			/*mW._41 = vPos.x;
			mW._42 = vPos.y;
			mW._43 = vPos.z;
			SetWorld(mW);*/
		}
		m_vCameraTargetPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_fFreeBulletMode = XMFLOAT2(15000.0f, 8000.0f);
	}
	break;
	case 2:
	{
		for (int i = 0; i < 10; ++i) {
			CEnemy* pEnemy = new CEnemy(m_pScene);
			pEnemy->Init();
			m_pWave.push_back(pEnemy);
		}

		float fRad = 10000.0f;
		for (int i = 0; i < m_pWave.size(); ++i) {
			m_pWave[i]->SetPlayer(m_pPlayer);
			m_pWave[i]->SetTarget(m_pGameObj);

			float fAngle;
			fAngle = (float)i / (float)m_pWave.size() * 2 * M_PI;
			m_pWave[i]->SetAngle(XMFLOAT3(0.0f, -fAngle + XMConvertToRadians(-90.0f), 0.0f));
			m_pWave[i]->SetPos(XMFLOAT3(fRad * cosf(fAngle), 2000.0f, fRad * sinf(fAngle)));
			m_pWave[i]->SetEfkHandle(&g_pszEfkPath);
			XMFLOAT3 vScl = m_pWave[i]->GetScale();
			XMFLOAT3 vRot = m_pWave[i]->GetAngle();
			XMFLOAT3 vPos = m_pWave[i]->GetPos();
			XMFLOAT4X4& mW = m_pWave[i]->GetWorld();

			XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTrans;
			mtxWorld = XMMatrixIdentity();

			mtxScl = XMMatrixScaling(vScl.x, vScl.y, vScl.z);
			mtxWorld = XMMatrixMultiply(mtxScl, mtxWorld);

			mtxRot = XMMatrixRotationRollPitchYaw(vRot.x, vRot.y, vRot.z);
			mtxWorld = XMMatrixMultiply(mtxRot, mtxWorld);

			mtxTrans = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTrans);

			XMStoreFloat4x4(&mW, mtxWorld);

			//SetWorld(mW);

			/*mW._41 = vPos.x;
			mW._42 = vPos.y;
			mW._43 = vPos.z;
			SetWorld(mW);*/
		}
		m_vCameraTargetPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_fFreeBulletMode = XMFLOAT2(15000.0f, 8000.0f);
		break;
	}
	case 3:
	{
		for (int i = 0; i < 15; ++i) {
			CEnemy* pEnemy = new CEnemy(m_pScene);
			pEnemy->Init();
			m_pWave.push_back(pEnemy);
		}

		float fRad = 10000.0f;
		for (int i = 0; i < m_pWave.size(); ++i) {
			m_pWave[i]->SetPlayer(m_pPlayer);
			m_pWave[i]->SetTarget(m_pGameObj);

			float fAngle;
			fAngle = (float)i / (float)m_pWave.size() * 2 * M_PI;
			m_pWave[i]->SetAngle(XMFLOAT3(0.0f, -fAngle + XMConvertToRadians(-90.0f), 0.0f));
			m_pWave[i]->SetPos(XMFLOAT3(fRad * cosf(fAngle), 2000.0f, fRad * sinf(fAngle)));
			m_pWave[i]->SetEfkHandle(&g_pszEfkPath);
			XMFLOAT3 vScl = m_pWave[i]->GetScale();
			XMFLOAT3 vRot = m_pWave[i]->GetAngle();
			XMFLOAT3 vPos = m_pWave[i]->GetPos();
			XMFLOAT4X4& mW = m_pWave[i]->GetWorld();

			XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTrans;
			mtxWorld = XMMatrixIdentity();

			mtxScl = XMMatrixScaling(vScl.x, vScl.y, vScl.z);
			mtxWorld = XMMatrixMultiply(mtxScl, mtxWorld);

			mtxRot = XMMatrixRotationRollPitchYaw(vRot.x, vRot.y, vRot.z);
			mtxWorld = XMMatrixMultiply(mtxRot, mtxWorld);

			mtxTrans = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTrans);

			XMStoreFloat4x4(&mW, mtxWorld);

			//SetWorld(mW);

			/*mW._41 = vPos.x;
			mW._42 = vPos.y;
			mW._43 = vPos.z;
			SetWorld(mW);*/
		}
		m_vCameraTargetPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_fFreeBulletMode = XMFLOAT2(15000.0f, 8000.0f);
		break;
	}
	case 4:
	{
		for (int i = 0; i < 15; ++i) {
			CEnemy* pEnemy = new CEnemy(m_pScene);
			pEnemy->Init();
			m_pWave.push_back(pEnemy);
		}

		float fRad = 10000.0f;
		for (int i = 0; i < m_pWave.size(); ++i) {
			m_pWave[i]->SetPlayer(m_pPlayer);
			m_pWave[i]->SetTarget(m_pGameObj);

			float fAngle;
			fAngle = (float)i / (float)m_pWave.size() * 2 * M_PI;
			m_pWave[i]->SetAngle(XMFLOAT3(0.0f, -fAngle + XMConvertToRadians(-90.0f), 0.0f));
			m_pWave[i]->SetPos(XMFLOAT3(fRad * cosf(fAngle), fRad * sinf(fAngle), fRad * sinf(fAngle)));
			m_pWave[i]->SetEfkHandle(&g_pszEfkPath);
			XMFLOAT3 vScl = m_pWave[i]->GetScale();
			XMFLOAT3 vRot = m_pWave[i]->GetAngle();
			XMFLOAT3 vPos = m_pWave[i]->GetPos();
			XMFLOAT4X4& mW = m_pWave[i]->GetWorld();

			XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTrans;
			mtxWorld = XMMatrixIdentity();

			mtxScl = XMMatrixScaling(vScl.x, vScl.y, vScl.z);
			mtxWorld = XMMatrixMultiply(mtxScl, mtxWorld);

			mtxRot = XMMatrixRotationRollPitchYaw(vRot.x, vRot.y, vRot.z);
			mtxWorld = XMMatrixMultiply(mtxRot, mtxWorld);

			mtxTrans = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTrans);

			XMStoreFloat4x4(&mW, mtxWorld);

			//SetWorld(mW);

			/*mW._41 = vPos.x;
			mW._42 = vPos.y;
			mW._43 = vPos.z;
			SetWorld(mW);*/
		}
		m_vCameraTargetPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_fFreeBulletMode = XMFLOAT2(15000.0f, 8000.0f);
		break;
	}
	case 5:
	{
		for (int i = 0; i < 20; ++i) {
			CEnemy* pEnemy = new CEnemy(m_pScene);
			pEnemy->Init();
			m_pWave.push_back(pEnemy);
		}

		CBoss* pBoss = new CBoss(m_pScene);
		m_pWave.push_back(pBoss);

		float fRad = 7500.0f;
		for (int i = 0; i < m_pWave.size(); ++i) {
			m_pWave[i]->SetPlayer(m_pPlayer);
			m_pWave[i]->SetTarget(m_pGameObj);

			float fAngle;
			fAngle = (float)i / (float)m_pWave.size() * 2 * M_PI;
			m_pWave[i]->SetAngle(XMFLOAT3(0.0f, -fAngle + XMConvertToRadians(-90.0f), 0.0f));
			m_pWave[i]->SetPos(XMFLOAT3(fRad * cosf(fAngle), fRad * sinf(fAngle), 4000.0f));
			m_pWave[i]->SetEfkHandle(&g_pszEfkPath);
			XMFLOAT3 vScl = m_pWave[i]->GetScale();
			XMFLOAT3 vRot = m_pWave[i]->GetAngle();
			XMFLOAT3 vPos = m_pWave[i]->GetPos();
			XMFLOAT4X4& mW = m_pWave[i]->GetWorld();

			XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTrans;
			mtxWorld = XMMatrixIdentity();

			mtxScl = XMMatrixScaling(vScl.x, vScl.y, vScl.z);
			mtxWorld = XMMatrixMultiply(mtxScl, mtxWorld);

			mtxRot = XMMatrixRotationRollPitchYaw(vRot.x, vRot.y, vRot.z);
			mtxWorld = XMMatrixMultiply(mtxRot, mtxWorld);

			mtxTrans = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTrans);

			XMStoreFloat4x4(&mW, mtxWorld);

		
		}


		//　ボス初期化
		m_pWave[m_pWave.size() - 1]->Init();
		m_pWave[m_pWave.size() - 1]->SetPos(XMFLOAT3(0.0f, 0.0f, 4000.0f));
		m_pWave[m_pWave.size() - 1]->SetScale(XMFLOAT3(2.5f, 2.5f, 2.5f));

		XMFLOAT3 vPos = m_pWave[m_pWave.size() - 1]->GetPos();
		XMFLOAT3 vScl = m_pWave[m_pWave.size() - 1]->GetScale();
		XMFLOAT4X4& mW = m_pWave[m_pWave.size() - 1]->GetWorld();

		XMMATRIX mtxWorld, mtxRot, mtxScl, mtxTrans;
		mtxWorld = XMMatrixIdentity();

		mtxRot = XMMatrixRotationRollPitchYaw(0.0f, 180.0f, 0.0f);
		mtxWorld = XMMatrixMultiply(mtxRot, mtxWorld);

		mtxScl = XMMatrixScaling(vScl.x, vScl.y, vScl.z);
		mtxWorld = XMMatrixMultiply(mtxScl, mtxWorld);

		mtxTrans = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTrans);

	
		m_vCameraTargetPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_fFreeBulletMode = XMFLOAT2(15000.0f, 8000.0f);
		break;
	}
	default:
		break;
	}
}