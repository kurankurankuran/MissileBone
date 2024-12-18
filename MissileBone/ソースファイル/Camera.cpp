//=============================================================================
//
// �J���� �N���X [Camera.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include "Camera.h"
#include "Input.h"
#include "DebugProc.h"
#include "AssimpModel.h"
#include "Player.h"

#include "Scene.h"
#include "Pause.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CAM_POS_P_X			(0.0f)					// �J�����̎��_�����ʒu(X���W)
#define	CAM_POS_P_Y			(200.0f)				// �J�����̎��_�����ʒu(Y���W)
#define	CAM_POS_P_Z			(-700.0f)				// �J�����̎��_�����ʒu(Z���W)
#define	CAM_POS_TPS_X		(0.0f)					// �J�����̎��_�����ʒu(X���W)
#define	CAM_POS_TPS_Y		(1500.0f)				// �J�����̎��_�����ʒu(Y���W)
#define	CAM_POS_TPS_Z		(-3000.0f)				// �J�����̎��_�����ʒu(Z���W)
//#define	CAM_POS_TPS_Y		(15.0f)				// �J�����̎��_�����ʒu(Y���W)
//#define	CAM_POS_TPS_Z		(-30.0f)				// �J�����̎��_�����ʒu(Z���W)
#define	CAM_POS_R_X			(0.0f)					// �J�����̒����_�����ʒu(X���W)
#define	CAM_POS_R_Y			(0.0f)					// �J�����̒����_�����ʒu(Y���W)
#define	CAM_POS_R_Z			(0.0f)					// �J�����̒����_�����ʒu(Z���W)


#define CAM_ROTANGLE_ROL        (20.0f)                 // ���[���s�b�`���̃J�����̃x�N�g����]��
#define CAM_ROTANGLE_PIT        (10.0f)                 // ���[���s�b�`���̃J�����̃x�N�g����]��
#define CAM_ROTCONFICIENT_ROL   (1.0f / CAM_ROTANGLE_ROL)   // ��]�W��
#define CAM_ROTCONFICIENT_PIT   (1.0f / CAM_ROTANGLE_PIT)   // ��]�W��

#define	CAM_POS_Y_X			(0.0f)					// �J�����̎��_�����ʒu(X���W)
#define	CAM_POS_Y_Y			(35000.0f)				// �J�����̎��_�����ʒu(Y���W)
#define	CAM_POS_Y_Z			(0.0f)				// �J�����̎��_�����ʒu(Z���W)

// �R���X�g���N�^
CCamera::CCamera()
{
	m_pSky = nullptr;
	m_pPlayer = nullptr;
	m_nMode = 3;

	Init();
}

// ������
void CCamera::Init()
{
	m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);		// ���_
	m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);	// �����_
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);		// ����x�N�g��

	m_fAspectRatio = VIEW_ASPECT;			// �c����
	m_fFovY = VIEW_ANGLE;					// ����p(Degree)
	m_fNearZ = VIEW_NEAR_Z;					// �O���N���b�v����
	m_fFarZ = VIEW_FAR_Z;					// ����N���b�v����

	m_fRotValue.x = 0.0f;
	m_fRotValue.z = 0.0f;

	//m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//float fVecX, fVecZ;
	//fVecX = m_vPos.x - m_vTarget.x;
	//fVecZ = m_vPos.z - m_vTarget.z;
	//m_fLengthInterval = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	m_fRadius = 0.5f;

	m_pPlayer = nullptr;
	m_fAngle = 0.0f;

	CreateViewFrustum();
	CalcWorldMatrix();
	UpdateViewFrustum();

	m_nMode = 3;
	m_fFreeBulletMode = XMFLOAT2(0.0f, 0.0f);
	//m_pTargetObj = nullptr;
}

