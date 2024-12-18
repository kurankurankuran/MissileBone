// �ǔ��~�T�C�� [Machinegun.cpp]
#include "Machinegun.h"
#include "Scene.h"
#include "Enemy.h"
#include "Explosion.h"
#include "Game.h"
//#include "EffectManager.h"
#include "EffekseerManager.h"
#include "MGManager.h"
//#include "Texture.h"
//#include "Sound.h"

// �萔
namespace {
	const float g_fSpeed = 270.0f;
	//const float g_fRotateSpeed = 1.5f;	// �P��:�x
	const float g_fSpeedEnemy = 50.0f;

	const int g_nMaxLife = 300;

	const int g_nDamage = 2;
	/*const char16_t g_pszEfkPath[] = {
		u"Laser01_Scale.efk",
	};*/
}



// �R���X�g���N�^
CMachinegun::CMachinegun(CScene* pScene) : CWeapon(pScene)
{
	SetID(GOT_GAMEOBJ);
	

}

// �f�X�g���N�^
CMachinegun::~CMachinegun()
{
}

// ������
HRESULT CMachinegun::Init()
{
	HRESULT hr = S_OK;

	//SetModel(MODEL_BOSS);
	CGameObj::SetRadius(250.0f);
	CGameObj::SetBBox(XMFLOAT3(40.f, 40.f, 40.0f));
	m_fSpeed = g_fSpeed;
	m_nLife = g_nMaxLife;
	/*m_vScale = XMFLOAT3(100.0f, 100.0f, 1.0f);*/
	//m_vScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_vPos = XMFLOAT3(0.0f, -30.0f, 0.0f);
	// ���[���h�ϊ��s��擾
	XMFLOAT4X4 mW = GetWorld();

	/*XMMATRIX mtxScl;

	mtxScl = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	XMStoreFloat4x4(&mW, XMMatrixMultiply(XMLoadFloat4x4(&mW), mtxScl));*/
	// ���f�����WY���擾
	XMFLOAT3 vY = XMFLOAT3(mW._21, mW._22, mW._23);
	// ���W�擾�A�N���A
	m_vPos = XMFLOAT3(mW._41, mW._42, mW._43);
	mW._41 = mW._42 = mW._43 = 0.0f;

	m_vPos.x -= vY.x * 30.0f;
	m_vPos.y -= vY.y * 30.0f;
	m_vPos.z -= vY.z * 30.0f;

	// ���[���h�ϊ��s��ɔ��f
	mW._41 = m_vPos.x;
	mW._42 = m_vPos.y;
	mW._43 = m_vPos.z;
	SetWorld(mW);


	//// ���b�V���̃��[���h�s����X�V
	//m_Mesh.SetWorld(&mW);

	//
	//HRESULT hr;
	//// ���b�V���쐬
	//const static float vtx[] = {
	//	-0.5f, -0.5f,  0.0f,
	//	-0.5f,  0.5f,  0.0f,
	//	 0.5f, -0.5f,  0.0f,
	//	 0.5f,  0.5f,  0.0f,
	//};
	//const static float nor[] = {
	//	 0.0f,  0.0f, -1.0f,
	//	 0.0f,  0.0f, -1.0f,
	//	 0.0f,  0.0f, -1.0f,
	//	 0.0f,  0.0f, -1.0f,
	//};

	//const static int Index[] = {
	//	0, 1, 2, 3

	//};

	//const static float uv[] = {
	//	0.0f, 1.0f,
	//	0.0f, 0.0f,
	//	1.0f, 1.0f,
	//	1.0f, 0.0f,
	//};
	//VERTEX_3D* pVertexWk = new VERTEX_3D[4];
	//int* pIndexWk = new int[4];
	//VERTEX_3D* pVtx = pVertexWk;
	//int nIdx = 0;

	//for (int j = 0; j < 4; ++j) {
	//	pVtx->vtx.x = vtx[Index[j] * 3 + 0];
	//	pVtx->vtx.y = vtx[Index[j] * 3 + 1];
	//	pVtx->vtx.z = vtx[Index[j] * 3 + 2];
	//	pVtx->nor.x = nor[j * 3 + 0];
	//	pVtx->nor.y = nor[j * 3 + 1];
	//	pVtx->nor.z = nor[j * 3 + 2];
	//	pVtx->tex.x = uv[j * 2 + 0];
	//	pVtx->tex.y = uv[j * 2 + 1];
	//	pVtx->diffuse = XMFLOAT4(1, 1, 1, 1);
	//	++pVtx;
	//	//pIndexWk[/*i * 6 + */j] = nIdx;
	//	//++nIdx;
	//}
	//for (int i = 0; i < 4; ++i) {
	//	pIndexWk[i] = Index[i];
	//}

	//hr = m_Mesh.Init(pVertexWk, 4, pIndexWk, 4);
	//delete[] pIndexWk;
	//delete[] pVertexWk;


	
	

	return hr;
}

