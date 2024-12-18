//=============================================================================
//
// �I�u�W�F�N�g �N���X���� [GameObj.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include "GameObj.h"
#include "Scene.h"

// �O���[�o���ϐ�
namespace {
	const CMeshMaterial g_material = {
		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.3f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f
	};
	const XMFLOAT4 g_vRed(1.0f, 0.0f, 0.0f, 0.3f);
	const XMFLOAT4 g_vGreen(0.0f, 1.0f, 0.0f, 0.3f);
}

// �R���X�g���N�^
CGameObj::CGameObj(CScene* pScene) : m_pScene(pScene)
{
	m_id = GOT_GAMEOBJ;

	m_pBack = NULL;
	CGameObj* pTop = m_pScene->GetObj();
	m_pNext = pTop;
	if (pTop) {
		pTop->m_pBack = this;
	}
	m_pScene->SetObj(this);

	m_vPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vAccel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vScale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());

	m_vCenter = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vBBox = XMFLOAT3(0.5f, 0.5f, 0.5f);
	m_fRadius = 0.5f;

	m_pBBox = nullptr;
	m_pBSphere = nullptr;
	//m_bBBox = m_bBSphere = false;
	m_eBS = BOUND_NONE;

	m_bVisible = true;

	m_nLife = 0;
	m_nMaxLife = 0;
}

// �f�X�g���N�^
CGameObj::~CGameObj(void)
{
	if (m_pNext) {
		m_pNext->m_pBack = m_pBack;
	}
	if (m_pBack) {
		m_pBack->m_pNext = m_pNext;
	} else {
		m_pScene->SetObj(m_pNext);
	}
}

// ������
HRESULT CGameObj::Init()
{
	return S_OK;
}

// ���
void CGameObj::Fin()
{
}

// �X�V
void CGameObj::Update()
{
	m_vVel.x += m_vAccel.x;
	m_vVel.y += m_vAccel.y;
	m_vVel.z += m_vAccel.z;

	m_vPos.x += m_vVel.x;
	m_vPos.y += m_vVel.y;
	m_vPos.z += m_vVel.z;

	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vAngle.x),
		XMConvertToRadians(m_vAngle.y), XMConvertToRadians(m_vAngle.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&m_mWorld, mtxWorld);
}

// �`��
void CGameObj::Draw()
{
}

// �������`��
void CGameObj::DrawAlpha()
{
	//if (m_bBBox || m_bBSphere) {
	if (m_eBS != BOUND_NONE) {
		XMVECTOR vCenter = XMLoadFloat3(&m_vCenter);
		int nCurCullNum = GetCurCullMode();

		if (m_eBS == BOUND_OBB) {
			XMMATRIX mWorld = XMMatrixTranslationFromVector(vCenter);
			mWorld *= XMLoadFloat4x4(&m_mWorld);
		
			XMFLOAT4X4 mW;
			XMStoreFloat4x4(&mW, mWorld);
			if (m_pBBox) {
				SetCullMode(CULLMODE_NONE);//�J�����O����
				m_pBBox->SetWorld(&mW);
				m_pBBox->Draw();
				SetCullMode(nCurCullNum);//���̃J�����O�ɖ߂�
			}
		}
		else
		{
			XMMATRIX mWorld = XMLoadFloat4x4(&m_mWorld);
			vCenter = XMVector3TransformCoord(vCenter, mWorld);
			mWorld = XMMatrixTranslationFromVector(vCenter);
			XMFLOAT4X4 mW;
			XMStoreFloat4x4(&mW, mWorld);
			//if (m_pBBox && m_bBBox) {
			if (m_pBBox && m_eBS == BOUND_AABB) {
				SetCullMode(CULLMODE_NONE);//�J�����O����
				m_pBBox->SetWorld(&mW);
				m_pBBox->Draw();
				SetCullMode(nCurCullNum);//���̃J�����O�ɖ߂�
			}
			if (m_pBSphere && m_eBS == BOUND_SPHERE) {
				m_pBSphere->SetWorld(&mW);
				m_pBSphere->Draw();
			
			}

		}
		//if (m_pBSphere && m_bBSphere) {
		//if (m_pBSphere && m_eBS == BOUND_SPHERE) {
		//	if (m_id != GOT_STAGESPHERE) {
		//		m_pBSphere->SetWorld(&mW);
		//		m_pBSphere->Draw();
		//		
		//	}
		//	else
		//	{
		//		SetCullMode(CULLMODE_NONE);//�J�����O����
		//		m_pBSphere->SetWorld(&mW);
		//		m_pBSphere->Draw();
		//		SetCullMode(CULLMODE_CCW);//�w�ʃJ�����O
		//	}
		//}
	}
}

