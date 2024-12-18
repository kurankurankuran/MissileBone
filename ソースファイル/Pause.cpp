//=============================================================================
//
// 一時停止処理 [pause.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#include "Pause.h"
#include "Input.h"
#include "fade.h"
#include "Polygon.h"
#include "Texture.h"
#include "Sound.h"
//#include "Scene.h"
#include "Fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
namespace {
	const float PAUSE_MENU_WIDTH = 300.0f;                  // ポーズメニュー幅
	const float PAUSE_MENU_HEIGHT =300.0f;                  // ポーズメニュー高さ
	const float PAUSE_MENU_POS_X = 0.0f;                    // ポーズメニュー位置(X座標)
	const float PAUSE_MENU_POS_Y = 150.0f;                  // ポーズメニュー位置(Y座標)
	const float PAUSE_MENU_INTERVAL = 100.0f;               // ポーズメニュー間隔
	const float PLATE_WIDTH = 360.0f;                       // プレートの幅(横)
	const float PLATE_HEIGHT = 430.0f;                      // プレートの幅(縦)
	const float PLATE_POS_X = 0.0f;                         // プレートの位置(X座標)
	const float PLATE_POS_Y = 0.0f;                         // プレートの位置(Y座標)
}


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
namespace {
	const LPCWSTR g_pszPathTexControl = L"data/texture/Control.png";
	const LPCWSTR g_pszPathTex[PAUSE_MENU_MAX] =
	{
		L"data/texture/Continue.png",
		L"data/texture/Retry.png",
		L"data/texture/SOUND_PAUSE.png",
		L"data/texture/Title.png",
		
	};


	const LPCWSTR g_FileNameSound[MAX_SOUND] =
	{
		L"data/texture/MASTER.png",
		L"data/texture/BGM.png",
		L"data/texture/SE.png",
		L"data/texture/EXIT.png",
	};

	const float g_fVolume_Value = 0.02f; // 音量変化量
	const float g_fMaxVolume = 10.0f; // 最大音量

	const float g_fMaxSizeX_Bar = 500.0f;


	const int SCALE_RATE = 1;             // 拡縮速度
	const float COLOR_RATE = 0.01f;       // 色変化速度
	const float SELECT_INTERVAL = -100.0f; // 枠間の距離間隔
	const float SELECT_SIZE_X = 240.0f;   // 画像の横サイズ
	const float SELECT_SIZE_Y = 200.0f;   // 画像の縦サイズ
	const float FRAME_WIDTH = 20.0f;      // 枠の幅サイズ

	const int MAX_SCALE_UP = 40.0f;       // 膨張時最大
	const int MIN_SCALE_DOWN = -10.0f;    // 縮小時最小

	
}


CPause::CPause()
{
	m_nSelectMenu = PAUSE_MENU_CONTINUE;

	m_bPause = false;

	//m_bInputUpNow = false;
	//m_bInputUpOld = false;
	//m_bInputDownNow = false;
	//m_bInputDownOld = false;

	m_pTextureControl = nullptr;	// コントロールキーテクスチャ

	for (int i = 0; i < PAUSE_MENU_MAX; ++i) {
		m_pTextureMenue[i] = nullptr;	// ポーズメニューテクスチャ
	}
	
}