void CMachinegun::Fin()
{
	CEffekseerManager::Stop(m_nHandle, true);
	//m_Effect.Fin();
	
	//m_Mesh.Fin();
	//CModel::Fin();
}

// �X�V
void CMachinegun::Update()
{
	if (!m_bUnit) {
		--m_nLife;
		if (m_nLife <= 0) {
			Fin();
			delete this;
			return;

		}



		// ���[���h�ϊ��s��擾
		XMFLOAT4X4 mW = GetWorld();
		// ���f�����W���擾
		XMFLOAT3 vX = XMFLOAT3(mW._11, mW._12, mW._13);
		XMFLOAT3 vY = XMFLOAT3(mW._21, mW._22, mW._23);
		XMFLOAT3 vZ = XMFLOAT3(mW._31, mW._32, mW._33);
		// ���W�擾�A�N���A
		m_vPos = XMFLOAT3(mW._41, mW._42, mW._43);
		mW._41 = mW._42 = mW._43 = 0.0f;


		// �O�i
		m_vPos.x += vZ.x * m_fSpeed;
		m_vPos.y += vZ.y * m_fSpeed;
		m_vPos.z += vZ.z * m_fSpeed;

		// ���[���h�ϊ��s��ɔ��f
		mW._41 = m_vPos.x;
		mW._42 = m_vPos.y;
		mW._43 = m_vPos.z;
		SetWorld(mW);

		//CEffekseerManager::SetLocation(m_nHandle, m_vPos);
		CEffekseerManager::SetMatrix(m_nHandle, true, m_mWorld);
		
		// ���b�V���̃��[���h�s����X�V
		//m_Mesh.SetWorld(&mW);

		// �Փ˔���
		// ��ɑS�ẴG�l�~�[�Ƃ̏Փ˔���
		CEnemy* pEnemy;
		CPlayer* pPlayer;
		switch (m_Type) {
		case GOT_BOSS:
		case GOT_ENEMY:
			pEnemy = (CEnemy*)m_pScene->Find(GOT_ENEMY);
			while (pEnemy) {
				if (pEnemy->GetVisible()) {
					switch (((CGame*)m_pScene)->GetBound()) {
					case BOUND_AABB:	// ���E�{�b�N�X
						if (CollisionAABB(pEnemy)) {
							pEnemy->AddLife(-g_nDamage);
							
							CSound::Play(SE_DEFEND);
							if (m_EfkHandle != nullptr) {
								int Handle = CEffekseerManager::Play(m_EfkHandle->t_Path[CMGManager::EfkType::EXPLOSION_TYPE], false, 30);
								CEffekseerManager::SetMatrix(m_nHandle, false, m_mWorld);
								/*XMFLOAT3 vScl(0.5f, 0.5f, 0.5f);
								CEffekseerManager::SetScale(Handle, vScl);*/
							}
							/*if (pEnemy->GetLife() <= 0) {

								pEnemy->StartExplosion();
								pEnemy->SetLife(pEnemy->GetMaxLife());

								CSound::Play(SE_EXPLOSION);

							}*/
							Fin();
							delete this;
							return;

						}
						break;
					case BOUND_SPHERE:	// ���E��
						if (CollisionBS(pEnemy)) {
							pEnemy->AddLife(-g_nDamage);
							
							CSound::Play(SE_DEFEND);
							if (m_EfkHandle != nullptr) {
								int Handle = CEffekseerManager::Play(m_EfkHandle->t_Path[CMGManager::EfkType::EXPLOSION_TYPE], false, 30);
								CEffekseerManager::SetMatrix(Handle,false, m_mWorld);
								/*XMFLOAT3 vScl(0.5f, 0.5f, 0.5f);
								CEffekseerManager::SetScale(Handle, vScl);*/
							}
							/*if (pEnemy->GetLife() <= 0) {

								pEnemy->StartExplosion();
								pEnemy->SetLife(pEnemy->GetMaxLife());

								CSound::Play(SE_EXPLOSION);

							}*/
							Fin();
							delete this;
							return;
						}
						break;

					default:
						if (CollisionOBB(pEnemy)) {
							
							pEnemy->AddLife(-g_nDamage);
							/*CEffectManager::StartEffect(
								EFFECT_EXPLOSION,
								m_vPos, 
								XMFLOAT2(1000.0f, 1000.0f));*/
							CSound::Play(SE_DEFEND);
							if (m_EfkHandle != nullptr) {
								int Handle = CEffekseerManager::Play(m_EfkHandle->t_Path[CMGManager::EfkType::EXPLOSION_TYPE], false, 30);
								CEffekseerManager::SetMatrix(Handle, false,m_mWorld);
								/*XMFLOAT3 vScl(0.5f, 0.5f, 0.5f);
								CEffekseerManager::SetScale(Handle, vScl);*/
							}
							/*if (pEnemy->GetLife() <= 0) {

								pEnemy->StartExplosion();
								pEnemy->SetLife(pEnemy->GetMaxLife());

								CSound::Play(SE_EXPLOSION);
							}*/
							Fin();
							delete this;
							return;
						}
						break;
					}
				}

				if (pEnemy->GetID() == GOT_BOSS)break;
				pEnemy = (CEnemy*)m_pScene->Find(GOT_ENEMY, pEnemy);
				if (!pEnemy) {
					pEnemy = (CEnemy*)m_pScene->Find(GOT_BOSS, pEnemy);
				}
			}
			break;
		case GOT_PLAYER:
			m_fSpeed = g_fSpeedEnemy;

			pPlayer = (CPlayer*)m_pScene->Find(GOT_PLAYER);
			m_pTarget = nullptr;

			if (pPlayer->GetVisible()) {
				if (m_EfkHandle != nullptr) {
					int Handle = CEffekseerManager::Play(m_EfkHandle->t_Path[CMGManager::EfkType::EXPLOSION_TYPE], false, 30);
					CEffekseerManager::SetMatrix(Handle, false,m_mWorld);
				}

				Fin();
				delete this;
				return;
			}
		
			break;
		}
				
	}
	


	//m_Effect.Update();

}