// �X�V
void CCamera::Update()
{
	if (CInput::GetKeyRelease(VK_0))
		m_nMode = 0;
	if (CInput::GetKeyRelease(VK_1))
		m_nMode = 1;
	if (CInput::GetKeyRelease(VK_2))
		m_nMode = 2;
	if (CInput::GetKeyRelease(VK_3))
		m_nMode = 3;
	if (CInput::GetKeyRelease(VK_4))
		m_nMode = 4;
	if (CInput::GetKeyRelease(VK_5))
		m_nMode = 5;


	if (m_pPlayer) {
		switch (m_nMode) {
		case STATIC_CAMERAMODE:	// ��_�J����
			m_vTarget = m_pPlayer->GetPos();
			m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);
			m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
			break;
		case FPS_CAMERAMODE:	// ��l�̃J����
			{
				XMMATRIX mW = XMLoadFloat4x4(&m_pPlayer->GetWorld());
				XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), mW));
				XMStoreFloat3(&m_vTarget, XMVector3TransformCoord(XMVectorSet(0.0f, 22.6f, 259.96f, 1.0f), mW));
				XMStoreFloat3(&m_vPos, XMVector3TransformCoord(XMVectorSet(0.0f, 38.6f, 159.96f, 1.0f), mW));
			}
			break;
		case BULLET_CAMERAMODO:	// �o���b�g�^�C�����J����
			m_fAngle += 1.0f;
		
			m_vTarget = m_pPlayer->GetPos();
			
			m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
			m_vPos.x = m_vTarget.x + sinf(XMConvertToRadians(m_fAngle)) * (m_fRadius * 10.0f);
			m_vPos.y = m_vTarget.y;
			m_vPos.z = m_vTarget.z + cosf(XMConvertToRadians(m_fAngle)) * (m_fRadius * 10.0f);
			break;
		case TPS_CAMERAMODE:	// �O�l�̃J����
		{
			XMFLOAT4X4 mtxW = m_pPlayer->GetWorld();
			XMMATRIX mW = XMLoadFloat4x4(&mtxW/*m_pPlayer->GetWorld()*/);
			XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMVectorSet(0, 1, 0, 1), mW));
			XMStoreFloat3(&m_vTarget, XMVector3TransformCoord(XMVectorSet(PLAYER_POS.x, PLAYER_POS.y + 300.0f, PLAYER_POS.z, 1.0f), mW));
			XMStoreFloat3(&m_vPos, XMVector3TransformCoord(XMVectorSet(CAM_POS_TPS_X, CAM_POS_TPS_Y, CAM_POS_TPS_Z, 1.0f), mW));

			if (CScene::GetPause()->GetPauseFlg())break;
			
			// ******�J��������͕����Ƌt�����ɌX����******
			//�@�v���C���[X��
			XMVECTOR vX = XMVectorSet(mtxW._11, mtxW._12, mtxW._13, 1.0f);
			//�@�v���C���[Z��
			XMVECTOR vZ = XMVectorSet(mtxW._31, mtxW._32, mtxW._33, 1.0f);
			// �s�b�`
			if (CInput::GetKeyPress(VK_W) ) {

				m_fRotValue.x += -CAM_ROTANGLE_PIT * CAM_ROTCONFICIENT_PIT * 2.0f;

			}
			if (CInput::GetKeyPress(VK_S) ) {

				m_fRotValue.x += CAM_ROTANGLE_PIT * CAM_ROTCONFICIENT_PIT * 2.0f;

			}

			// ���[��
			if (CInput::GetKeyPress(VK_A) ) {

				m_fRotValue.z += CAM_ROTANGLE_ROL * CAM_ROTCONFICIENT_ROL * 2.0f;

			}
			if (CInput::GetKeyPress(VK_D) ) {
				m_fRotValue.z += -CAM_ROTANGLE_ROL * CAM_ROTCONFICIENT_ROL * 2.0f;

			}

			// ��]�ʂ����E�l�𒴂����ꍇ�̕␳
			if (m_fRotValue.x * m_fRotValue.x / 4.0f >= CAM_ROTANGLE_PIT * CAM_ROTANGLE_PIT) m_fRotValue.x = CAM_ROTANGLE_PIT;
			if (m_fRotValue.z * m_fRotValue.z / 4.0f >= CAM_ROTANGLE_ROL * CAM_ROTANGLE_ROL) m_fRotValue.z = CAM_ROTANGLE_ROL;

			// ����
			m_fRotValue.x = m_fRotValue.x - m_fRotValue.x * CAM_ROTCONFICIENT_PIT;
			m_fRotValue.z = m_fRotValue.z - m_fRotValue.z * CAM_ROTCONFICIENT_ROL;

			//XMVECTOR vecUp;
			XMVECTOR vecPtoPos = XMVectorAdd(XMLoadFloat3(&m_vPos), -XMLoadFloat3(&m_vTarget));

			XMMATRIX mR = XMMatrixIdentity();
			mR = XMMatrixMultiply(mR, XMMatrixRotationAxis(vX, XMConvertToRadians(m_fRotValue.x)));
			//mR = XMMatrixMultiply(mR, XMMatrixRotationAxis(vZ, XMConvertToRadians(m_fRotValue.z)));

			vecPtoPos = XMVector3Transform(vecPtoPos, mR);
			
			XMStoreFloat3(&m_vPos, vecPtoPos);
			m_vPos.x += m_vTarget.x;
			m_vPos.y += m_vTarget.y;
			m_vPos.z += m_vTarget.z;
			

			// *******************************************************************************************************
			break;

		}	

		case TPS_BUCK_CAMERAMODE: // �O�l�̔w��J����
		{
			XMMATRIX mW = XMLoadFloat4x4(&m_pPlayer->GetWorld());
			XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), mW));
			XMStoreFloat3(&m_vTarget, XMVector3TransformCoord(XMVectorSet(0.0f, 200.0f, 0.0f, 1.0f), mW));
			XMStoreFloat3(&m_vPos, XMVector3TransformCoord(XMVectorSet(CAM_POS_TPS_X, CAM_POS_TPS_Y, -CAM_POS_TPS_Z, 1.0f), mW));


			if (CScene::GetPause()->GetPauseFlg())break;

			XMFLOAT4X4 mtxW = m_pPlayer->GetWorld();
			//�@�v���C���[X��
			XMVECTOR vX = XMVectorSet(mtxW._11, mtxW._12, mtxW._13, 1.0f);
			//�@�v���C���[Z��
			XMVECTOR vZ = XMVectorSet(mtxW._31, mtxW._32, mtxW._33, 1.0f);


			// ******�J��������͕����Ƌt�����ɌX����******
			// �s�b�`
			XMMATRIX mR = XMMatrixIdentity();
			if (CInput::GetKeyPress(VK_W) || CInput::GetKeyPress(VK_UP)) {
				m_fRotValue.x += -CAM_ROTANGLE_PIT * CAM_ROTCONFICIENT_PIT * 2.0f;
			}
			if (CInput::GetKeyPress(VK_S) || CInput::GetKeyPress(VK_DOWN)) {
				m_fRotValue.x += CAM_ROTANGLE_PIT * CAM_ROTCONFICIENT_PIT * 2.0f;
			}

			// ���[��
			if (CInput::GetKeyPress(VK_A) || CInput::GetKeyPress(VK_LEFT)) {
				m_fRotValue.z += -CAM_ROTANGLE_ROL * CAM_ROTCONFICIENT_ROL * 2.0f;
			}
			if (CInput::GetKeyPress(VK_D) || CInput::GetKeyPress(VK_RIGHT)) {
				m_fRotValue.z += CAM_ROTANGLE_ROL * CAM_ROTCONFICIENT_ROL * 2.0f;
			}

			if (m_fRotValue.x * m_fRotValue.x / 4.0f >= CAM_ROTANGLE_PIT * CAM_ROTANGLE_PIT) m_fRotValue.x = CAM_ROTANGLE_PIT;
			if (m_fRotValue.z * m_fRotValue.z / 4.0f >= CAM_ROTANGLE_ROL * CAM_ROTANGLE_ROL) m_fRotValue.z = CAM_ROTANGLE_ROL;

			m_fRotValue.x = m_fRotValue.x - m_fRotValue.x * CAM_ROTCONFICIENT_PIT;
			m_fRotValue.z = m_fRotValue.z - m_fRotValue.z * CAM_ROTCONFICIENT_ROL;

			XMVECTOR vecUp;
			XMVECTOR vecPtoPos = XMVectorAdd(XMLoadFloat3(&m_vPos), -XMLoadFloat3(&m_vTarget));

			mR = XMMatrixMultiply(mR, XMMatrixRotationAxis(vX, XMConvertToRadians(m_fRotValue.x)));
			vecPtoPos = XMVector3Transform(vecPtoPos, mR);

			vecUp = XMVector3Transform(XMLoadFloat3(&m_vUp), mR);

			mR = XMMatrixMultiply(mR, XMMatrixRotationAxis(vZ, XMConvertToRadians(m_fRotValue.z)));
			vecUp = XMVector3Transform(/*XMLoadFloat3(&m_vUp)*/vecUp, mR);

			m_vPos.x = XMVectorGetX(vecPtoPos) + m_vTarget.x;
			m_vPos.y = XMVectorGetY(vecPtoPos) + m_vTarget.y;
			m_vPos.z = XMVectorGetZ(vecPtoPos) + m_vTarget.z;

			vecUp = XMVector3Normalize(vecUp);

			m_vUp.x = XMVectorGetX(vecUp);
			m_vUp.y = XMVectorGetY(vecUp);
			m_vUp.z = XMVectorGetZ(vecUp);

			// *******************************************************************************************************
			break; 
		}
		case BULLET_FREE_CAMERAMODE:	// �o���b�g�^�C�����J����(���R�ӏ�)
			m_fAngle += 1.0f;
			
			m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
			m_vPos.x = m_vTarget.x + sinf(XMConvertToRadians(m_fAngle)) * (m_fFreeBulletMode.x);
			m_vPos.y = m_vTarget.y + m_fFreeBulletMode.y;
			m_vPos.z = m_vTarget.z + cosf(XMConvertToRadians(m_fAngle)) * (m_fFreeBulletMode.x);
			break;
		}
	} else {
		m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);
		m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);
		m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}

	// �}�g���b�N�X�X�V
	UpdateMatrix();

	// ���[���h�}�g���b�N�X�X�V�A������X�V
	CalcWorldMatrix();
	UpdateViewFrustum();

	// �f�o�b�O�\��
	CDebugProc::Print("*** ��� ��� ***\n");
	CDebugProc::Print("FP View:[1]\nBullet-time like:[2]\nTP View:[3]\n:TPBack View[4]\nFixed point:[0]\n");
	CDebugProc::Print("[��� �� : (%f, %f, %f)]\n", m_vPos.x, m_vPos.y, m_vPos.z);
	CDebugProc::Print("[������ : (%f, %f, %f)]\n", m_vTarget.x, m_vTarget.y, m_vTarget.z);
	CDebugProc::Print("\n");
}

