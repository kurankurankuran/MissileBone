//=============================================================================
//
// タイトル画面クラス実装 [Title.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include "Title.h"
#include "Texture.h"
#include "Sound.h"
#include "Input.h"
#include "Polygon.h"
#include "Fade.h"



// 定数定義
namespace {
	const LPCWSTR g_pszPathTexTitle = L"data/texture/TitleName.png";
	const float g_fTexTitleWidth = 1120.0f;
	const float g_fTexTitleHeight = 960.0f;

	const LPCWSTR g_pszPathTexStart = L"data/texture/PressSpaceKey.png";
	const float g_fTexStartWidth = 960.0f;
	const float g_fTexStartHeight = 640.0f;
	const float g_fTexStartY = SCREEN_HEIGHT / -3.0f;

	const LPCWSTR g_pszPathTexBG = L"data/texture/BG00a0_80.jpg";

	const LPCWSTR g_FileNameMenue[MAX_MENUE] =
	{
		L"data/texture/STAGE_SELECT.png",
		L"data/texture/OPTION.png",		
		//L"data/texture/EXIT.png",		
		
	};


	const int SCALE_RATE = 1;             // 拡縮速度
	const float COLOR_RATE = 0.01f;       // 色変化速度
	const float SELECT_INTERVAL = -100.0f; // 枠間の距離間隔
	const float SELECT_SIZE_X = 240.0f;   // 画像の横サイズ
	const float SELECT_SIZE_Y = 200.0f;   // 画像の縦サイズ
	const float FRAME_WIDTH = 20.0f;      // 枠の幅サイズ

	const int MAX_SCALE_UP = 40.0f;       // 膨張時最大
	const int MIN_SCALE_DOWN = -10.0f;    // 縮小時最小

