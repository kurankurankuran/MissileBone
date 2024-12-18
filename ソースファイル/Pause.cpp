//=============================================================================
//
// �ꎞ��~���� [pause.cpp]
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
// �}�N����`
//*****************************************************************************
namespace {
	const float PAUSE_MENU_WIDTH = 300.0f;                  // �|�[�Y���j���[��
	const float PAUSE_MENU_HEIGHT =300.0f;                  // �|�[�Y���j���[����
	const float PAUSE_MENU_POS_X = 0.0f;                    // �|�[�Y���j���[�ʒu(X���W)
	const float PAUSE_MENU_POS_Y = 150.0f;                  // �|�[�Y���j���[�ʒu(Y���W)
	const float PAUSE_MENU_INTERVAL = 100.0f;               // �|�[�Y���j���[�Ԋu
	const float PLATE_WIDTH = 360.0f;                       // �v���[�g�̕�(��)
	const float PLATE_HEIGHT = 430.0f;                      // �v���[�g�̕�(�c)
	const float PLATE_POS_X = 0.0f;                         // �v���[�g�̈ʒu(X���W)
	const float PLATE_POS_Y = 0.0f;                         // �v���[�g�̈ʒu(Y���W)
}


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
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

	const float g_fVolume_Value = 0.02f; // ���ʕω���
	const float g_fMaxVolume = 10.0f; // �ő剹��

	const float g_fMaxSizeX_Bar = 500.0f;


	const int SCALE_RATE = 1;             // �g�k���x
	const float COLOR_RATE = 0.01f;       // �F�ω����x
	const float SELECT_INTERVAL = -100.0f; // �g�Ԃ̋����Ԋu
	const float SELECT_SIZE_X = 240.0f;   // �摜�̉��T�C�Y
	const float SELECT_SIZE_Y = 200.0f;   // �摜�̏c�T�C�Y
	const float FRAME_WIDTH = 20.0f;      // �g�̕��T�C�Y

	const int MAX_SCALE_UP = 40.0f;       // �c�����ő�
	const int MIN_SCALE_DOWN = -10.0f;    // �k�����ŏ�

	
}


CPause::CPause()
{
	m_nSelectMenu = PAUSE_MENU_CONTINUE;

	m_bPause = false;

	//m_bInputUpNow = false;
	//m_bInputUpOld = false;
	//m_bInputDownNow = false;
	//m_bInputDownOld = false;

	m_pTextureControl = nullptr;	// �R���g���[���L�[�e�N�X�`��

	for (int i = 0; i < PAUSE_MENU_MAX; ++i) {
		m_pTextureMenue[i] = nullptr;	// �|�[�Y���j���[�e�N�X�`��
	}
	
}

CPause::~CPause()
{

}


//=============================================================================
// ����������
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


	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(
		pDevice,					// �f�o�C�X�ւ̃|�C���^
		g_pszPathTexControl,		// �t�@�C���̖��O
		&m_pTextureControl);					// �ǂݍ��ރ������[
	if (FAILED(hr)) {
		Fin();
		return false;
	}


	for (int i = 0; i < PAUSE_MENU_MAX; i++)
	{
		
		hr = CreateTextureFromFile(
			pDevice,					// �f�o�C�X�ւ̃|�C���^
			g_pszPathTex[i],		// �t�@�C���̖��O
			&m_pTextureMenue[i]);					// �ǂݍ��ރ������[

		if (FAILED(hr)) {
			Fin();
			return false;
		}
	}


	m_bSound = false;
	for (int i = 0; i < MAX_SOUND; ++i)
	{
		hr = CreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			g_FileNameSound[i],		// �t�@�C���̖��O
			&m_pTexSound[i]);					// �ǂݍ��ރ������[
		if (FAILED(hr))
		{
			return hr;
		}
	}
	m_nCurSound = MASTER_SOUND;

	m_nSclCnt = 0; // �g�k�^�C�}�[
	m_bScaleUp = true; // �g�咆�t���O
	m_vColor = XMFLOAT3(1.0f, 0.0f, 0.0f); // �g�F

	return hr;
}



//=============================================================================
// �I������
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
// �X�V����
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
		// �T�E���h
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
// �`�揈��
//=============================================================================

