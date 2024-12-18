// 地面 [Land.h]
#pragma once
#include "Model.h"

class CLand : public CModel
{
private:
	//CPlayer* m_pPlayer;
	TAssimpVertex* m_pVertex;
	UINT m_nVertex;
	UINT* m_pIndex;
	UINT m_nIndex;

public:
	CLand(CScene* pScene);	// コンストラクタ
	virtual ~CLand();		// デストラクタ

	virtual HRESULT Init();	// 初期化
	virtual void Fin();		// 終了処理
	virtual void Update();  // 更新処理
	virtual void Draw();    // 描画処理

	bool Collision(XMFLOAT3 vP0, XMFLOAT3 vW, XMFLOAT3* pX, XMFLOAT3* pN = nullptr);
	//void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
};