CPause::~CPause()
{

}


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPause::Init()
{
	m_nSelectMenu = PAUSE_MENU_CONTINUE;

	m_bPause = false;

	/*m_bInputUpNow = false;
	m_bInputUpOld = false;
	m_bInputDownNow = false;
	m_bInputDownOld = false;
*/


	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();


	// テクスチャの読み込み
	hr = CreateTextureFromFile(
		pDevice,					// デバイスへのポインタ
		g_pszPathTexControl,		// ファイルの名前
		&m_pTextureControl);					// 読み込むメモリー
	if (FAILED(hr)) {
		Fin();
		return false;
	}


	for (int i = 0; i < PAUSE_MENU_MAX; i++)
	{
		
		hr = CreateTextureFromFile(
			pDevice,					// デバイスへのポインタ
			g_pszPathTex[i],		// ファイルの名前
			&m_pTextureMenue[i]);					// 読み込むメモリー

		if (FAILED(hr)) {
			Fin();
			return false;
		}
	}


	m_bSound = false;
	for (int i = 0; i < MAX_SOUND; ++i)
	{
		hr = CreateTextureFromFile(pDevice,					// デバイスへのポインタ
			g_FileNameSound[i],		// ファイルの名前
			&m_pTexSound[i]);					// 読み込むメモリー
		if (FAILED(hr))
		{
			return hr;
		}
	}
	m_nCurSound = MASTER_SOUND;

	m_nSclCnt = 0; // 拡縮タイマー
	m_bScaleUp = true; // 拡大中フラグ
	m_vColor = XMFLOAT3(1.0f, 0.0f, 0.0f); // 枠色

	return hr;
}



//=============================================================================
// 終了処理
//=============================================================================
void CPause::Fin()
{
	for (int i = 0; i < MAX_SOUND; ++i)
	{
		SAFE_RELEASE(m_pTexSound[i]);
	}

	SAFE_RELEASE(m_pTextureControl);
	for (int i = 0; i < PAUSE_MENU_MAX; ++i) {
		SAFE_RELEASE(m_pTextureMenue[i]);
	}
}



//=============================================================================
// 更新処理
//=============================================================================

