//=============================================================================
//
// ゲームオーバークラス実装 [GameClear.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include "GameClear.h"
#include "Texture.h"
#include "Sound.h"
#include "Input.h"
#include "Polygon.h"
#include "Fade.h"

// 定数定義
namespace {
	const LPCWSTR g_pszPathTexGameClear = L"data/texture/Congratulations.png";
	const float g_fTexGameClearWidth = 1120.0f;
	const float g_fTexGameClearHeight = 960.0f;

	const LPCWSTR g_pszPathTexThanks = L"data/texture/ThankyouforPlaying.png";
	const float g_fTexThanksWidth = 960.0f;
	const float g_fTexThanksHeight = 800.0f;

	const LPCWSTR g_pszPathTexStart = L"data/texture/PressSpaceKey.png";
	const float g_fTexStartWidth = 960.0f;
	const float g_fTexStartHeight = 640.0f;
	const float g_fTexStartY = SCREEN_HEIGHT / -3.0f;

	const LPCWSTR g_pszPathTexBG = L"data/texture/BG00z_80.jpg";
}

// コンストラクタ
CGameClear::CGameClear() : CScene()
{
	m_id = SCENE_GAMECLEAR;

	m_bStart = false;
	m_pTexGameClear = nullptr;
	m_pTexStart = nullptr;
	m_pTexBG = nullptr;
	m_nTimer = 0;
}

// デストラクタ
CGameClear::~CGameClear()
{
}

// 初期化
bool CGameClear::Init()
{
	if (!CScene::Init()) {
		return false;
	}

	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	m_bStart = false;
	// テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice, g_pszPathTexGameClear, &m_pTexGameClear);
	if (FAILED(hr)) {
		return false;
	}

	hr = CreateTextureFromFile(pDevice, g_pszPathTexThanks, &m_pTexThanks);
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

	m_nTimer = 0;

	// BGM再生開始
	CSound::Play(BGM_GAMECLEAR);

	return true;
}

// 終了処理
void CGameClear::Fin()
{
	// BGM再生停止
	CSound::Stop(BGM_GAMECLEAR);

	SAFE_RELEASE(m_pTexBG);
	SAFE_RELEASE(m_pTexStart);
	SAFE_RELEASE(m_pTexThanks);
	SAFE_RELEASE(m_pTexGameClear);

	CScene::Fin();
}

// 更新
void CGameClear::Update()
{
	++m_nTimer;
	if (m_nTimer > 120) {
		m_nTimer = 0;
	}

	if (!m_bStart) {
		if (CInput::GetKeyRelease(VK_SPACE) ||
			CInput::GetKeyRelease(VK_RETURN)) {
			m_bStart = true;
			CFade::Out(SCENE_TITLE);
			CSound::Play(SE_DECID);
		}
	}
}

// 描画
void CGameClear::Draw()
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

	
	CPolygon::SetTexture(m_pTexGameClear);
	CPolygon::SetSize(g_fTexGameClearWidth, g_fTexGameClearHeight);
	CPolygon::SetPos(0.0f, 100.0f);
	CPolygon::Draw(pDC);

	CPolygon::SetTexture(m_pTexThanks);
	CPolygon::SetSize(g_fTexThanksWidth, g_fTexThanksHeight);
	CPolygon::SetPos(0.0f, -100.0f);
	CPolygon::Draw(pDC);
	

	CPolygon::SetAlpha((m_nTimer <= 60) ? m_nTimer / 60.0f : (120 - m_nTimer) / 60.0f);
	CPolygon::SetTexture(m_pTexStart);
	CPolygon::SetSize(g_fTexStartWidth, g_fTexStartHeight);
	CPolygon::SetPos(0.0f, g_fTexStartY);
	CPolygon::Draw(pDC);
}
