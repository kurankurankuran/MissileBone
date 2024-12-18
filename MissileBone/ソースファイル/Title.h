//=============================================================================
//
// タイトル画面クラス定義 [Title.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once
#include "Scene.h"
#include "Sound.h"
//#include "EffekseerManager.h"
typedef enum {
	STAGESELECT_MENUE = 0,
	OPTION_MENUE,
	//EXIT_MENUE,

	MAX_MENUE
}Title_Menue;

typedef enum {
	SOUND_OPTION = 0,
	EXIT_OPTION,
	
	MAX_OPTION
}Option_Menue;





// クラス定義
class CTitle : public CScene
{
private:
	bool m_bStart;
	ID3D11ShaderResourceView* m_pTexTitle;
	ID3D11ShaderResourceView* m_pTexStart;
	ID3D11ShaderResourceView* m_pTexBG;
	int m_nTimer;
	bool m_bMenue;

	int m_nSclCnt; // 拡縮タイマー
	bool m_bScaleUp; // 拡大中フラグ
	XMFLOAT3 m_vColor; // 枠色

	ID3D11ShaderResourceView* m_pTexMenue[MAX_MENUE]; // ステージ番号文字
	Title_Menue m_nCurMenue;
	/*bool m_bInputUpNow;
	bool m_bInputUpOld;
	bool m_bInputDownNow;
	bool m_bInputDownOld;*/

	bool m_bOption;
	Option_Menue m_nCurOption;
	ID3D11ShaderResourceView* m_pTexOption[MAX_MENUE]; // ステージ番号文字

	bool m_bSound;
	Sound_Menue m_nCurSound;
	ID3D11ShaderResourceView* m_pTexSound[MAX_SOUND]; // ステージ番号文字

	//CEffekseerManager           m_EffekseerManager;
	

public:
	CTitle();
	virtual ~CTitle();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
};
