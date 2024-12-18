// �G�@(��~����) [Enemy_Sphere.cpp]
#include "Enemy_Sphere.h"
#include "Explosion.h"
#include "Player.h"
#include "Game.h"
#include "Sound.h"
#include "EffekseerManager.h"
// �萔
namespace {
	
	const int g_nFireCnt = 120;
	const float g_fBS = 5000.0f;
	const float g_fAtkBS = 2000.0f;

	const int g_nMaxLife = 10;
}


CEnemy_Sphere::CEnemy_Sphere(CScene* pScene,CollisionTarget ColTarget) : CEnemy(pScene)		// �R���X�g���N�^
{
	m_nColTarget = ColTarget;
}

CEnemy_Sphere::~CEnemy_Sphere()			// �f�X�g���N�^
{
	
}

HRESULT CEnemy_Sphere::Init()		// ������
{
	m_nLife = g_nMaxLife;
	
	SetModel(MODEL_SPHERE);
	CGameObj::SetRadius(200.0f);
	CGameObj::SetBBox(XMFLOAT3(5.0f, 5.0f, 5.0f));

	m_vCenter.x += 1.80f;

	XMFLOAT4X4 mW;
	XMStoreFloat4x4(&mW, XMMatrixRotationY(XMConvertToRadians(180.0f)));

	XMMATRIX mtxScl/*, mtxTranslation*/;
	m_vScale = XMFLOAT3(50.0f, 50.0f, 50.0f);
	//m_vScale = XMFLOAT3(5.0f, 5.0f, 5.0f);
	mtxScl = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	XMStoreFloat4x4(&mW, XMMatrixMultiply(XMLoadFloat4x4(&mW), mtxScl));

	/*m_vPos = XMFLOAT3(0.0f, 3000.0f, 3000.0f);
	mtxTranslation = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMStoreFloat4x4(&mW, XMMatrixMultiply(XMLoadFloat4x4(&mW), mtxTranslation));*/

	SetWorld(mW);

	return CModel::Init();
}
void CEnemy_Sphere::Fin()			// �I������
{
	CModel::Fin();
}

void CEnemy_Sphere::Update()		// �X�V
{
	if (m_pExp) {
		if (m_uExpTimer) {
			--m_uExpTimer;
			if (!m_uExpTimer) {
				m_pExp->FinDeb();
			}
		}
		m_pExp->Update();
	}

	if (m_nLife <= 0)
	{
		m_nLife = g_nMaxLife;

		StartExplosion();
		CSound::Play(SE_EXPLOSION);

		if (m_EfkHandle != nullptr) {
			XMFLOAT4X4 mtxWorld;
			XMMATRIX mtxScl/*, mtxTranslation*/;
			mtxScl = XMMatrixScaling(1.0f / m_vScale.x, 1.0f / m_vScale.y, 1.0f / m_vScale.z);
			XMStoreFloat4x4(&mtxWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_mWorld), mtxScl));

			int Handle = CEffekseerManager::Play(m_EfkHandle->t_Path[0], false, 30);
			CEffekseerManager::SetMatrix(Handle, false,mtxWorld);
		}
	}



	if (m_nColTarget != PLAYER_COL)return;
	// �v���C���[�ɂ�����Ǝ������j��
	if (m_pPlayer) {
		if (BS(m_vPos, m_pPlayer->GetPos(), g_fBS, m_pPlayer->GetRadius())) {
			// �Փ˔���
			int nBound = ((CGame*)m_pScene)->GetBound();

			XMFLOAT4X4 mtxWorld;
			switch (nBound) {
			case BOUND_SPHERE:
				if (CollisionBS(m_pPlayer)) {
					StartExplosion();
					if (m_EfkHandle != nullptr) {
						
						XMMATRIX mtxScl/*, mtxTranslation*/;
						mtxScl = XMMatrixScaling(1.0f/m_vScale.x, 1.0f/m_vScale.y, 1.0f/m_vScale.z);
						XMStoreFloat4x4(&mtxWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_mWorld), mtxScl));

						int Handle = CEffekseerManager::Play(m_EfkHandle->t_Path[0], false, 30);
						CEffekseerManager::SetMatrix(Handle,false, mtxWorld);
					}
					CSound::Play(SE_EXPLOSION);
				}
				break;
			case BOUND_AABB:
				if (CollisionAABB(m_pPlayer)) {
					StartExplosion();
					if (m_EfkHandle != nullptr) {
						XMMATRIX mtxScl/*, mtxTranslation*/;
						mtxScl = XMMatrixScaling(1.0f/m_vScale.x / m_vScale.x, 1.0f / m_vScale.y, 1.0f / m_vScale.z);
						XMStoreFloat4x4(&mtxWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_mWorld), mtxScl));

						int Handle = CEffekseerManager::Play(m_EfkHandle->t_Path[0], false, 30);
						CEffekseerManager::SetMatrix(Handle, false,mtxWorld);
					}
					CSound::Play(SE_EXPLOSION);
				}
				break;
			default:
				if (CollisionOBB(m_pPlayer)) {
					StartExplosion();
					if (m_EfkHandle != nullptr) {
						XMMATRIX mtxScl/*, mtxTranslation*/;
						mtxScl = XMMatrixScaling(1.0f / m_vScale.x, 1.0f / m_vScale.y, 1.0f / m_vScale.z);
						XMStoreFloat4x4(&mtxWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_mWorld), mtxScl));

						int Handle = CEffekseerManager::Play(m_EfkHandle->t_Path[0], false, 30);
						CEffekseerManager::SetMatrix(Handle, false,mtxWorld);
					}
					CSound::Play(SE_EXPLOSION);
				}
				break;
			}
		}
	}
}

void CEnemy_Sphere::Draw()		// �`��
{
	CModel::Draw();
}