// �S������
HRESULT CGameObj::InitAll(CGameObj* pTop)
{
	HRESULT hr = S_OK;
	for (CGameObj* pObj = pTop; pObj; pObj = pObj->m_pNext) {
		hr = pObj->Init();
		if (FAILED(hr)) {
			return hr;
		}
	}
	return hr;
}

// �S�X�V
void CGameObj::UpdateAll(CGameObj* pTop)
{
	CGameObj* pNext;
	for (CGameObj* pObj = pTop; pObj; pObj = pNext) {
		pNext = pObj->m_pNext;
		pObj->Update();
	}
}

// �S�`��
void CGameObj::DrawAll(CGameObj* pTop)
{
	// �s���������`��
	for (CGameObj* pObj = pTop; pObj; pObj = pObj->m_pNext) {
		//if (pObj->m_bVisible) {
			pObj->Draw();
		//}
	}

	// �����������`��
	SetBlendState(BS_ALPHABLEND);
	SetZWrite(false);
	for (CGameObj* pObj = pTop; pObj; pObj = pObj->m_pNext) {
		//if (pObj->m_bVisible) {
			pObj->DrawAlpha();
		//}
	}
	SetZWrite(true);
	SetBlendState(BS_NONE);
}

// �S�I������
void CGameObj::FinAll(CGameObj* pTop)
{
	CGameObj* pNext;
	for (CGameObj* pObj = pTop; pObj; pObj = pNext) {
		pNext = pObj->m_pNext;
		pObj->Fin();
		delete pObj;
	}
}

// OBB���m�̏Փ˔���
bool CGameObj::CollisionOBB(CGameObj* pOther)
{
	if(pOther == this) return false;
	if (!m_bVisible || !pOther->m_bVisible)
		return false;
	// ���[���h �}�g���b�N�X�擾
	XMFLOAT4X4 mW1 = m_mWorld;
	XMFLOAT4X4 mW2 = pOther->GetWorld();
	// ���S���W�擾
	XMFLOAT3 vP1, vP2;
	XMStoreFloat3(&vP1, XMVector3TransformCoord(XMLoadFloat3(&m_vCenter), XMLoadFloat4x4(&mW1)));
	XMStoreFloat3(&vP2, XMVector3TransformCoord(XMLoadFloat3(&pOther->GetCenter()),XMLoadFloat4x4(&mW2)));
	// ���S���W�Ԃ̃x�N�g�������߂�
	XMVECTOR vD;
	vD = XMVectorSet(vP2.x - vP1.x, vP2.y - vP1.y, vP2.z - vP1.z, 0.0f);
	// ���f�����W�������߂�
	XMVECTOR vN[6];
	vN[0] = XMVectorSet(mW1._11, mW1._12, mW1._13, 0.0f);
	vN[1] = XMVectorSet(mW1._21, mW1._22, mW1._23, 0.0f);
	vN[2] = XMVectorSet(mW1._31, mW1._32, mW1._33, 0.0f);
	vN[3] = XMVectorSet(mW2._11, mW2._12, mW2._13, 0.0f);
	vN[4] = XMVectorSet(mW2._21, mW2._22, mW2._23, 0.0f);
	vN[5] = XMVectorSet(mW2._31, mW2._32, mW2._33, 0.0f);
	// OBB�̑傫��(����)���|�����x�N�g�������߂�
	XMFLOAT3& vBB1 = m_vBBox;
	XMFLOAT3& vBB2 = pOther->GetBBox();
	XMVECTOR vL[6];
	vL[0] = XMVectorSet(mW1._11 * vBB1.x, mW1._12 * vBB1.x, mW1._13 * vBB1.x, 0.0f);
	vL[1] = XMVectorSet(mW1._21 * vBB1.y, mW1._22 * vBB1.y, mW1._23 * vBB1.y, 0.0f);
	vL[2] = XMVectorSet(mW1._31 * vBB1.z, mW1._32 * vBB1.z, mW1._33 * vBB1.z, 0.0f);
	vL[3] = XMVectorSet(mW2._11 * vBB2.x, mW2._12 * vBB2.x, mW2._13 * vBB2.x, 0.0f);
	vL[4] = XMVectorSet(mW2._21 * vBB2.y, mW2._22 * vBB2.y, mW2._23 * vBB2.y, 0.0f);
	vL[5] = XMVectorSet(mW2._31 * vBB2.z, mW2._32 * vBB2.z, mW2._33 * vBB2.z, 0.0f);
	// ��������₪�ʂ̖@���x�N�g��(���f�����W��)
	float fL, f, fD;
	for (int i = 0; i < 6; ++i) {
		XMVECTOR& vS = vN[i];//���������
		// OBB�̉e(�̔���)�̍��v
		fL = 0.0f;
		for (int j = 0; j < 6; ++j) {
			XMStoreFloat(&f, XMVector3Dot(vS, vL[j]));
			fL += fabsf(f);
		}
		// ���S���W�Ԃ̋����Ɣ�r
		XMStoreFloat(&fD, XMVector3Dot(vS, vD));
		fD = fabsf(fD);
		if (fL < fD) return false;// �������Ă��Ȃ�
	}
	// ��������₪���ꂼ��̕ӂɐ����ȃx�N�g��
	XMVECTOR vS;
	for (int i = 0; i < 3; ++i) {
		for (int j = 3; j < 6; ++j) {
			// �������������߂�
			vS = XMVector3Normalize(XMVector3Cross(vN[i], vN[j]));
			// OBB�̉e(�̔���)�̍��v
			fL = 0.0f;
			for (int k = 0; k < 6; ++k) {
				XMStoreFloat(&f, XMVector3Dot(vS, vL[k]));
				fL += fabsf(f);
			}
			// ���S���W�Ԃ̋����Ɣ�r
			XMStoreFloat(&fD, XMVector3Dot(vS, vD));
			fD = fabsf(fD);
			if (fL < fD) return false;// �������Ă��Ȃ�
		}
	}
	return true;// �������Ă���
}