void CPause::Draw()
{
	if (!m_bPause)return;

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);								// Z�o�b�t�@�X�V���Ȃ�

	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetUV(0.0f, 0.0f);
	if (!m_bSound)
	{
		// �v���[�g
		CPolygon::SetTexture(nullptr);
		CPolygon::SetSize(PLATE_WIDTH, PLATE_HEIGHT);
		CPolygon::SetPos(PLATE_POS_X - 300.0f, PLATE_POS_Y);
		CPolygon::SetColor(1.0f, 0.2f, 0.2f/* g_fCol, g_fCol, g_fCol */);
		CPolygon::SetAlpha(0.4f);
		CPolygon::Draw(pDeviceContext);

		// �L�[����
		CPolygon::SetSize(PLATE_WIDTH, PLATE_HEIGHT);
		CPolygon::SetPos(PLATE_POS_X - 300.0f, PLATE_POS_Y);
		CPolygon::SetFrameSize(1.0f, 1.0f);
		CPolygon::SetUV(0.0f, 0.0f);
		CPolygon::SetColor(1.0f, 1.0f, 1.0f);
		CPolygon::SetAlpha(1.0f);
		CPolygon::SetTexture(m_pTextureControl);
		CPolygon::Draw(pDeviceContext);


		//**************************************************************************


		// �E�v���[�g
		CPolygon::SetTexture(nullptr);
		CPolygon::SetSize(PLATE_WIDTH, PLATE_HEIGHT);
		CPolygon::SetPos(PLATE_POS_X + 300, PLATE_POS_Y);
		CPolygon::SetFrameSize(1.0f, 1.0f);
		CPolygon::SetUV(0.0f, 0.0f);
		CPolygon::SetColor(1.0f, 0.2f, 0.2f/* g_fCol, g_fCol, g_fCol */);
		CPolygon::SetAlpha(0.4f);
		CPolygon::Draw(pDeviceContext);


		// �I�����j���[
		CPolygon::SetSize(PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT);
		for (int i = 0; i < PAUSE_MENU_MAX; i++)
		{
			CPolygon::SetPos(PAUSE_MENU_POS_X + 300, PAUSE_MENU_POS_Y - i * PAUSE_MENU_INTERVAL);
			if (i == m_nSelectMenu)
			{
				// �I������Ă���
				CPolygon::SetColor(1.0f, 1.0f, 1.0f);
			}
			else
			{
				// �I������Ă��Ȃ�
				CPolygon::SetColor(0.8f, 0.8f, 0.8f);
			}
			CPolygon::SetAlpha(1.0f);

			// �e�N�X�`���̐ݒ�
			CPolygon::SetTexture(m_pTextureMenue[i]);

			// �|���S���̕`��
			CPolygon::Draw(pDeviceContext);
		}
	}
	else
	{
		// �v���[�g
		CPolygon::SetTexture(nullptr);
		CPolygon::SetPos(0.0f, -150.0f);
		CPolygon::SetSize(900.0f, 400.0f);
		CPolygon::SetColor(0.5f, 0.3f, 0.1f);
		CPolygon::SetAlpha(0.4f);
		CPolygon::Draw(pDeviceContext);


		// �T�E���h���j���[
		for (int i = 0; i < MAX_SOUND; ++i)
		{
			int fPosY = -10.0f;
			fPosY -= i * SELECT_SIZE_Y;
			fPosY -= i * SELECT_INTERVAL;
			//fPosY -= (SELECT_SIZE_Y - SELECT_INTERVAL) / 2.0f;
			if (i == m_nCurSound) {

				// �g
				CPolygon::SetTexture(nullptr);
				CPolygon::SetPos(-300.0f, fPosY + 6.0f);
				CPolygon::SetSize(200.0f + m_nSclCnt, 50.0f + m_nSclCnt);
				CPolygon::SetColor(m_vColor.x, m_vColor.y, m_vColor.z);
				CPolygon::SetAlpha(1.0f);
				CPolygon::Draw(pDeviceContext);

				// ���j���[�摜
				CPolygon::SetTexture(m_pTexSound[i]);
				CPolygon::SetPos(-300.0f, fPosY);
				CPolygon::SetSize(SELECT_SIZE_X, SELECT_SIZE_Y);
				CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
				CPolygon::Draw(pDeviceContext);
			}
			else
			{
				// ���j���[�摜
				CPolygon::SetTexture(m_pTexSound[i]);
				CPolygon::SetPos(-300.0f, fPosY);
				CPolygon::SetSize(SELECT_SIZE_X, SELECT_SIZE_Y);
				CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
				CPolygon::SetAlpha(1.0f);
				CPolygon::Draw(pDeviceContext);
			}


			// �o�[
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
				//                                 �����o���{�����[����BGM�Ȃ�ǂ�ł��悢
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
				//                                 �����o���{�����[����SE�Ȃ�ǂ�ł��悢
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
	SetZWrite(true);							// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);		// �A���t�@�u�����h����
}


////=============================================================================
//// �|�[�Y���j���[�̐ݒ�
////=============================================================================
//void SetPauseMenu(void)
//{
//	g_fCurve = 0.0f;
//}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
PAUSE_MENU CPause::GetPauseMenu(void)
{
	return m_nSelectMenu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void CPause::ResetPauseMenu(void)
{
	m_nSelectMenu = PAUSE_MENU_CONTINUE;
	CSound::Play(SE_MENUE);
	//SetPauseMenu();
}
