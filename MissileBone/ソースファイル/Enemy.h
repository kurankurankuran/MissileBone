// 敵機 [Enemy.h]
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
	CEnemy(CScene* pScene);		// コンストラクタ
	virtual ~CEnemy();			// デストラクタ

	virtual HRESULT Init();		// 初期化
	virtual void Fin();			// 終了処理
	virtual void Update();		// 更新
	virtual void Draw();		// 描画

	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	void SetTarget(CGameObj* pTarget) { m_pTarget = pTarget; }

	bool BS(XMFLOAT3, XMFLOAT3, float, float);

	
};
