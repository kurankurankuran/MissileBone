//=============================================================================
//
// 一時停止処理 [pause.h]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#pragma once
//#include "Scene.h"

#include "main.h"
#include "Sound.h"

// ポーズメニュー
typedef enum 
{
	PAUSE_MENU_CONTINUE = 0,		// コンティニュー
	PAUSE_MENU_RETRY,					// リトライ
	PAUSE_MENU_SOUND,
	PAUSE_MENU_QUIT,						// クイット

	PAUSE_MENU_MAX
} PAUSE_MENU;

//const int NUM_PAUSE_MENU = 3;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//enum Sound_Menue;
class CPause
{
private:
	PAUSE_MENU m_nSelectMenu;	// 選択メニュー

	bool m_bPause;              // ポーズフラグ

	/*bool m_bInputUpNow;
	bool m_bInputUpOld;
	bool m_bInputDownNow;
	bool m_bInputDownOld;*/

	ID3D11ShaderResourceView*	m_pTextureControl;	// コントロールキーテクスチャ
	ID3D11ShaderResourceView*	m_pTextureMenue[PAUSE_MENU_MAX];	// ポーズメニューテクスチャ




	int m_nSclCnt; // 拡縮タイマー
	bool m_bScaleUp; // 拡大中フラグ
	XMFLOAT3 m_vColor; // 枠色
	bool m_bSound;
	Sound_Menue m_nCurSound;
	ID3D11ShaderResourceView* m_pTexSound[MAX_SOUND]; // ステージ番号文字
public:
	CPause();
	~CPause();

	HRESULT Init();
	void Fin();
	void Update();
	void Draw();

	//void SetPauseMenu(void);
	PAUSE_MENU GetPauseMenu(void);
	void ResetPauseMenu(void);

	bool GetPauseFlg() { return m_bPause; }
};