// AABB���m�̏Փ˔���
bool CGameObj::CollisionAABB(CGameObj* pOther)
{
	if (pOther == this) return false;
	if (!m_bVisible || !pOther->m_bVisible)
		return false;
	// ���S���W�����߂�
	XMFLOAT3 vA, vB;
	XMStoreFloat3(&vA,
		XMVector3TransformCoord(
			XMLoadFloat3(&m_vCenter),
			XMLoadFloat4x4(&m_mWorld)));
	XMStoreFloat3(&vB,
		XMVector3TransformCoord(
			XMLoadFloat3(&pOther->m_vCenter),
			XMLoadFloat4x4(&pOther->m_mWorld)));
	// �Փ˔���
	XMFLOAT3& vAr = m_vBBox;
	XMFLOAT3& vBr = pOther->m_vBBox;
	return vA.x - vAr.x <= vB.x + vBr.x &&
		vB.x - vBr.x <= vA.x + vAr.x &&
		vA.y - vAr.y <= vB.y + vBr.y &&
		vB.y - vBr.y <= vA.y + vAr.y &&
		vA.z - vAr.z <= vB.z + vBr.z &&
		vB.z - vBr.z <= vA.z + vAr.z;
}

// ���E�����m�̏Փ˔���
bool CGameObj::CollisionBS(CGameObj* pOther)
{
	if (pOther == this) return false;
	if (!m_bVisible || !pOther->m_bVisible)
		return false;
	// ���S���W�����߂�
	XMFLOAT3 vA, vB;
	XMStoreFloat3(&vA,
		XMVector3TransformCoord(
			XMLoadFloat3(&m_vCenter),
			XMLoadFloat4x4(&m_mWorld)));
	XMStoreFloat3(&vB,
		XMVector3TransformCoord(
			XMLoadFloat3(&pOther->m_vCenter),
			XMLoadFloat4x4(&pOther->m_mWorld)));
	// �Փ˔���
	float dx = vA.x - vB.x;
	float dy = vA.y - vB.y;
	float dz = vA.z - vB.z;
	float dr = m_fRadius + pOther->m_fRadius;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}

// ���E�{�b�N�X�ݒ�
void CGameObj::SetBBox(XMFLOAT3 vBBox)
{
	if (m_pBBox) {
		m_pBBox->Fin();
		SAFE_DELETE(m_pBBox);
	}

	m_vBBox = vBBox;

	m_pBBox = new CBox();
	HRESULT hr = m_pBBox->Init(vBBox);
	if (FAILED(hr)) {
		m_pBBox->Fin();
		SAFE_DELETE(m_pBBox);
	}
	m_pBBox->SetCamera(m_pScene->GetCamera());
	m_pBBox->SetLight(m_pScene->GetLight());
	m_pBBox->SetMaterial(&g_material);
}

