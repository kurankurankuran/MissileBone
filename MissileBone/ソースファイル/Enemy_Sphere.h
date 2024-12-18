#pragma once
#include "Enemy.h"

typedef enum {
	PLAYER_COL = 0,
	MISSILE_COL,

	MAX_COL
}CollisionTarget;

class CEnemy_Sphere : public CEnemy
{
private:
	int m_nColTarget;

public:
	CEnemy_Sphere(CScene* pScene,CollisionTarget ColTarget);		// �R���X�g���N�^
	virtual ~CEnemy_Sphere();			// �f�X�g���N�^

	virtual HRESULT Init();		// ������
	virtual void Fin();			// �I������
	virtual void Update();		// �X�V
	virtual void Draw();		// �`��

};
