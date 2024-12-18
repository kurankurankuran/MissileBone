// �`���[�g���A���C�x���g�t���O�Ǘ��N���X EventManager_StartEnd.cpp

#include "EventManager_StartEnd.h"
#include "Texture.h"
#include "Polygon.h"
#include "Scene.h"
#include "Fade.h"
#include "GameObj.h"
#include "EnemyManager.h"
#include "Input.h"

namespace {

	const float g_fFadeSpeed = 1.0f / 60.0f;
	const int g_nTexDrawTime = 1 * 60;
	const int g_nStopTime[MAX_STARTEND] = {
		3 * 60,
		0/*3 * 60*/,
		3 * 60,
	};
	const int g_nNormalStopTime = 3 * 60;
	

	const LPCWSTR g_pszPathTex[MAX_STARTEND] =
	{
		L"data/texture/GAMESTART.png",
		L"data/texture/WAVE_NUM.png",
		L"data/texture/TARGETDESTROYED.png",
	};

	const LPCWSTR g_pszPathTexNum = L"data/texture/Number.png";
	const float g_SizeX = 80.0f; // �T�C�YX
	const float g_SizeY = 200.0f; // �T�C�YY
	const float g_PosX = 0.0f + g_SizeX/2.0f; // �ʒuX
	const float g_PosY = 100.0f; // �ʒuY
	const int g_NumU = 10; // �����e�N�X�`����������
	const int g_NumV = 1; // �����e�N�X�`���c������
}

CEventManager_StartEnd::CEventManager_StartEnd(CScene* pScene) :CEventManager(pScene)
{
	SetEvent_ID(STARTEND_EVENT_ID);
	m_nCurEventType = MAX_STARTEND;

	for (int i = 0; i < MAX_STARTEND; ++i) {
		m_pTex[i] = nullptr;	// �|�[�Y���j���[�e�N�X�`��
	}

	m_pEnemyManager = nullptr;
}

CEventManager_StartEnd::~CEventManager_StartEnd()
{

}

HRESULT CEventManager_StartEnd::Init()
{
	HRESULT hr = S_OK;

	// �J�����擾
	m_pCamera = m_pScene->GetCamera();
	if (!m_pCamera) {
		return false;
	}

	m_nTexFade = FADE_TEXNONE;
	m_nTexDrawCnt = g_nTexDrawTime;

	m_nStopTime.clear();
	for (int i = 0; i < MAX_STARTEND; ++i) {
		m_nStopTime.push_back(g_nStopTime[i]); /*= g_nStopTime[i];*/
	}
	m_nStopCnt = 0;
	StartEventType(START_STARTEND);
	//m_bCreate = true;

	ID3D11Device* pDevice = GetDevice();
	m_fAlpha = 0.0f;

	/*m_bFadeIn = false;
	m_bFadeOut = false;*/
	
	for (int i = 0; i < MAX_STARTEND; i++)
	{
		hr = CreateTextureFromFile(
			pDevice,				// �f�o�C�X�ւ̃|�C���^
			g_pszPathTex[i],		// �t�@�C���̖��O
			&m_pTex[i]);			// �ǂݍ��ރ������[

		if (FAILED(hr)) {
			Fin();
			return false;
		}
	}

	// �G�l�~�[�}�l�[�W���[�擾
	m_pEnemyManager = (CEnemyManager*)m_pScene->Find(GOT_ENEMYMANAGER);
	if (!m_pEnemyManager)
	{
		Fin();
		return false;
	}

	// ����
	hr = CreateTextureFromFile(
		pDevice,					// �f�o�C�X�ւ̃|�C���^
		g_pszPathTexNum,		// �t�@�C���̖��O
		&m_pTextureNum);					// �ǂݍ��ރ������[
	if (FAILED(hr)) {
		Fin();
		return false;
	}

	m_bCreate = false;
	m_bSetCameraMode = false;

	return hr;
}

void CEventManager_StartEnd::Fin()
{

	// �e�N�X�`���̉��
	SAFE_RELEASE(m_pTextureNum);
	for (int i = 0; i < MAX_STARTEND; ++i) {
		SAFE_RELEASE(m_pTex[i]);
	}
}