// ��ʃN���A
void CCamera::Clear()
{
	// main.cpp�̃��\�[�X�Q��
	extern ID3D11RenderTargetView* g_pRenderTargetView;
	extern ID3D11DepthStencilView* g_pDepthStencilView;

	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	ID3D11DeviceContext* pDC = GetDeviceContext();
	pDC->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	pDC->ClearDepthStencilView(g_pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (m_pSky) {
		SetZBuffer(false);
		m_pSky->SetCamera(this);
		CLight lightOff;
		lightOff.SetDir(XMFLOAT3(0, 0, 0));
		m_pSky->SetLight(&lightOff);
		XMFLOAT4X4 mWorld;
		XMStoreFloat4x4(&mWorld, XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z));
		m_pSky->Draw(pDC, mWorld);
	}

	SetZBuffer(true);
	SetBlendState(BS_NONE);
}

// �r���[/�v���W�F�N�V���� �}�g���b�N�X�X�V
void CCamera::UpdateMatrix()
{
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(
		XMLoadFloat3(&m_vPos), XMLoadFloat3(&m_vTarget), XMLoadFloat3(&m_vUp)));
	XMStoreFloat4x4(&m_mtxProj, XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFovY), m_fAspectRatio, m_fNearZ, m_fFarZ));
}

