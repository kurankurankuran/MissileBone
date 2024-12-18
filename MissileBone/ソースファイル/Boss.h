// �{�X�G�@ [Boss.h]
#pragma once
#include "Model.h"
#include "Enemy.h"
#include "WeaponManager.h"
//#include "Effect.h"
//enum EEnemyParts {
//	PARTS_BODY = 0,
//	PARTS_CANON,
//	/*PARTS_MISSILE_1,
//	PARTS_MISSILE_2,
//	PARTS_MISSILE_3,
//	PARTS_MISSILE_4,
//	PARTS_MISSILE_5,
//	PARTS_MISSILE_6,*/
//
//
//	MAX_ENEMY_PARTS
//};

class CPlayer;
class CBoss : public CEnemy
{
private:
	
	vector<TParts> m_WeaponParts;
	CWeaponManager* m_pWeaponManager;

	int m_nFireCnt;

	//CEffect m_Effect;
public:
	CBoss(CScene* pScene);		// �R���X�g���N�^
	virtual ~CBoss();			// �f�X�g���N�^

	virtual HRESULT Init();		// ������
	virtual void Fin();			// �I������
	virtual void Update();		// �X�V
	virtual void Draw();		// �`��

	bool BS(XMFLOAT3, XMFLOAT3, float, float);

	
};
