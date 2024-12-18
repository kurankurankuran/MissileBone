//=============================================================================
//
// �X�e�[�W�Z���N�g���� [StageSelect.cpp]
// Author : KAZUKI KURA
//
//=============================================================================

#include "main.h"
#include "Texture.h"
#include "StageSelect.h"
#include "input.h"
#include "fade.h"
#include "scene.h"
#include "polygon.h"
#include "debugproc.h"
#include "Sound.h"
#include "EnemyManager_StageSelect.h"
#include "EffectManager.h"
#include "EffekseerManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

namespace {
	const int SCALE_RATE = 1;             // �g�k���x
	const float COLOR_RATE = 0.01f;       // �F�ω����x
	const float SELECT_INTERVAL = 100.0f; // �g�Ԃ̋����Ԋu
	const float SELECT_SIZE_X = 180.0f;   // �摜�̉��T�C�Y
	const float SELECT_SIZE_Y = 300.0f;   // �摜�̏c�T�C�Y
	const float FRAME_WIDTH = 20.0f;      // �g�̕��T�C�Y

	const int MAX_SCALE_UP = 50.0f;       // �c�����ő�
	const int MIN_SCALE_DOWN = -10.0f;    // �k�����ŏ�

	//const LPCWSTR g_FileNameBG = L"data/texture/TitleName.png";
	const LPCWSTR g_FileNameStage[MAX_STAGE_SELECT] =
	{
		L"data/texture/SELECT_TUTORIAL.png",		// �X�e�[�W�P
		L"data/texture/SELECT_STAGE_1.png",		// �X�e�[�W�Q
		L"data/texture/SELECT_STAGE_2.png",		// �X�e�[�W�R
		L"data/texture/SELECT_STAGE_3.png",		// �X�e�[�W�R
		//L"data/texture/TitleName.png",		// �X�e�[�W�R
		//L"data/texture/TitleName.png",		// �X�e�[�W�R
	};
}


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

CStageSelect::CStageSelect()
{
	m_id = SCENE_SELECT;

	m_nCurrentSelect = TUTORIAL;
	/*m_bInputUpNow = false;
	m_bInputUpOld = false;
	m_bInputDownNow = false;
	m_bInputDownOld = false;*/

	//m_pTexBG = nullptr;

	for (int i = 0; i < MAX_STAGE_SELECT; ++i) {
		m_pTexStage[i] = nullptr;
	}
}

CStageSelect::~CStageSelect()
{

}

//=============================================================================
// �X�e�[�W�Z���N�g�̏���������
//=============================================================================
bool CStageSelect::Init()
{
	if(!CScene::Init()) {
		return false;
	}

	m_nSclCnt = 0; // �g�k�^�C�}�[
	m_bScaleUp = true; // �g�咆�t���O
	m_vColor = XMFLOAT3(1.0f, 0.0f, 0.0f); // �g�F


	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���ǂݍ���
	/*hr = CreateTextureFromFile(pDevice, g_FileNameBG, &m_pTexBG);
	if (FAILED(hr))
	{
		return hr;
	}*/
	
	// �X�e�[�W�I���̃e�N�X�`���ǂݍ���
	for (int i = 0; i < MAX_STAGE_SELECT; ++i)
	{
		hr = CreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			g_FileNameStage[i],		// �t�@�C���̖��O
			&m_pTexStage[i]);					// �ǂݍ��ރ������[
		if (FAILED(hr))
		{
			return hr;
		}
	}


	// �I�u�W�F�N�g����
	m_camera.Init();
	m_camera.SetSkyDome(CModel::GetAssimp(MODEL_SKY));
	SetCamera(&m_camera);

	m_light.Init();
	SetLight(&m_light);


	// �G�t�F�N�V�A������
	m_pEffekseerManager = new CEffekseerManager(this);
	m_pEffekseerManager->Init();
	m_pEffekseerManager->SetCamera(&m_camera);

	// �X�e�[�W���E��
	m_pStageSphere = new CStageSphere(this);
	new CEffectManager(this);
	// �G�l�~�[
	CEnemyManager* pEnemyManager = new CEnemyManager_StageSelect(this);
	pEnemyManager->SetTarget(m_pStageSphere);
	// �S�I�u�W�F�N�g������
	if (FAILED(CGameObj::InitAll(m_pObj))) {
		return false;
	}

	m_pStageSphere->SetRadius(100.0f);
	m_pStageSphere->SetPos(XMFLOAT3(0.0f, -1500.0f, 7000.0f));
	//m_pStageSphere->SetCenter(XMFLOAT3(0.0f, 0.0f, 10000.0f));

	// BGM�Đ��J�n
	CSound::Play(BGM_STAGESELECT);


	return hr;
}