// ���[���h �}�g���b�N�X�ݒ�
void CCamera::SetWorldMatrix(XMFLOAT4X4& mtxWorld)
{
	m_mtxWorld = mtxWorld;
	m_vPos = XMFLOAT3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	m_vTarget = XMFLOAT3(mtxWorld._41 + mtxWorld._31, mtxWorld._42 + mtxWorld._32, mtxWorld._43 + mtxWorld._33);
	m_vUp = XMFLOAT3(mtxWorld._21, mtxWorld._22, mtxWorld._23);
}

// ���_/�����_/����x�N�g�����烏�[���h �}�g���b�N�X�Z�o
XMFLOAT4X4& CCamera::CalcWorldMatrix()
{
	XMVECTOR vecZ = XMVectorSet(m_vTarget.x - m_vPos.x, m_vTarget.y - m_vPos.y, m_vTarget.z - m_vPos.z, 0.0f);
	XMFLOAT3 vZ;
	XMStoreFloat3(&vZ, XMVector3Normalize(vecZ));
	XMVECTOR vecY = XMLoadFloat3(&m_vUp);
	XMVECTOR vecX = XMVector3Normalize(XMVector3Cross(vecY, vecZ));
	XMFLOAT3 vX;
	XMStoreFloat3(&vX, vecX);
	vecY = XMVector3Normalize(XMVector3Cross(vecZ, vecX));
	XMFLOAT3 vY;
	XMStoreFloat3(&vY, vecY);

	m_mtxWorld._11 = vX.x;
	m_mtxWorld._12 = vX.y;
	m_mtxWorld._13 = vX.z;
	m_mtxWorld._14 = 0.0f;
	m_mtxWorld._21 = vY.x;
	m_mtxWorld._22 = vY.y;
	m_mtxWorld._23 = vY.z;
	m_mtxWorld._24 = 0.0f;
	m_mtxWorld._31 = vZ.x;
	m_mtxWorld._32 = vZ.y;
	m_mtxWorld._33 = vZ.z;
	m_mtxWorld._34 = 0.0f;
	m_mtxWorld._41 = m_vPos.x;
	m_mtxWorld._42 = m_vPos.y;
	m_mtxWorld._43 = m_vPos.z;
	m_mtxWorld._44 = 1.0f;

	return m_mtxWorld;
}

