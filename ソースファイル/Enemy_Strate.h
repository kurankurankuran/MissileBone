// 敵機 [Enemy_Strate.h]
#pragma once
#include "Enemy.h"
//#include "Effect.h"

class CEnemy_Strate : public CEnemy
{
private:

protected:


public:
	CEnemy_Strate(CScene* pScene);		// コンストラクタ
	virtual ~CEnemy_Strate();			// デストラクタ

	virtual HRESULT Init();		// 初期化
	virtual void Fin();			// 終了処理
	virtual void Update();		// 更新
	virtual void Draw();		// 描画
};