// ���E���ݒ�
void CGameObj::SetRadius(float fRadius)
{
	if (m_pBSphere) {
		m_pBSphere->Fin();
		SAFE_DELETE(m_pBSphere);
	}

	m_fRadius = fRadius;

	m_pBSphere = new CSphere();
	HRESULT hr = m_pBSphere->Init(16, 8, fRadius);
	if (FAILED(hr)) {
		m_pBSphere->Fin();
		SAFE_DELETE(m_pBSphere);
	}
	m_pBSphere->SetCamera(m_pScene->GetCamera());
	m_pBSphere->SetLight(m_pScene->GetLight());
	m_pBSphere->SetMaterial(&g_material);
}

//// ���E�{�b�N�X�\������
//void CGameObj::SetBBoxVisible(bool bVisible)
//{
//	m_bBBox = bVisible;
//	if (bVisible && m_bBSphere)
//		m_bBSphere = false;
//}
//
//// ���E���\������
//void CGameObj::SetBSphereVisible(bool bVisible)
//{
//	m_bBSphere = bVisible;
//	if (bVisible && m_bBBox)
//		m_bBBox = false;
//}


XMFLOAT3 CGameObj::SlideOBB(CGameObj* pLand, XMFLOAT3 vVec, XMFLOAT3 f3Pos/*XMFLOAT4X4& world, XMFLOAT3 vScale, XMFLOAT3 vVec*/)
{
	if (!CollisionOBB(pLand))return vVec;

	return vVec;
	


	//if (pLand == nullptr)return vVec;

	//XMFLOAT4X4 world = pLand->GetWorld();
	//// ����̍��W��world.
	//XMFLOAT3 vX = XMFLOAT3(world._11 / 2.0f, world._12 / 2.0f, world._13 / 2.0f);
	//XMFLOAT3 vY = XMFLOAT3(world._21 / 2.0f, world._22 / 2.0f, world._23 / 2.0f);
	//XMFLOAT3 vZ = XMFLOAT3(world._31 / 2.0f, world._32 / 2.0f, world._33 / 2.0f);

	//// ����̒��S���W
	//XMFLOAT3 vPos = XMFLOAT3(world._41, world._42, world._43);

	//// ����̊e�ʏ�̒��S���W
	//XMFLOAT3 vSurface[6];
	//vSurface[0] = XMFLOAT3(vPos.x + vX.x, vPos.y + vX.y, vPos.z + vX.z);
	//vSurface[1] = XMFLOAT3(vPos.x + vY.x, vPos.y + vY.y, vPos.z + vY.z);
	//vSurface[2] = XMFLOAT3(vPos.x + vZ.x, vPos.y + vZ.y, vPos.z + vZ.z);
	//vSurface[3] = XMFLOAT3(vPos.x - vX.x, vPos.y - vX.y, vPos.z - vX.z);
	//vSurface[4] = XMFLOAT3(vPos.x - vY.x, vPos.y - vY.y, vPos.z - vY.z);
	//vSurface[5] = XMFLOAT3(vPos.x - vZ.x, vPos.y - vZ.y, vPos.z - vZ.z);


	//XMVECTOR vNor;
	//XMFLOAT3 vSlideVec;

	//for (int i = 0; i < 6; ++i) {


	//	XMVECTOR V1 = XMVectorSet(f3Pos.x - vSurface[i].x, f3Pos.y - vSurface[i].y, f3Pos.z - vSurface[i].z, 1.0f);
	//	XMVECTOR V2;
	//	XMVECTOR Nor;
	//	V2 = XMVectorSet(vPos.x - vSurface[i].x, vPos.y - vSurface[i].y, vPos.z - vSurface[i].z, 1.0f);
	//	Nor = XMVectorSet(vSurface[i].x - vPos.x, vSurface[i].y - vPos.y, vSurface[i].z - vPos.z, 1.0f);


	//	if (XMVectorGetX(XMVector3Dot(V1, Nor) * XMVectorGetX(XMVector3Dot(V2, Nor))) <= 0) {

	//		vNor = XMVector3Normalize(Nor);
	//		break;
	//	}
	//	else if (i >= 5)
	//	{
	//		return vSlideVec = vVec;
	//	}
	//}


	//XMVECTOR Vec = XMLoadFloat3(&vVec);

	//XMStoreFloat3(&vSlideVec, XMVectorSubtract(Vec, XMVectorScale(vNor, XMVectorGetX(XMVector3Dot(Vec, -vNor)))));

	//return vSlideVec;


}