void CMachinegun::Draw()
{
	if (m_bUnit)return;

	
	

	//// ���Z����
	//SetBlendState(BS_ADDITIVE);
	//
	//// �����ŃI�t�ɂ���
	//m_pScene->GetLight()->SetEnable(false);

	//m_Mesh.SetCamera(m_pScene->GetCamera());
	//m_Mesh.SetLight(m_pScene->GetLight());

	//// �r���[�}�g���b�N�X���擾
	//XMFLOAT4X4& mtxView = m_pScene->GetCamera()->GetViewMatrix();
	//XMFLOAT4X4* pWorld = m_Mesh.GetWorld();
	//XMMATRIX mtxWorld, mtxScl, mtxTranslate;

	//// �r���{�[�h����
	//mtxWorld = XMMatrixIdentity();
	//XMStoreFloat4x4(pWorld, mtxWorld);

	//pWorld->_11 = mtxView._11;
	//pWorld->_12 = mtxView._21;
	//pWorld->_13 = mtxView._31;
	//pWorld->_21 = mtxView._12;
	//pWorld->_22 = mtxView._22;
	//pWorld->_23 = mtxView._32;
	//pWorld->_31 = mtxView._13;
	//pWorld->_32 = mtxView._23;
	//pWorld->_33 = mtxView._33;


	//// �p�����[�^�ݒ�
	//mtxWorld = XMLoadFloat4x4(pWorld);

	//mtxScl = XMMatrixScaling(m_vScale.x, m_vScale.y, 1.0f);
	//mtxWorld = XMMatrixMultiply(mtxScl, mtxWorld);

	//mtxTranslate = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//XMStoreFloat4x4(pWorld, mtxWorld);

	//m_Mesh.SetWorld(pWorld);


	//
	//m_Mesh.Draw();

	//m_pScene->GetLight()->SetEnable(true);

	//
	//SetBlendState(BS_NONE);		// �A���t�@�u�����h����

}

// ����
int CMachinegun::Fire(XMFLOAT4X4* pWorld, EGameObjType Type, CModel* pModel)
{
	CScene* pScene = CScene::GetCurrent();
	if (!pScene)
		return -1;
	CMachinegun* pMachinegun = new CMachinegun(pScene);
	pMachinegun->SetWorld(*pWorld);
	pMachinegun->Init();
	pMachinegun->m_pTarget = pModel;
	pMachinegun->m_Type = Type;

	pMachinegun->SetBSVisible((EBoundingShape)((CGame*)pScene)->GetBound());

	

	return 1;
}

int CMachinegun::Fire()
{
	
	m_bUnit = false;
	
	return 1;
}

