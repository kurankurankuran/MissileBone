// �G�@ [Enemy_Strate.h]
#pragma once
#include "Enemy.h"
//#include "Effect.h"

class CEnemy_Strate : public CEnemy
{
private:

protected:


public:
	CEnemy_Strate(CScene* pScene);		// �R���X�g���N�^
	virtual ~CEnemy_Strate();			// �f�X�g���N�^

	virtual HRESULT Init();		// ������
	virtual void Fin();			// �I������
	virtual void Update();		// �X�V
	virtual void Draw();		// �`��
};
