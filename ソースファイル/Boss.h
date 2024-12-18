// ボス敵機 [Boss.h]
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
	CBoss(CScene* pScene);		// コンストラクタ
	virtual ~CBoss();			// デストラクタ

	virtual HRESULT Init();		// 初期化
	virtual void Fin();			// 終了処理
	virtual void Update();		// 更新
	virtual void Draw();		// 描画

	bool BS(XMFLOAT3, XMFLOAT3, float, float);

	
};
