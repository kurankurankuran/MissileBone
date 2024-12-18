// �G�@ [Enemy.cpp]
#include "Enemy.h"
#include "Input.h"
#include "Player.h"
#include "Game.h"
#include "Missile.h"
#include "Sound.h"
#include "Explosion.h"
#include "EffectManager.h"
#include "EffekseerManager.h"

// �萔
namespace {
	const float g_fSpeed = 7.0f;
	const float g_fRollSpeed = 2.0f;//�P��:�x
	const float g_fPitchSpeed = 1.0f;//�P��:�x

	const int g_nFireCnt = 120;
	const float g_fBS = 5000.0f;
	const float g_fAtkBS = 2000.0f;

	const int g_nMaxLife = 10;


	/*EfkPath g_pszEfkPath = {
		u"FiterSmoke.efk",
	};*/
}

// �R���X�g���N�^
CEnemy::CEnemy(CScene* pScene) : CModel(pScene)
{
	SetID(GOT_ENEMY);

	m_pPlayer = nullptr;
}

// �f�X�g���N�^
CEnemy::~CEnemy()
{
}

// ������
HRESULT CEnemy::Init()
{
	m_nLife = g_nMaxLife;
	m_nEffectCnt = 0;
	m_nFireCnt = g_nFireCnt;

	SetModel(MODEL_ENEMY);
	CGameObj::SetRadius(250.0f);
	CGameObj::SetBBox(XMFLOAT3(40.f, 20.f, 60.0f));

	XMFLOAT4X4 mW;
	XMStoreFloat4x4(&mW, XMMatrixRotationY(XMConvertToRadians(180.0f)));

	XMMATRIX mtxScl;
	m_vScale = XMFLOAT3(5.0f, 5.0f, 5.0f);
	mtxScl = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	XMStoreFloat4x4(&mW, XMMatrixMultiply(XMLoadFloat4x4(&mW), mtxScl));

	SetWorld(mW);

	// �G�t�F�N�V�A�t�@�C����ǂݍ���
	/*if (!CEffekseerManager::Load(g_pszEfkPath.t_Path[0]))
	{
		return false;
	}*/

	//m_Effect.Init(m_pScene);

	return CModel::Init();
}

// �I������
void CEnemy::Fin()
{
	//m_Effect.Fin();
	CModel::Fin();
}

