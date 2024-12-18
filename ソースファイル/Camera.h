#pragma once
#include "main.h"

#define	VIEW_ANGLE			(45.0f)										// ����p
#define	VIEW_ASPECT			((float)SCREEN_WIDTH/(float)SCREEN_HEIGHT)	// �A�X�y�N�g��
#define	VIEW_NEAR_Z			(1.0f)										// NearZ�l
#define	VIEW_FAR_Z			(50000.0f)									// FarZ�l

enum ECameraMode {
	STATIC_CAMERAMODE = 0,// ��_�J����
	FPS_CAMERAMODE,// ��l�̃J����
	BULLET_CAMERAMODO,// �o���b�g�^�C�����J����
	TPS_CAMERAMODE,// �O�l�̃J����
	TPS_BUCK_CAMERAMODE,// �O�l�̔w��J����
	BULLET_FREE_CAMERAMODE,// �o���b�g�^�C�����J����(���R�ӏ�)

	MAX_CAMERAMODE
};


class CAssimpModel;
class CPlayer;
class CGameObj;
class CCamera
{
private:
	DirectX::XMFLOAT3 m_vPos;		// ���_
	DirectX::XMFLOAT3 m_vTarget;	// �����_
	DirectX::XMFLOAT3 m_vUp;		// ����x�N�g��

	float m_fAspectRatio;			// �c����
	float m_fFovY;					// ����p(Degree)
	float m_fNearZ;					// �O���N���b�v����
	float m_fFarZ;					// ����N���b�v����

	DirectX::XMFLOAT4X4 m_mtxWorld;	// ���[���h �}�g���b�N�X
	DirectX::XMFLOAT4X4 m_mtxView;	// �r���[ �}�g���b�N�X
	DirectX::XMFLOAT4X4 m_mtxProj;	// �v���W�F�N�V���� �}�g���b�N�X

	//DirectX::XMFLOAT3 m_vAngle;		// �J�����̊p�x
	//float m_fLengthInterval;		// �J�����̎��_�ƒ����_�̋���

	CAssimpModel* m_pSky;			// �X�J�C �h�[��

	float m_fRadius;				// ���a

	CPlayer* m_pPlayer;				// ���@
	int m_nMode;					// �J�������[�h
	float m_fAngle;					// �J�����A���O��

	DirectX::XMFLOAT4 m_frus[6];	// ������
	DirectX::XMFLOAT4 m_frusw[6];	// ������(���[���h���)

	DirectX::XMFLOAT3 m_fRotValue;

	
	DirectX::XMFLOAT2 m_fFreeBulletMode;
	//CGameObj* m_pTargetObj;
public:
	CCamera();

	virtual void Init();
	virtual void Update();
	virtual void Clear();

	void SetPos(DirectX::XMFLOAT3& vPos) { m_vPos = vPos; }
	DirectX::XMFLOAT3& GetPos() { return m_vPos; }
	void SetPos(float x, float y, float z) { m_vPos.x = x, m_vPos.y = y, m_vPos.z = z; }
	void SetTarget(DirectX::XMFLOAT3& vTarget) { m_vTarget = vTarget; }
	void SetTarget(float x, float y, float z) { m_vTarget.x = x, m_vTarget.y = y, m_vTarget.z = z; }
	XMFLOAT3 GetTargetPos() { return m_vTarget; }
	void SetUpVector(DirectX::XMFLOAT3& vUp) { m_vUp = vUp; }
	void SetUpVector(float x, float y, float z) { m_vUp.x = x, m_vUp.y = y, m_vUp.z = z; }
	XMFLOAT3 GetUpVector() { return m_vUp; }
	void SetWorldMatrix(DirectX::XMFLOAT4X4& mtxWorld);
	DirectX::XMFLOAT4X4& GetWorldMatrix() { return m_mtxWorld; }
	DirectX::XMFLOAT4X4& CalcWorldMatrix();
	void SetFovY(float fFovY);
	void SetAspectRatio(float fAspect);
	void SetRangeZ(float fNearZ, float fFarZ);
	void UpdateMatrix();
	DirectX::XMFLOAT4X4& GetViewMatrix() { return m_mtxView; }
	DirectX::XMFLOAT4X4& GetProjMatrix() { return m_mtxProj; }
	//DirectX::XMFLOAT3& GetAngle() { return m_vAngle; }
	void SetSkyDome(CAssimpModel* pSky) { m_pSky = pSky; }
	CAssimpModel* GetSkyDome() { return m_pSky; }
	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	void SetPlayer(CPlayer* pPlayer);
	int CollisionViewFrustum(XMFLOAT3 vCenter,float fRadius);//������(0:��\��,-1:����, 1:�\��)
	void SetMode(int nMode) { m_nMode = nMode; }
	int GetMode() { return m_nMode; }
	//void SetTargetObj(CGameObj* pObj) { m_pTargetObj = pObj; }
	//CGameObj* GetTargetObj() { return m_pTargetObj; }
	void SetFreeBulletMode(XMFLOAT2 fFreeBulletMode) { m_fFreeBulletMode = fFreeBulletMode; } // �t���[�o���b�g���[�h���̔��a�ƃJ�����̍�����ݒ�
private:
	void CreateViewFrustum();
	void UpdateViewFrustum();
};
