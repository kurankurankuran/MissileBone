// �`���[�g���A���C�x���g�t���O�Ǘ��N���X EventManager_Tutorial.cpp

#include "EventManager_Tutorial.h"
#include "Texture.h"
#include "Polygon.h"
#include "Scene.h"
#include "EnemyManager_Tutorial.h"
#include "Fade.h"

namespace {

	const float g_fFadeSpeed = 1.0f / 60.0f;
	const int g_nTexDrawTime = 1 * 60;
	const int g_nStopTime[MAX_TUTORIAL] = {
		3 * 60,
		3 * 60,
		3 * 60,
	};
	const int g_nNormalStopTime = 3 * 60;
	const LPCWSTR g_pszPathTex[MAX_TEXTURE] =
	{
		L"data/texture/IdouKunren.png",
		L"data/texture/SyagekiKunren.png",
		L"data/texture/ZissenKunren.png",
	};
	const LPCWSTR g_pszPathTexMoveTutorial = L"data/texture/MOVE_TUTORIAL.png";
	const LPCWSTR g_pszPathTexWeaponTutorial = L"data/texture/WEAPON_TUTORIAL.png";
	const LPCWSTR g_pszPathTexPauseTutorial = L"data/texture/PAUSE_TUTORIAL.png";
}

CEventManager_Tutorial::CEventManager_Tutorial(CScene* pScene) : CEventManager(pScene)
{
	SetEvent_ID(TUTORIAL_EVENT_ID);
	m_nCurEventType = MAX_TUTORIAL;

	for (int i = 0; i < MAX_TEXTURE; ++i) {
		m_pTex[i] = nullptr;	// �|�[�Y���j���[�e�N�X�`��
	}
	m_pTexMoveTutorial = nullptr;
	m_pTexWeaponTutorial = nullptr;
	m_pTexPauseTutorial = nullptr;
	m_pEnemyManager = nullptr;
}

CEventManager_Tutorial::~CEventManager_Tutorial()
{

}

HRESULT CEventManager_Tutorial::Init()
{
	HRESULT hr = S_OK;

	m_bEvent = false;
	m_bSetCameraMode = false;

	// �G�l�~�[�}�l�[�W���[�擾
	m_pEnemyManager = (CEnemyManager*)m_pScene->Find(GOT_ENEMYMANAGER);
	if (!m_pEnemyManager)
	{
		Fin();
		return false;
	}

	// �J�����擾
	m_pCamera = m_pScene->GetCamera();
	if (!m_pCamera) {
		return false;
	}

	m_nTexFade = FADE_TEXNONE;
	m_nTexDrawCnt = g_nTexDrawTime;

	m_nStopTime.clear();
	for (int i = 0; i < MAX_TUTORIAL; ++i) {
		m_nStopTime.push_back(g_nStopTime[i]); /*= g_nStopTime[i];*/
	}
	m_nStopCnt = 0;
	StartEventType(MOVE_TUTORIAL);

	ID3D11Device* pDevice = GetDevice();
	m_fAlpha = 0.0f;
	/*m_bFadeIn = false;
	m_bFadeOut = false;*/
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		/*m_bFade[i] = false;*/
		/*m_fAlpha[i] = 0.0f;*/

		hr = CreateTextureFromFile(
			pDevice,					// �f�o�C�X�ւ̃|�C���^
			g_pszPathTex[i],		// �t�@�C���̖��O
			&m_pTex[i]);					// �ǂݍ��ރ������[

		if (FAILED(hr)) {
			Fin();
			return false;
		}
	}

	hr = CreateTextureFromFile(
		pDevice,					// �f�o�C�X�ւ̃|�C���^
		g_pszPathTexMoveTutorial,		// �t�@�C���̖��O
		&m_pTexMoveTutorial);					// �ǂݍ��ރ������[
	if (FAILED(hr)) {
		Fin();
		return false;
	}

	hr = CreateTextureFromFile(
		pDevice,					// �f�o�C�X�ւ̃|�C���^
		g_pszPathTexWeaponTutorial,		// �t�@�C���̖��O
		&m_pTexWeaponTutorial);					// �ǂݍ��ރ������[
	if (FAILED(hr)) {
		Fin();
		return false;
	}

	hr = CreateTextureFromFile(
		pDevice,					// �f�o�C�X�ւ̃|�C���^
		g_pszPathTexPauseTutorial,		// �t�@�C���̖��O
		&m_pTexPauseTutorial);					// �ǂݍ��ރ������[
	if (FAILED(hr)) {
		Fin();
		return false;
	}

	return hr;
}

void CEventManager_Tutorial::Fin()
{
	// �e�N�X�`���̉��
	for (int i = 0; i < MAX_TEXTURE; ++i) {
		SAFE_RELEASE(m_pTex[i]);
	}

	SAFE_RELEASE(m_pTexMoveTutorial);
	SAFE_RELEASE(m_pTexWeaponTutorial);
	SAFE_RELEASE(m_pTexPauseTutorial);

}

