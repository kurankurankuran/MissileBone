//=============================================================================
//
// ゲームオーバー画面クラス定義 [GameOver.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once
#include "Scene.h"

// クラス定義
class CGameOver : public CScene
{
private:
	bool m_bStart;
	ID3D11ShaderResourceView* m_pTexGameOver;
	ID3D11ShaderResourceView* m_pTexStart;
	ID3D11ShaderResourceView* m_pTexBG;
	int m_nTimer;

public:
	CGameOver();
	virtual ~CGameOver();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
};
