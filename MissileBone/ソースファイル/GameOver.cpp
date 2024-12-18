//=============================================================================
//
// ゲームオーバークラス実装 [GameOver.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include "GameOver.h"
#include "Texture.h"
#include "Sound.h"
#include "Input.h"
#include "Polygon.h"
#include "Fade.h"

// 定数定義
namespace {
	const LPCWSTR g_pszPathTexGameOver = L"data/texture/GameOver.png";
	const float g_fTexGameOverWidth = 1120.0f;
	const float g_fTexGameOverHeight = 960.0f;

	const LPCWSTR g_pszPathTexStart = L"data/texture/PressSpaceKey.png";
	const float g_fTexStartWidth = 960.0f;
	const float g_fTexStartHeight = 640.0f;
	const float g_fTexStartY = SCREEN_HEIGHT / -3.0f;

	const LPCWSTR g_pszPathTexBG = L"data/texture/BG00g1_80.jpg";
}

// コンストラクタ
CGameOver::CGameOver() : CScene()
{
	m_id = SCENE_GAMEOVER;

	m_bStart = false;
	m_pTexGameOver = nullptr;
	m_pTexStart = nullptr;
	m_pTexBG = nullptr;
	m_nTimer = 0;
}

// デストラクタ
CGameOver::~CGameOver()
{
}

// 初期化
bool CGameOver::Init()
{
	if (!CScene::Init()) {
		return false;
	}

	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	m_bStart = false;
	// テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice, g_pszPathTexGameOver, &m_pTexGameOver);
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
	CSound::Play(BGM_GAMEOVER);

	return true;
}

// 終了処理
void CGameOver::Fin()
{
	// BGM再生停止
	CSound::Stop(BGM_GAMEOVER);

	SAFE_RELEASE(m_pTexBG);
	SAFE_RELEASE(m_pTexStart);
	SAFE_RELEASE(m_pTexGameOver);

	CScene::Fin();
}

// 更新
void CGameOver::Update()
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
void CGameOver::Draw()
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

	CPolygon::SetTexture(m_pTexGameOver);
	CPolygon::SetSize(g_fTexGameOverWidth, g_fTexGameOverHeight);
	CPolygon::Draw(pDC);

	CPolygon::SetAlpha((m_nTimer <= 60) ? m_nTimer / 60.0f : (120 - m_nTimer) / 60.0f);
	CPolygon::SetTexture(m_pTexStart);
	CPolygon::SetSize(g_fTexStartWidth, g_fTexStartHeight);
	CPolygon::SetPos(0.0f, g_fTexStartY);
	CPolygon::Draw(pDC);
}