void CEventManager_StartEnd::Update()
{
	// �t�F�[�h���I�����Ă��珈��
	if (CFade::Get() != FADE_NONE)return;


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

	
	
	CEventManager::Update();

	if (m_nCurEventType == WAVE_STARTEND && m_pEnemyManager->GetWaveEnemyNum() <= 0 && m_fAlpha <= 0 && !m_bCreate)
	{
		// �E�F�[�u�ԍ���\��
		StartEventType(WAVE_STARTEND);
		m_bCreate = true;
	}else if (m_nCurEventType == WAVE_STARTEND && m_bCreate && m_fAlpha <= 0)
	{
		// �G�𐶐�
		m_pEnemyManager->CreateWave(m_pEnemyManager->GetCurWaveNum());
		m_bCreate = false;

		StartPause(g_nNormalStopTime);
	}

	
	if (m_nCurEventType == START_STARTEND && m_nStopCnt <= 0) {
		// �ŏ��̃E�F�[�u�ԍ���\��
		StartEventType(WAVE_STARTEND);
		m_bCreate = true;
	}
	else if (m_nCurEventType == END_STARTEND) {
		// �{�X���|���ꂽ�Ƃ��̕����\��
		if (m_nStopCnt < 1.0f*60.0f) {
			CFade::Out(SCENE_GAMECLEAR);
		}
	}


	if ( m_bEventPause && m_nCurEventType == WAVE_STARTEND) {
		if (m_pCamera->GetMode() != (int)BULLET_FREE_CAMERAMODE) {

			m_pCamera->SetMode(BULLET_FREE_CAMERAMODE);
			m_pCamera->SetTarget(m_pEnemyManager->GetCameraTargetPos());
			m_pCamera->SetFreeBulletMode(m_pEnemyManager->GetfFreeBulletMode());

			m_bSetCameraMode = true;
		}
	}
	else
	{
		if (m_bSetCameraMode) {
			m_pCamera->SetMode(TPS_CAMERAMODE);
			m_bSetCameraMode = false;
		}
	}


#ifdef _DEBUG
	if (CInput::GetKeyRelease(VK_DELETE))
	{
		m_pEnemyManager->DestroyEnemy();
	}
#endif // !_DEBUG
}

void CEventManager_StartEnd::Draw()
{
	if (CFade::Get() != FADE_NONE)return;

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�


	CPolygon::SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	CPolygon::SetPos(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetColor(1.0f, 1.0f, 1.0f/* g_fCol, g_fCol, g_fCol */);


	CPolygon::SetAlpha(m_fAlpha);
	switch (m_nCurEventType)
	{
	case START_STARTEND:
		// �Q�[���X�^�[�g
		CPolygon::SetTexture(m_pTex[START_STARTEND]);
		CPolygon::Draw(pDeviceContext);
		break;
	case WAVE_STARTEND:
	{
		// �E�F�[�u�\��
		CPolygon::SetTexture(m_pTex[WAVE_STARTEND]);
		CPolygon::Draw(pDeviceContext);

		// �c��Wave��
		CPolygon::SetTexture(m_pTextureNum);
		CPolygon::SetSize(g_SizeX, g_SizeY);
		CPolygon::SetFrameSize(1.0f / (float)g_NumU, 1.0f / (float)g_NumV);
		CPolygon::SetColor(1.0f, 1.0f, 0.3f);

		int nCurrentWaveNum = m_pEnemyManager->GetCurWaveNum();
		for (int i = 0; i < 2/*�ő包��*/; ++i) {
			CPolygon::SetPos(g_PosX - (i * g_SizeX), g_PosY);
			int nNum = nCurrentWaveNum % 10;
			float u, v;
			u = (1.0f / (float)g_NumU) * nNum;
			v = 1.0f / (float)g_NumV;
			CPolygon::SetUV(u, v);
			nCurrentWaveNum /= 10;

			CPolygon::Draw(pDeviceContext);
		}
	}
		break;
	case END_STARTEND:
		// �Q�[���N���A�[
		CPolygon::SetTexture(m_pTex[END_STARTEND]);
		CPolygon::Draw(pDeviceContext);
		break;
	default:

		break;
	}

	
	CPolygon::SetColor(0.0f, 0.0f, 0.0f);
	SetZWrite(true);							// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);		// �A���t�@�u�����h����
}