void CEventManager_Tutorial::Update()
{
	if (CFade::Get() != FADE_NONE)return;

	CEventManager::Update();

	if (m_bEvent && m_pEnemyManager->GetWaveEnemyNum() <= 0) {
		m_bEvent = false;
		++m_nCurEventType;

		if(m_nCurEventType != FIGHT_TUTORIAL)
		StartEventType(m_nCurEventType);
	}else if (m_nStopCnt <= 0 && m_pEnemyManager->GetWaveEnemyNum() <= 0){
	
		// �C�x���g���Ƃ̓G����
		m_nStopCnt = 1;
	
		m_pEnemyManager->CreateWave(m_nCurEventType + 1);
		StartPause(g_nNormalStopTime);

		m_pCamera->SetMode(BULLET_FREE_CAMERAMODE);
		m_pCamera->SetTarget(m_pEnemyManager->GetCameraTargetPos());
		m_pCamera->SetFreeBulletMode(m_pEnemyManager->GetfFreeBulletMode());
		m_bSetCameraMode = true;
		m_bEvent = true;
	}

	if (m_bSetCameraMode && m_nStopCnt <= 0) {
		m_pCamera->SetMode(TPS_CAMERAMODE);
		m_bSetCameraMode = false;
	}

	
	

	// �e�N�X�`���t�F�[�h����
	if (m_fAlpha >= 1.0f) {
		--m_nTexDrawCnt;
		if (m_nTexDrawCnt < 0)
		{
			m_nTexDrawCnt = g_nTexDrawTime;
			m_nTexFade = FADE_TEXOUT;
		}
	}
	if (m_nTexFade == FADE_TEXIN) {

		m_fAlpha += g_fFadeSpeed;
		if (m_fAlpha > 1.0f) {
			m_fAlpha = 1.0f;
			m_nTexFade = FADE_TEXNONE;
		}
	}
	else if (m_nTexFade == FADE_TEXOUT) {
		m_fAlpha -= g_fFadeSpeed;
		if (m_fAlpha < 0.0f) {
			m_fAlpha = 0.0f;
			m_nTexFade = FADE_TEXNONE;
		}
	}
}

void CEventManager_Tutorial::Draw()
{
	if (CFade::Get() != FADE_NONE)return;

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);								// Z�o�b�t�@�X�V���Ȃ�


	CPolygon::SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	CPolygon::SetPos(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetColor(1.0f, 1.0f, 1.0f/* g_fCol, g_fCol, g_fCol */);

	
	CPolygon::SetAlpha(m_fAlpha);
	
	switch (m_nCurEventType)
	{
	case MOVE_TUTORIAL:
		// �ړ��P��
		CPolygon::SetTexture(m_pTex[MOVE_TUTORIAL]);
		CPolygon::SetAlpha(m_fAlpha);

		CPolygon::Draw(pDeviceContext);
		

		if (m_fAlpha <= 0) {
			CPolygon::SetTexture(m_pTexMoveTutorial);
			CPolygon::SetSize(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.6f);
			CPolygon::SetPos(25.0f, -250.0f);
			CPolygon::SetFrameSize(1.0f, 1.0f);
			CPolygon::SetUV(0.0f, 0.0f);
			CPolygon::SetColor(1.0f, 1.0f, 1.0f);
			CPolygon::SetAlpha(1.0f);
			CPolygon::Draw(pDeviceContext);
		}

		break;
	case ATTACK_TUTORIAL:
		CPolygon::SetTexture(m_pTex[ATTACK_TUTORIAL]);
		CPolygon::SetAlpha(m_fAlpha/*[0]*/);
		CPolygon::Draw(pDeviceContext);
		
		if (m_fAlpha <= 0) {
			CPolygon::SetTexture(m_pTexWeaponTutorial);
			CPolygon::SetSize(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.6f);
			CPolygon::SetPos(25.0f, -250.0f);
			CPolygon::SetFrameSize(1.0f, 1.0f);
			CPolygon::SetUV(0.0f, 0.0f);
			CPolygon::SetColor(1.0f, 1.0f, 1.0f);
			CPolygon::SetAlpha(1.0f);
			CPolygon::Draw(pDeviceContext);
		}
			
		break;
	case FIGHT_TUTORIAL:
		
		CPolygon::SetTexture(m_pTex[FIGHT_TUTORIAL]);
		CPolygon::SetAlpha(m_fAlpha/*[0]*/);
		CPolygon::Draw(pDeviceContext);
		if (m_fAlpha <= 0) {
			CPolygon::SetTexture(m_pTexPauseTutorial);
			CPolygon::SetSize(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.6f);
			CPolygon::SetPos(25.0f, -250.0f);
			CPolygon::SetFrameSize(1.0f, 1.0f);
			CPolygon::SetUV(0.0f, 0.0f);
			CPolygon::SetColor(1.0f, 1.0f, 1.0f);
			CPolygon::SetAlpha(1.0f);
			CPolygon::Draw(pDeviceContext);
		}

		break;
	default:
		break;
	}


	

	

	CPolygon::SetColor(0.0f, 0.0f, 0.0f);
	SetZWrite(true);							// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);		// �A���t�@�u�����h����
}
