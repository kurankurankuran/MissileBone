// �����G�t�F�N�g �N���X [Explosion.h]
#pragma once
#include "GameObj.h"
#include "AssimpModel.h"

class CExplosion;

// �j��
class CDebris
{
private:
	CExplosion* m_pParent;	// �����G�t�F�N�g
	CDebris* m_pBack;		// �O�̔j��
	CDebris* m_pNext;		// ���̔j��

	ID3D11Buffer* m_pVertexBuffer;	// ���_�o�b�t�@
	XMFLOAT4X4 m_world;		// ���[���h�ϊ�
	XMFLOAT3 m_velocity;	// ���x
	XMFLOAT3 m_accel;		// �����x
	XMFLOAT3 m_axis;		// ��]��
	float m_rotate;			// ��]���x
	float m_alpha;			// �s�����x

public:
	CDebris(CExplosion* pExp);	// �R���X�g���N�^
	~CDebris();					// �f�X�g���N�^

	CDebris* GetTop();			// ���X�g�擪���擾
	void SetTop(CDebris* pTop);	// ���X�g�擪��ݒ�
	CDebris* GetNext() { return m_pNext; }

	HRESULT Init(XMFLOAT3& v0, XMFLOAT3& v1, XMFLOAT3& v2,
		float fRadius, XMFLOAT4X4* pWorld, XMFLOAT3* pVelocity);	// ������
	void Uninit();	// �I������
	void Update();	// �X�V
	void Draw();	// �`��
};

// �����G�t�F�N�g
class CExplosion //: public CGameObj
{
private:
	CDebris* m_pDebris;	// �j��
	static ID3D11Buffer* m_pConstantBuffer[2];		// �萔�o�b�t�@
	static ID3D11VertexShader* m_pVertexShader;		// ���_�V�F�[�_
	static ID3D11InputLayout* m_pInputLayout;		// ���_�t�H�[�}�b�g
	static ID3D11PixelShader* m_pPixelShader;		// �s�N�Z���V�F�[�_

	TAssimpVertex* m_pVertex;	// ���_�z��
	UINT m_uVertex;				// ���_��
	UINT* m_pIndex;				// �C���f�b�N�X�z��
	UINT m_uIndex;				// �C���f�b�N�X��

	float m_fRadius;
	CScene* m_pScene;

public:
	CExplosion(CScene* pScene);
	~CExplosion();

	CDebris* GetTop() { return m_pDebris; }
	void SetTop(CDebris* pTop) { m_pDebris = pTop; }

	static HRESULT InitShader();
	static void FinShader();

	HRESULT Init();		// ������
	void Fin();			// �I������
	void Update();		// �X�V
	void Draw();		// �`��
	void DrawAlpha();	// �������`��

	// ���f���ݒ�
	HRESULT SetModel(CAssimpModel* pModel);

	// �����J�n
	HRESULT Start(XMFLOAT4X4* pWorld, XMFLOAT3* pVelocity = nullptr);

	// �V�F�[�_�̒萔�o�b�t�@���X�V
	void UpdateBuffer(int nShader, void* pData);

	// �V�[���擾
	CScene* GetScene() { return m_pScene; }

	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	float GetRadius() { return m_fRadius; }

	//int GetDebrisSize();

	void FinDeb();
};