//=============================================================================
// �X�e�[�W�Z���N�g�̏I������
//=============================================================================
void CStageSelect::Fin()
{
	//SAFE_RELEASE(m_pTexBG);
	for (int i = 0; i < MAX_STAGE_SELECT; ++i) {
		SAFE_RELEASE(m_pTexStage[i]);
	}


	

	// BGM�Đ���~
	CSound::Stop(BGM_STAGESELECT);

	// �S�I�u�W�F�N�g�I������
	CGameObj::FinAll(m_pObj);


	// �G�t�F�N�V�A�I��
	m_pEffekseerManager->Uninit();
	delete m_pEffekseerManager;
}


//=============================================================================
// �X�e�[�W�Z���N�g�̍X�V����
//=============================================================================
void CStageSelect::Update()
{
	/***** �I�����ꂽ�X�e�[�W�摜�̊g��k�� *****/
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

	/***** �I�����ꂽ�g(�w�i�摜)�̕\��*****/
	if (m_vColor.y <= 0.0f && m_vColor.z <= 1.0f)
	{
		m_vColor.z-= COLOR_RATE;
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

	/*m_bInputDownOld = m_bInputDownNow;
	m_bInputUpOld = m_bInputUpNow;*/

	if (CFade::Get() != FADE_NONE) return;
	if (CInput::GetKeyTrigger(VK_A) /*|| m_bInputUpNow && !m_bInputUpOld*/)
	{
		CSound::Play(SE_MENUE);
		m_nCurrentSelect = (STAGE_MENU)((m_nCurrentSelect + MAX_STAGE_SELECT - 1) % MAX_STAGE_SELECT);

	}
	else if (CInput::GetKeyTrigger(VK_D) /*|| m_bInputDownNow && !m_bInputDownOld*/)
	{
		CSound::Play(SE_MENUE);
		m_nCurrentSelect = (STAGE_MENU)((m_nCurrentSelect + 1) % MAX_STAGE_SELECT);

	}


	if (CInput::GetKeyRelease(VK_SPACE)) {
		CSound::Play(SE_SELECT_DECID);
		switch (m_nCurrentSelect)
		{
		case TUTORIAL:
			CFade::Out(SCENE_TUTORIAL);
			break;
		case STAGE_1:
			CFade::Out(SCENE_GAME_1);
			break;
		case STAGE_2:
			CFade::Out(SCENE_GAME_2);
			break;
		case STAGE_3:
			CFade::Out(SCENE_GAME_3);
			break;
		default:
			break;
		}
	}


	// �S�L�����X�V
	CGameObj::UpdateAll(m_pObj);

	// �G�t�F�N�V�A�X�V
	m_pEffekseerManager->Update();
}


//=============================================================================
// �X�e�[�W�Z���N�g�̕`�揈��
//=============================================================================
void CStageSelect::Draw()
{

	// �S�L�����`��
	CGameObj::DrawAll(m_pObj);

	// �G�t�F�N�V�A�`��
	m_pEffekseerManager->Draw();


	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);


	//// �w�i�`��
	//CPolygon::SetTexture(m_pTexBG);
	//CPolygon::SetPos(0.0f, 0.0f);
	//CPolygon::SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	//CPolygon::SetFrameSize( 1.0f, 1.0f);
	//CPolygon::SetUV(0.0f, 0.0f);
	//CPolygon::Draw(pDeviceContext);

	// �I����ʕ`��
	int i;
	if (MAX_STAGE_SELECT % 2) {
		// �
		int fPosX;
		for (i = 0; i < MAX_STAGE_SELECT; ++i) {

			fPosX = (i - MAX_STAGE_SELECT / 2) * SELECT_SIZE_X;
			fPosX += (i - MAX_STAGE_SELECT / 2) * SELECT_INTERVAL;

			if (i == m_nCurrentSelect) {
				// �g
				CPolygon::SetTexture(nullptr);
				CPolygon::SetPos(fPosX, 0.0f);
				CPolygon::SetSize(SELECT_SIZE_X + m_nSclCnt + FRAME_WIDTH, 100.0f + m_nSclCnt + FRAME_WIDTH);
				CPolygon::SetColor(m_vColor.x, m_vColor.y, m_vColor.z);
				CPolygon::SetAlpha(1.0f);
				CPolygon::Draw(pDeviceContext);

				// �X�e�[�W�摜
				CPolygon::SetTexture(m_pTexStage[i]);
				CPolygon::SetPos(fPosX, 0.0f);
				CPolygon::SetSize(SELECT_SIZE_X + m_nSclCnt, SELECT_SIZE_Y  + m_nSclCnt);
				CPolygon::SetColor(1.0f, 1.0f, 1.0f);
				CPolygon::SetAlpha(1.0f);
				CPolygon::Draw(pDeviceContext);
			}
			else
			{
				// �X�e�[�W�摜
				CPolygon::SetTexture(m_pTexStage[i]);
				CPolygon::SetPos(fPosX, 0.0f);
				CPolygon::SetSize(SELECT_SIZE_X, SELECT_SIZE_Y);
				CPolygon::SetColor(1.0f, 1.0f, 1.0f);
				CPolygon::SetAlpha(1.0f);
				CPolygon::Draw(pDeviceContext);
			}
		}
		

		
	}
	else
	{
		// ����


		for (int i = 0; i < MAX_STAGE_SELECT; ++i) {
			int fPosX;
			fPosX = (i - MAX_STAGE_SELECT / 2) * SELECT_SIZE_X;
			fPosX += (i - MAX_STAGE_SELECT / 2) * SELECT_INTERVAL;
			fPosX += (SELECT_SIZE_X + SELECT_INTERVAL) / 2.0f;

			if (i == m_nCurrentSelect) {
				// �g
				CPolygon::SetTexture(nullptr);
				CPolygon::SetPos(fPosX, 0.0f);
				CPolygon::SetSize(SELECT_SIZE_X + m_nSclCnt + FRAME_WIDTH, 100.0f + m_nSclCnt + FRAME_WIDTH);
				CPolygon::SetColor(m_vColor.x, m_vColor.y, m_vColor.z);
				CPolygon::SetAlpha(1.0f);
				CPolygon::Draw(pDeviceContext);

				// �X�e�[�W�摜
				CPolygon::SetTexture(m_pTexStage[i]);
				CPolygon::SetPos(fPosX, 0.0f);
				CPolygon::SetSize(SELECT_SIZE_X + m_nSclCnt, SELECT_SIZE_Y + m_nSclCnt);
				CPolygon::SetColor(1.0f, 1.0f, 1.0f);
				CPolygon::SetAlpha(1.0f);
				CPolygon::Draw(pDeviceContext);
			}
			else {
				// �X�e�[�W�摜
				CPolygon::SetTexture(m_pTexStage[i]);
				CPolygon::SetPos(fPosX, 0.0f);
				CPolygon::SetSize(SELECT_SIZE_X, SELECT_SIZE_Y);
				CPolygon::SetColor(1.0f, 1.0f, 1.0f);
				CPolygon::SetAlpha(1.0f);
				CPolygon::Draw(pDeviceContext);
			}
		}

		
	}
	
	


	SetZWrite(true);							// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);		// �A���t�@�u�����h����
}
