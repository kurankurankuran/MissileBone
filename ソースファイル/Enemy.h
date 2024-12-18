// �G�@ [Enemy.h]
#pragma once
#include "Model.h"
//#include "Effect.h"

class CPlayer;
class CEnemy : public CModel
{
private:
	
	//CMSManager m_pWeaponManager;
	//CEffect m_Effect;
	int m_nFireCnt;
protected:
	CPlayer* m_pPlayer;
	CGameObj* m_pTarget;

	int m_nEffectCnt;
	
public:
	CEnemy(CScene* pScene);		// �R���X�g���N�^
	virtual ~CEnemy();			// �f�X�g���N�^

	virtual HRESULT Init();		// ������
	virtual void Fin();			// �I������
	virtual void Update();		// �X�V
	virtual void Draw();		// �`��

	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	void SetTarget(CGameObj* pTarget) { m_pTarget = pTarget; }

	bool BS(XMFLOAT3, XMFLOAT3, float, float);

	
};