// �X�V
void CEnemy::Update()
{
	if (m_pExp) {
		if (m_uExpTimer) {
			--m_uExpTimer;
			if (!m_uExpTimer) {
				m_pExp->FinDeb();

				
				//SetVisible();
			}
		}
		m_pExp->Update();
	}

	if (m_nLife <= 0)
	{
		m_nLife = g_nMaxLife;

		//StartExplosion();
		m_bVisible = false;
		CSound::Play(SE_EXPLOSION);

		if (m_EfkHandle != nullptr) {
			int Handle = CEffekseerManager::Play(m_EfkHandle->t_Path[0], false, 100);
			XMFLOAT3 vScl(1.0f, 1.0f, 1.0f);
			XMMATRIX mtxWorld = XMMatrixIdentity();
			XMMATRIX mtxScl,mtxTrans;
			mtxScl = XMMatrixScaling(vScl.x, vScl.y, vScl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);
			mtxTrans = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTrans);
			XMFLOAT4X4 World;
			XMStoreFloat4x4(&World, mtxWorld);
			CEffekseerManager::SetMatrix(Handle, false,World);
		}
	}

	if (m_bVisible) {

		// ���[���h�ϊ��s��擾
		XMFLOAT4X4 mW = GetWorld();
		// ���f�����W���擾
		XMFLOAT3 vX = XMFLOAT3(mW._11, mW._12, mW._13);
		XMFLOAT3 vY = XMFLOAT3(mW._21, mW._22, mW._23);
		XMFLOAT3 vZ = XMFLOAT3(mW._31, mW._32, mW._33);
		// ���W�擾�A�N���A
		m_vPos = XMFLOAT3(mW._41, mW._42, mW._43);
		mW._41 = mW._42 = mW._43 = 0.0f;






		// �X�e�[�W�O�ɏo���Ƃ������Ɍ����Ĕ��]����
		if (m_pTarget) {
			if (!CollisionBS(m_pTarget)) {
				// �^�[�Q�b�g�ւ̃x�N�g�����擾
				XMFLOAT3& vTargetPos = m_pTarget->GetPos();
				XMFLOAT3 vTarget;
				vTarget.x = vTargetPos.x - m_vPos.x;
				vTarget.y = vTargetPos.y - m_vPos.y;
				vTarget.z = vTargetPos.z - m_vPos.z;



				// �^�[�Q�b�g���㉺���E�ǂ��炩
				XMMATRIX mR = XMMatrixIdentity();

				float fRight = vX.x * vTarget.x + vX.y * vTarget.y + vX.z * vTarget.z;	// ����
				if (fRight > 0.0f)		// �E
					mR *= XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(-g_fRollSpeed));
				else if (fRight < 0.0f)	// ��
					mR *= XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(g_fRollSpeed));
				float fUp = vY.x * vTarget.x + vY.y * vTarget.y + vY.z * vTarget.z;		// ����
				if (fUp > 0.0f)			// ��
					mR *= XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(-g_fPitchSpeed));
				else if (fUp < 0.0f)	// ��
					mR *= XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(g_fPitchSpeed));

				if (!fRight && !fUp) {
					mR *= XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(-g_fPitchSpeed));
				}

				XMStoreFloat4x4(&mW, XMMatrixMultiply(XMLoadFloat4x4(&mW), mR));
			}
			else
			{
				if (m_pPlayer) {
					if (BS(m_vPos, m_pPlayer->GetPos(), g_fBS, m_pPlayer->GetRadius())) {
						if (m_pPlayer->GetVisible()) {
							// �v���C���[�ւ̃x�N�g�����擾
							XMFLOAT3& vPlayerPos = m_pPlayer->GetPos();
							XMFLOAT3 vPlayer;
							vPlayer.x = vPlayerPos.x - m_vPos.x;
							vPlayer.y = vPlayerPos.y - m_vPos.y;
							vPlayer.z = vPlayerPos.z - m_vPos.z;

							// �^�[�Q�b�g���㉺���E�ǂ��炩
							XMMATRIX mR = XMMatrixIdentity();

							float fRight = vX.x * vPlayer.x + vX.y * vPlayer.y + vX.z * vPlayer.z;	// ����
							if (fRight > 0.0f)		// �E
								mR *= XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(-g_fRollSpeed));
							else if (fRight < 0.0f)	// ��
								mR *= XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(g_fRollSpeed));
							float fUp = vY.x * vPlayer.x + vY.y * vPlayer.y + vY.z * vPlayer.z;		// ����
							if (fUp > 0.0f)			// ��
								mR *= XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(-g_fPitchSpeed));
							else if (fUp < 0.0f)	// ��
								mR *= XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(g_fPitchSpeed));

							if (!fRight && !fUp) {
								mR *= XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(-g_fPitchSpeed));
							}

							XMStoreFloat4x4(&mW, XMMatrixMultiply(XMLoadFloat4x4(&mW), mR));
						}
					}
				}
			}
		}



		// ��ɑO�i
		m_vPos.x += vZ.x * g_fSpeed;
		m_vPos.y += vZ.y * g_fSpeed;
		m_vPos.z += vZ.z * g_fSpeed;

		// ���[���h�ϊ��s��ɔ��f
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


					// �Փ˔���
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
							StartExplosion();

							CSound::Play(SE_EXPLOSION);
						}
						break;
					case BOUND_AABB:
						if (CollisionAABB(m_pPlayer)) {
							m_pPlayer->StartExplosion();
							StartExplosion();

							CSound::Play(SE_EXPLOSION);
						}
						break;
					default:
						if (CollisionOBB(m_pPlayer)) {
							m_pPlayer->StartExplosion();
							StartExplosion();

							CSound::Play(SE_EXPLOSION);
						}
						break;
					}
				}


			}
		}

		// �X���[�N�G�t�F�N�g
		++m_nEffectCnt;
		if (!(m_nEffectCnt % 10)) {
			XMFLOAT3 EffePos;
			EffePos = m_vPos;
			XMFLOAT3 EffeZ = XMFLOAT3(m_mWorld._31, m_mWorld._32, m_mWorld._33);
			EffePos.x -= EffeZ.x * 50;
			EffePos.y -= EffeZ.y * 50;
			EffePos.z -= EffeZ.z * 50;
			//m_Effect.SetEffect(SMOKE_EFFECT, EffePos, XMFLOAT2(100.0f, 100.0f));
			CEffectManager::StartEffect(EFFECT_SMOKE, EffePos, XMFLOAT2(300.0f, 300.0f), true);
		
		/*	int Handle = CEffekseerManager::Play(g_pszEfkPath.t_Path[0], false, 400);
			CEffekseerManager::SetMatrix(Handle, false, m_mWorld);*/
		}
	}


	//m_Effect.Update();
	
	//CModel::Update();
}


void CEnemy::Draw()
{
	CModel::Draw();

	//m_Effect.Draw();
}


// �o�E���f�B�{�b�N�X
bool CEnemy::BS(XMFLOAT3 vA, XMFLOAT3 vB, float RadA, float RadB)
{
	// �Փ˔���
	float dx = vA.x - vB.x;
	float dy = vA.y - vB.y;
	float dz = vA.z - vB.z;
	float dr = RadA + RadB;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}
