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
	CEnemy_Sphere(CScene* pScene,CollisionTarget ColTarget);		// コンストラクタ
	virtual ~CEnemy_Sphere();			// デストラクタ

	virtual HRESULT Init();		// 初期化
	virtual void Fin();			// 終了処理
	virtual void Update();		// 更新
	virtual void Draw();		// 描画

};