// �v���C���[���Z�b�g
void CCamera::SetPlayer(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
	m_fRadius = (pPlayer) ? pPlayer->GetRadius() : 0.5f;
}

// ������(0:��\��, -1:����, 1:�\��)
int CCamera::CollisionViewFrustum(XMFLOAT3 vCenter,
	float fRadius)
{
	bool bHit = false;
	XMVECTOR frusw, center, dot;
	float fDot;
	center = XMLoadFloat3(&vCenter);
	for (int i = 0; i < 6; ++i) {
		frusw = XMLoadFloat4(&m_frusw[i]);
		dot = XMPlaneDotCoord(frusw, center);
		XMStoreFloat(&fDot, dot);
		if (fDot < -fRadius)
			return 0;	// ���S�ɊO��
		if (fDot <= fRadius)
			bHit = true;
	}
	if (bHit)
		return -1;	// �����I�ɓ���
	return 1;	// ���S�ɓ���
}

// �v���W�F�N�V�����ϊ��p�����[�^���王���䐶��
void CCamera::CreateViewFrustum()
{
	// 0:�� 1:�� 2:�� 3:�E 4:�O 5:���
	float fTan =
		tanf(XMConvertToRadians(m_fFovY) * 0.5f);
	m_frus[0] = XMFLOAT4(0.0f, -1.0f, fTan, 0.0f);
	m_frus[1] = XMFLOAT4(0.0f, 1.0f, fTan, 0.0f);
	fTan *= m_fAspectRatio;
	m_frus[2] = XMFLOAT4(1.0f, 0.0f, fTan, 0.0f);
	m_frus[3] = XMFLOAT4(-1.0f, 0.0f, fTan, 0.0f);
	m_frus[4] = XMFLOAT4(0.0f, 0.0f, 1.0f, -m_fNearZ);
	m_frus[5] = XMFLOAT4(0.0f, 0.0f, -1.0f, m_fFarZ);
	// 0�`3�𐳋K��
	for (int i = 0; i < 4; ++i) {
		XMStoreFloat4(&m_frus[i],
			XMPlaneNormalize(
				XMLoadFloat4(&m_frus[i])));
	}
}

// �J�����̃��[���h�}�g���b�N�X���王������ړ�
void CCamera::UpdateViewFrustum()
{
	// CalcWorldMatrix��m_mtxWorld���X�V�ς�O��
	XMMATRIX mW = XMLoadFloat4x4(&m_mtxWorld);
	mW = XMMatrixInverse(nullptr, mW);//�t�s��
	mW = XMMatrixTranspose(mW);//�]�u�s��
	// ���t�s��̓]�u�s������߂�̂�
	//   XMPlaneTransform�̎d�l!!
	for (int i = 0; i < 6; ++i) {
		XMStoreFloat4(&m_frusw[i],
			XMPlaneTransform(
				XMLoadFloat4(&m_frus[i]), mW));
	}
}

// ����p�ݒ�
void CCamera::SetFovY(float fFovY)
{
	m_fFovY = fFovY;
	CreateViewFrustum();
}

// �A�X�y�N�g��ݒ�
void CCamera::SetAspectRatio(float fAspect)
{
	m_fAspectRatio = fAspect;
	CreateViewFrustum();
}

// �O��N���b�v�����ݒ�
void CCamera::SetRangeZ(float fNearZ, float fFarZ)
{
	m_fNearZ = fNearZ, m_fFarZ = fFarZ;
	CreateViewFrustum();
}
