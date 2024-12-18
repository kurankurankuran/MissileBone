// 敵機 [Enemy_Strate.cpp]
#include "Enemy_Strate.h"



// コンストラクタ
CEnemy_Strate::CEnemy_Strate(CScene* pScene) : CEnemy(pScene)
{
}

// デストラクタ
CEnemy_Strate::~CEnemy_Strate()
{
}

// 初期化
HRESULT CEnemy_Strate::Init()
{
	
	HRESULT hr = CEnemy::Init();
	SetModel(MODEL_ENEMY_STRATE);
	
	return hr;
}

// 終了処理
void CEnemy_Strate::Fin()
{
	//m_Effect.Fin();
	CModel::Fin();
}

// 更新
void CEnemy_Strate::Update()
{
	CEnemy::Update();
}


void CEnemy_Strate::Draw()
{
	CEnemy::Draw();
}

