// �G�@ [Enemy_Strate.cpp]
#include "Enemy_Strate.h"



// �R���X�g���N�^
CEnemy_Strate::CEnemy_Strate(CScene* pScene) : CEnemy(pScene)
{
}

// �f�X�g���N�^
CEnemy_Strate::~CEnemy_Strate()
{
}

// ������
HRESULT CEnemy_Strate::Init()
{
	
	HRESULT hr = CEnemy::Init();
	SetModel(MODEL_ENEMY_STRATE);
	
	return hr;
}

// �I������
void CEnemy_Strate::Fin()
{
	//m_Effect.Fin();
	CModel::Fin();
}

// �X�V
void CEnemy_Strate::Update()
{
	CEnemy::Update();
}


void CEnemy_Strate::Draw()
{
	CEnemy::Draw();
}