	const LPCWSTR g_FileNameOption[MAX_OPTION] =
	{
		L"data/texture/SOUND.png",
		L"data/texture/EXIT.png",
		//L"data/texture/EXIT.png",		

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

	
}

// コンストラクタ
CTitle::CTitle() : CScene()
{
	m_id = SCENE_TITLE;

	m_bStart = false;
	m_pTexTitle = nullptr;
	m_pTexStart = nullptr;
	m_pTexBG = nullptr;
	m_nTimer = 0;
	m_nCurMenue = STAGESELECT_MENUE;
	m_nCurOption = SOUND_OPTION;
	m_nCurSound = MASTER_SOUND;
	
	//m_bInputUpNow = false;
	//m_bInputUpOld = false;
	//m_bInputDownNow = false;
	//m_bInputDownOld = false;
	//CSound::SetVolume(1.0f);
}

// デストラクタ
CTitle::~CTitle()
{
}

// 初期化
bool CTitle::Init()
{
	
	if (!CScene::Init()) {
		return false;
	}

	// エフェクシア初期化
	//m_EffekseerManager.Init();

	m_nSclCnt = 0; // 拡縮タイマー
	m_bScaleUp = true; // 拡大中フラグ
	m_vColor = XMFLOAT3(1.0f, 0.0f, 0.0f); // 枠色

	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	m_bStart = false;
	// テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice, g_pszPathTexTitle, &m_pTexTitle);
	if (FAILED(hr)) {
		return false;
	}
	hr = CreateTextureFromFile(pDevice, g_pszPathTexStart, &m_pTexStart);
	if (FAILED(hr)) {
		Fin();
		return false;
	}
	hr = CreateTextureFromFile(pDevice, g_pszPathTexBG, &m_pTexBG);
	if (FAILED(hr)) {
		Fin();
		return false;
	}

	m_bMenue = false;

	// メニュー選択のテクスチャ読み込み
	for (int i = 0; i < MAX_MENUE; ++i)
	{
		hr = CreateTextureFromFile(pDevice,					// デバイスへのポインタ
			g_FileNameMenue[i],		// ファイルの名前
			&m_pTexMenue[i]);					// 読み込むメモリー
		if (FAILED(hr))
		{
			return hr;
		}
	}
	m_nTimer = 0;

	m_bOption = false;
	for (int i = 0; i < MAX_OPTION; ++i)
	{
		hr = CreateTextureFromFile(pDevice,					// デバイスへのポインタ
			g_FileNameOption[i],		// ファイルの名前
			&m_pTexOption[i]);					// 読み込むメモリー
		if (FAILED(hr))
		{
			return hr;
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
	//m_fVolume[MASTER_SOUND] = CSound::GetVolume(); // サウンド音量
	//m_fVolume[BGM_SOUND] = CSound::GetVolume(); // サウンド音量
	//m_fVolume[SE_SOUND] = CSound::GetVolume(); // サウンド音量



	// BGM再生開始
	CSound::Play(BGM_TITLE);

	return true;
}

// 終了処理
void CTitle::Fin()
{
	// BGM再生停止
	CSound::Stop(BGM_TITLE);

	for (int i = 0; i < MAX_SOUND; ++i)
	{
		SAFE_RELEASE(m_pTexSound[i]);
	}

	for (int i = 0; i < MAX_OPTION; ++i)
	{
		SAFE_RELEASE(m_pTexOption[i]);
	}


	for (int i = 0; i < MAX_MENUE; ++i)
	{
		SAFE_RELEASE(m_pTexMenue[i]);
	}

	SAFE_RELEASE(m_pTexBG);
	SAFE_RELEASE(m_pTexStart);
	SAFE_RELEASE(m_pTexTitle);

	// エフェクシア終了
	//m_EffekseerManager.Uninit();

	CScene::Fin();
}

// 更新
void CTitle::Update()
{



	if (!m_bMenue)
	{
		if (CInput::GetKeyRelease(VK_SPACE) ||
			CInput::GetKeyRelease(VK_RETURN)) {
			CSound::Play(SE_DECID);
			m_bMenue = true;
		}

		++m_nTimer;
		if (m_nTimer > 120) {
			m_nTimer = 0;
		}
		/*if (!m_bStart) {
		if (CInput::GetKeyRelease(VK_SPACE) ||
			CInput::GetKeyRelease(VK_RETURN)) {
			m_bStart = true;
			CFade::Out(SCENE_SELECT);
		}
	}*/
	}
	else
	{
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


		// メニュー選択
	/*	m_bInputDownOld = m_bInputDownNow;
		m_bInputUpOld = m_bInputUpNow;*/


		if (!m_bOption) {
			if (CFade::Get() != FADE_NONE) return;
			if (CInput::GetKeyTrigger(VK_W) /*|| m_bInputUpNow && !m_bInputUpOld*/)
			{
				CSound::Play(SE_MENUE);
				m_nCurMenue = (Title_Menue)((m_nCurMenue + MAX_MENUE - 1) % MAX_MENUE);

			}
			else if (CInput::GetKeyTrigger(VK_S) /*|| m_bInputDownNow && !m_bInputDownOld*/)
			{
				CSound::Play(SE_MENUE);
				m_nCurMenue = (Title_Menue)((m_nCurMenue + 1) % MAX_MENUE);

			}


			if (CInput::GetKeyRelease(VK_SPACE)) {
				CSound::Play(SE_DECID);
				switch (m_nCurMenue)
				{
				case STAGESELECT_MENUE:
					
					CFade::Out(SCENE_SELECT);
					break;
				case OPTION_MENUE:
					m_bOption = true;
					
					m_nCurOption = SOUND_OPTION;
					break;
					/*case EXIT_MENUE:

						break;*/
				default:
					break;
				}
			}
		}
		else
		{
			if (!m_bSound) {
				// オプション
				if (CInput::GetKeyTrigger(VK_W) /*|| m_bInputUpNow && !m_bInputUpOld*/)
				{
					CSound::Play(SE_MENUE);
					m_nCurOption = (Option_Menue)((m_nCurOption + MAX_OPTION - 1) % MAX_OPTION);

				}
				else if (CInput::GetKeyTrigger(VK_S) /*|| m_bInputDownNow && !m_bInputDownOld*/)
				{
					CSound::Play(SE_MENUE);
					m_nCurOption = (Option_Menue)((m_nCurOption + 1) % MAX_OPTION);

				}


				if (CInput::GetKeyRelease(VK_SPACE)) {
					CSound::Play(SE_DECID);
					switch (m_nCurOption)
					{
					case SOUND_OPTION:
						m_bSound = true;
						break;
					case EXIT_OPTION:
						m_bOption = false;
						break;
						/*case EXIT_MENUE:

							break;*/
					default:
						break;
					}
				}
			}
			else
			{
				// サウンド
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

				if (m_nCurSound != Exit_SOUND){
					if (CInput::GetKeyPress(VK_A) /*|| m_bInputUpNow && !m_bInputUpOld*/)
					{
						/*m_fVolume[m_nCurSound] -= g_fVolume_Value;
						if (m_fVolume[m_nCurSound] < 0.0f) {
							m_fVolume[m_nCurSound] = 0.0f;
						}
						else if (g_fMaxVolume < m_fVolume[m_nCurSound])
						{
							m_fVolume[m_nCurSound] = g_fMaxVolume;
						}*/

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
						CSound::Play(SE_DECID);
						m_bSound = false;
					}
				}

				

				
			}
		}
	}

	// エフェクシア更新
	//m_EffekseerManager.Update();
}

// 描画
void CTitle::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	CPolygon::SetTexture(m_pTexBG);
	CPolygon::SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetPos(0.0f, 0.0f);
	CPolygon::Draw(pDC);

	CPolygon::SetTexture(m_pTexTitle);
	CPolygon::SetSize(g_fTexTitleWidth, g_fTexTitleHeight);
	CPolygon::Draw(pDC);

	if (!m_bMenue) {
		CPolygon::SetAlpha((m_nTimer <= 60) ? m_nTimer / 60.0f : (120 - m_nTimer) / 60.0f);
		CPolygon::SetTexture(m_pTexStart);
		CPolygon::SetSize(g_fTexStartWidth, g_fTexStartHeight);
		CPolygon::SetPos(0.0f, g_fTexStartY);
		CPolygon::Draw(pDC);
	}
	else
	{
		if (!m_bOption) {
			for (int i = 0; i < MAX_MENUE; ++i)
			{
				int fPosY = -100.0f;
				fPosY -= i * SELECT_SIZE_Y;
				fPosY -= i * SELECT_INTERVAL;
				//fPosY -= (SELECT_SIZE_Y - SELECT_INTERVAL) / 2.0f;

				if (i == m_nCurMenue) {

					// 枠
					CPolygon::SetTexture(nullptr);
					CPolygon::SetPos(0.0f, fPosY + 6.0f);
					CPolygon::SetSize(250.0f + m_nSclCnt, 50.0f + m_nSclCnt);
					CPolygon::SetColor(m_vColor.x, m_vColor.y, m_vColor.z);
					CPolygon::SetAlpha(1.0f);
					CPolygon::Draw(pDC);

					// メニュー画像
					CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
					CPolygon::SetTexture(m_pTexMenue[i]);
					CPolygon::SetSize(SELECT_SIZE_X, SELECT_SIZE_Y);
					CPolygon::SetPos(0.0f, fPosY);
					CPolygon::Draw(pDC);
				}
				else
				{
					// ステージ画像
					CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
					CPolygon::SetTexture(m_pTexMenue[i]);
					CPolygon::SetSize(SELECT_SIZE_X, SELECT_SIZE_Y);
					CPolygon::SetPos(0.0f, fPosY);
					CPolygon::Draw(pDC);
				}
			}
		}
		else
		{

			if (!m_bSound) {
				// プレート
				CPolygon::SetTexture(nullptr);
				CPolygon::SetPos(0.0f, -40.0f);
				CPolygon::SetSize(400.0f, 200.0f);
				CPolygon::SetColor(0.5f, 0.3f, 0.1f);
				CPolygon::SetAlpha(0.4f);
				CPolygon::Draw(pDC);


				// オプションメニュー
				for (int i = 0; i < MAX_OPTION; ++i)
				{
					int fPosY = 0.0f;
					fPosY -= i * SELECT_SIZE_Y;
					fPosY -= i * SELECT_INTERVAL;
					//fPosY -= (SELECT_SIZE_Y - SELECT_INTERVAL) / 2.0f;

					if (i == m_nCurOption) {

						// 枠
						CPolygon::SetTexture(nullptr);
						CPolygon::SetPos(0.0f, fPosY + 6.0f);
						CPolygon::SetSize(250.0f + m_nSclCnt, 50.0f + m_nSclCnt);
						CPolygon::SetColor(m_vColor.x, m_vColor.y, m_vColor.z);
						CPolygon::SetAlpha(1.0f);
						CPolygon::Draw(pDC);

						// メニュー画像
						CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
						CPolygon::SetTexture(m_pTexOption[i]);
						CPolygon::SetSize(SELECT_SIZE_X, SELECT_SIZE_Y);
						CPolygon::SetPos(0.0f, fPosY);
						CPolygon::Draw(pDC);
					}
					else
					{
						// ステージ画像
						CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
						CPolygon::SetTexture(m_pTexOption[i]);
						CPolygon::SetSize(SELECT_SIZE_X, SELECT_SIZE_Y);
						CPolygon::SetPos(0.0f, fPosY);
						CPolygon::Draw(pDC);
					}

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
				CPolygon::Draw(pDC);


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
						CPolygon::Draw(pDC);

						// メニュー画像
						CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
						CPolygon::SetTexture(m_pTexSound[i]);
						CPolygon::SetSize(SELECT_SIZE_X, SELECT_SIZE_Y);
						CPolygon::SetPos(-300.0f, fPosY);
						CPolygon::Draw(pDC);
					}
					else
					{
						// ステージ画像
						CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
						CPolygon::SetTexture(m_pTexSound[i]);
						CPolygon::SetSize(SELECT_SIZE_X, SELECT_SIZE_Y);
						CPolygon::SetPos(-300.0f, fPosY);
						CPolygon::Draw(pDC);
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
						CPolygon::Draw(pDC);
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
						CPolygon::Draw(pDC);
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
						CPolygon::Draw(pDC);
					}
					break;
					}


				}
			}
		}
	}

	// エフェクシア描画
	//m_EffekseerManager.Draw();
}