void CPause::Update()
{

	if (CInput::GetKeyRelease(VK_Q)) {
		m_bPause = !m_bPause;
		m_bSound = false;
	}

	if (!m_bPause) return;

	/*m_bInputDownOld = m_bInputDownNow;
	m_bInputUpOld = m_bInputUpNow;*/

	if (!m_bSound) {

		if (CInput::GetKeyTrigger(VK_W) /*|| m_bInputUpNow && !m_bInputUpOld*/)
		{
			CSound::Play(SE_MENUE);
			m_nSelectMenu = (PAUSE_MENU)((m_nSelectMenu + PAUSE_MENU_MAX - 1) % PAUSE_MENU_MAX);
			//SetPauseMenu();
		}
		else if (CInput::GetKeyTrigger(VK_S) /*|| m_bInputDownNow && !m_bInputDownOld*/)
		{
			CSound::Play(SE_MENUE);
			m_nSelectMenu = (PAUSE_MENU)((m_nSelectMenu + 1) % PAUSE_MENU_MAX);
			//SetPauseMenu();
		}


		if (CInput::GetKeyRelease(VK_SPACE)) {
			switch (m_nSelectMenu)
			{
			case PAUSE_MENU_CONTINUE:
				m_bPause = false;
				break;
			case PAUSE_MENU_RETRY:
				CFade::Out(CScene::GetCurrent()->GetID());
				break;
			case PAUSE_MENU_SOUND:
				m_bSound = true;
				break;
			case PAUSE_MENU_QUIT:
				CFade::Out(SCENE_TITLE);
				break;
			default:
				break;
			}
		}
	}
	else
	{
		// サウンド
		/***** 選択されたステージ画像の拡大縮小 *****/
		if (m_bScaleUp)
		{
			m_nSclCnt += SCALE_RATE;
			if (m_nSclCnt >= MAX_SCALE_UP)
			{
				m_bScaleUp = false;
			}
		}
		else
		{
			m_nSclCnt -= SCALE_RATE;
			if (m_nSclCnt <= MIN_SCALE_DOWN)
			{
				m_bScaleUp = true;
			}
		}

		/***** 選択された枠(背景画像)の表示*****/
		if (m_vColor.y <= 0.0f && m_vColor.z <= 1.0f)
		{
			m_vColor.z -= COLOR_RATE;
			m_vColor.x += COLOR_RATE;
			if (m_vColor.z <= 0.0f)
			{
				m_vColor.z = 0.0f;
			}
			if (m_vColor.x >= 1.0f)
			{
				m_vColor.x = 1.0f;
			}
		}
		if (m_vColor.x <= 1.0f && m_vColor.z <= 0.0f)
		{
			m_vColor.x -= COLOR_RATE;
			m_vColor.y += COLOR_RATE;
			if (m_vColor.x <= 0.0f)
			{
				m_vColor.x = 0.0f;
			}
			if (m_vColor.y >= 1.0f)
			{
				m_vColor.y = 1.0f;
			}
		}
		if (m_vColor.x <= 0.0f && m_vColor.y <= 1.0f)
		{
			m_vColor.y -= COLOR_RATE;
			m_vColor.z += COLOR_RATE;
			if (m_vColor.y <= 0.0f)
			{
				m_vColor.y = 0.0f;
			}
			if (m_vColor.z >= 1.0f)
			{
				m_vColor.z = 1.0f;
			}
		}





		if (CInput::GetKeyTrigger(VK_W) /*|| m_bInputUpNow && !m_bInputUpOld*/)
		{
			CSound::Play(SE_MENUE);
			m_nCurSound = (Sound_Menue)((m_nCurSound + MAX_SOUND - 1) % MAX_SOUND);

		}
		else if (CInput::GetKeyTrigger(VK_S) /*|| m_bInputDownNow && !m_bInputDownOld*/)
		{
			CSound::Play(SE_MENUE);
			m_nCurSound = (Sound_Menue)((m_nCurSound + 1) % MAX_SOUND);

		}

		if (m_nCurSound != Exit_SOUND) {
			if (CInput::GetKeyPress(VK_A) /*|| m_bInputUpNow && !m_bInputUpOld*/)
			{
			

				switch (m_nCurSound)
				{
				case MASTER_SOUND:
				{
					float fVolume = CSound::GetVolume() - g_fVolume_Value;
					if (fVolume < 0.0f) {
						fVolume = 0.0f;
					}
					/*else if (g_fMaxVolume < fVolume) {
						fVolume = g_fMaxVolume;
					}*/
					CSound::SetVolume(fVolume);
				}
				break;
				case BGM_SOUND:
				{
					float fVolume = CSound::GetVolume(BGM_TITLE) - g_fVolume_Value;
					if (fVolume < 0.0f) {
						fVolume = 0.0f;
					}
					/*else if (g_fMaxVolume < fVolume) {
						fVolume = g_fMaxVolume;
					}*/
					CSound::SetVolumeBGM(fVolume);
				}
				break;
				case SE_SOUND:
				{
					float fVolume = CSound::GetVolume(SE_MENUE) - g_fVolume_Value;
					if (fVolume < 0.0f) {
						fVolume = 0.0f;
					}
					/*else if (g_fMaxVolume < fVolume) {
						fVolume = g_fMaxVolume;
					}*/
					CSound::SetVolumeSE(fVolume);
				}
				break;
				default:
					break;
				}
				/*for (eBGM i = 0; i < MAX_BGM;++i) {
					CSound::SetVolume(i, m_fVolume[m_nCurSound]);
				}*/

			}
			if (CInput::GetKeyPress(VK_D) /*|| m_bInputDownNow && !m_bInputDownOld*/)
			{
				switch (m_nCurSound)
				{
				case MASTER_SOUND:
				{
					float fVolume = CSound::GetVolume() + g_fVolume_Value;
					/*if (fVolume < 0.0f) {
						fVolume = 0.0f;
					}
					else*/ if (g_fMaxVolume < fVolume) {
						fVolume = g_fMaxVolume;
					}
					CSound::SetVolume(fVolume);
				}
				break;
				case BGM_SOUND:
				{
					float fVolume = CSound::GetVolume(BGM_TITLE) + g_fVolume_Value;
					/*if (fVolume < 0.0f) {
						fVolume = 0.0f;
					}
					else*/ if (g_fMaxVolume < fVolume) {
						fVolume = g_fMaxVolume;
					}
					CSound::SetVolumeBGM(fVolume);
				}
				break;
				case SE_SOUND:
				{
					float fVolume = CSound::GetVolume(SE_MENUE) + g_fVolume_Value;
					/*if (fVolume < 0.0f) {
						fVolume = 0.0f;
					}
					else*/ if (g_fMaxVolume < fVolume) {
						fVolume = g_fMaxVolume;
					}
					CSound::SetVolumeSE(fVolume);
				}
				break;
				default:
					break;
				}
			}
		}
		else
		{
			if (CInput::GetKeyRelease(VK_SPACE)) {
				m_bSound = false;
			}
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================

void CPause::Draw()
{
	if (!m_bPause)return;

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);								// Zバッファ更新しない

	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetUV(0.0f, 0.0f);
	if (!m_bSound)
	{
		// プレート
		CPolygon::SetTexture(nullptr);
		CPolygon::SetSize(PLATE_WIDTH, PLATE_HEIGHT);
		CPolygon::SetPos(PLATE_POS_X - 300.0f, PLATE_POS_Y);
		CPolygon::SetColor(1.0f, 0.2f, 0.2f/* g_fCol, g_fCol, g_fCol */);
		CPolygon::SetAlpha(0.4f);
		CPolygon::Draw(pDeviceContext);

		// キー説明
		CPolygon::SetSize(PLATE_WIDTH, PLATE_HEIGHT);
		CPolygon::SetPos(PLATE_POS_X - 300.0f, PLATE_POS_Y);
		CPolygon::SetFrameSize(1.0f, 1.0f);
		CPolygon::SetUV(0.0f, 0.0f);
		CPolygon::SetColor(1.0f, 1.0f, 1.0f);
		CPolygon::SetAlpha(1.0f);
		CPolygon::SetTexture(m_pTextureControl);
		CPolygon::Draw(pDeviceContext);


		//**************************************************************************


		// 右プレート
		CPolygon::SetTexture(nullptr);
		CPolygon::SetSize(PLATE_WIDTH, PLATE_HEIGHT);
		CPolygon::SetPos(PLATE_POS_X + 300, PLATE_POS_Y);
		CPolygon::SetFrameSize(1.0f, 1.0f);
		CPolygon::SetUV(0.0f, 0.0f);
		CPolygon::SetColor(1.0f, 0.2f, 0.2f/* g_fCol, g_fCol, g_fCol */);
		CPolygon::SetAlpha(0.4f);
		CPolygon::Draw(pDeviceContext);


		// 選択メニュー
		CPolygon::SetSize(PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT);
		for (int i = 0; i < PAUSE_MENU_MAX; i++)
		{
			CPolygon::SetPos(PAUSE_MENU_POS_X + 300, PAUSE_MENU_POS_Y - i * PAUSE_MENU_INTERVAL);
			if (i == m_nSelectMenu)
			{
				// 選択されている
				CPolygon::SetColor(1.0f, 1.0f, 1.0f);
			}
			else
			{
				// 選択されていない
				CPolygon::SetColor(0.8f, 0.8f, 0.8f);
			}
			CPolygon::SetAlpha(1.0f);

			// テクスチャの設定
			CPolygon::SetTexture(m_pTextureMenue[i]);

			// ポリゴンの描画
			CPolygon::Draw(pDeviceContext);
		}
	}
	else
	{
		// プレート
		CPolygon::SetTexture(nullptr);
		CPolygon::SetPos(0.0f, -150.0f);
		CPolygon::SetSize(900.0f, 400.0f);
		CPolygon::SetColor(0.5f, 0.3f, 0.1f);
		CPolygon::SetAlpha(0.4f);
		CPolygon::Draw(pDeviceContext);


		// サウンドメニュー
		for (int i = 0; i < MAX_SOUND; ++i)
		{
			int fPosY = -10.0f;
			fPosY -= i * SELECT_SIZE_Y;
			fPosY -= i * SELECT_INTERVAL;
			//fPosY -= (SELECT_SIZE_Y - SELECT_INTERVAL) / 2.0f;
			if (i == m_nCurSound) {

				// 枠
				CPolygon::SetTexture(nullptr);
				CPolygon::SetPos(-300.0f, fPosY + 6.0f);
				CPolygon::SetSize(200.0f + m_nSclCnt, 50.0f + m_nSclCnt);
				CPolygon::SetColor(m_vColor.x, m_vColor.y, m_vColor.z);
				CPolygon::SetAlpha(1.0f);
				CPolygon::Draw(pDeviceContext);

				// メニュー画像
				CPolygon::SetTexture(m_pTexSound[i]);
				CPolygon::SetPos(-300.0f, fPosY);
				CPolygon::SetSize(SELECT_SIZE_X, SELECT_SIZE_Y);
				CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
				CPolygon::Draw(pDeviceContext);
			}
			else
			{
				// メニュー画像
				CPolygon::SetTexture(m_pTexSound[i]);
				CPolygon::SetPos(-300.0f, fPosY);
				CPolygon::SetSize(SELECT_SIZE_X, SELECT_SIZE_Y);
				CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
				CPolygon::SetAlpha(1.0f);
				CPolygon::Draw(pDeviceContext);
			}


			// バー
			switch (i)
			{
			case MASTER_SOUND:
			{
				float fSizeRateX = g_fMaxSizeX_Bar / g_fMaxVolume;
				float fSizeX = CSound::GetVolume() * fSizeRateX;

				CPolygon::SetTexture(nullptr);
				CPolygon::SetPos(-150.0f + fSizeX / 2.0f, fPosY + 10.0f);
				CPolygon::SetSize(fSizeX, 50.0f);
				CPolygon::SetColor(0.0f, 1.0f, 1.0f);
				CPolygon::SetAlpha(1.0f);
				CPolygon::Draw(pDeviceContext);
			}
			break;
			case BGM_SOUND:
			{
				float fSizeRateX = g_fMaxSizeX_Bar / g_fMaxVolume;
				float fSizeX = CSound::GetVolume(BGM_TITLE) * fSizeRateX;
				//                                 ↑取り出すボリュームはBGMならどれでもよい
				CPolygon::SetTexture(nullptr);
				CPolygon::SetPos(-150.0f + fSizeX / 2.0f, fPosY + 10.0f);
				CPolygon::SetSize(fSizeX, 50.0f);
				CPolygon::SetColor(0.0f, 1.0f, 1.0f);
				CPolygon::SetAlpha(1.0f);
				CPolygon::Draw(pDeviceContext);
			}
			break;
			case SE_SOUND:
			{
				float fSizeRateX = g_fMaxSizeX_Bar / g_fMaxVolume;
				float fSizeX = CSound::GetVolume(SE_MENUE) * fSizeRateX;
				//                                 ↑取り出すボリュームはSEならどれでもよい
				CPolygon::SetTexture(nullptr);
				CPolygon::SetPos(-150.0f + fSizeX / 2.0f, fPosY + 10.0f);
				CPolygon::SetSize(fSizeX, 50.0f);
				CPolygon::SetColor(0.0f, 1.0f, 1.0f);
				CPolygon::SetAlpha(1.0f);
				CPolygon::Draw(pDeviceContext);
			}
			break;
			}
		}
	}

	CPolygon::SetColor(0.0f, 0.0f, 0.0f);
	SetZWrite(true);							// Zバッファ更新する
	SetBlendState(BS_NONE);		// アルファブレンド無効
}


////=============================================================================
//// ポーズメニューの設定
////=============================================================================
//void SetPauseMenu(void)
//{
//	g_fCurve = 0.0f;
//}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
PAUSE_MENU CPause::GetPauseMenu(void)
{
	return m_nSelectMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void CPause::ResetPauseMenu(void)
{
	m_nSelectMenu = PAUSE_MENU_CONTINUE;
	CSound::Play(SE_MENUE);
	//SetPauseMenu();
}
