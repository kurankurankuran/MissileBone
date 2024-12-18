// �n�� [Land.h]
#pragma once
#include "Model.h"

class CLand : public CModel
{
private:
	//CPlayer* m_pPlayer;
	TAssimpVertex* m_pVertex;
	UINT m_nVertex;
	UINT* m_pIndex;
	UINT m_nIndex;

public:
	CLand(CScene* pScene);	// �R���X�g���N�^
	virtual ~CLand();		// �f�X�g���N�^

	virtual HRESULT Init();	// ������
	virtual void Fin();		// �I������
	virtual void Update();  // �X�V����
	virtual void Draw();    // �`�揈��

	bool Collision(XMFLOAT3 vP0, XMFLOAT3 vW, XMFLOAT3* pX, XMFLOAT3* pN = nullptr);
	//void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
};
