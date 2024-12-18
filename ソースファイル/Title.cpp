//=============================================================================
//
// �^�C�g����ʃN���X���� [Title.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include "Title.h"
#include "Texture.h"
#include "Sound.h"
#include "Input.h"
#include "Polygon.h"
#include "Fade.h"



// �萔��`
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


	const int SCALE_RATE = 1;             // �g�k���x
	const float COLOR_RATE = 0.01f;       // �F�ω����x
	const float SELECT_INTERVAL = -100.0f; // �g�Ԃ̋����Ԋu
	const float SELECT_SIZE_X = 240.0f;   // �摜�̉��T�C�Y
	const float SELECT_SIZE_Y = 200.0f;   // �摜�̏c�T�C�Y
	const float FRAME_WIDTH = 20.0f;      // �g�̕��T�C�Y

	const int MAX_SCALE_UP = 40.0f;       // �c�����ő�
	const int MIN_SCALE_DOWN = -10.0f;    // �k�����ŏ�

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

	const float g_fVolume_Value = 0.02f; // ���ʕω���
	const float g_fMaxVolume = 10.0f; // �ő剹��

	const float g_fMaxSizeX_Bar = 500.0f;

	
}

// �R���X�g���N�^
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

// �f�X�g���N�^
CTitle::~CTitle()
{
}

// ������
bool CTitle::Init()
{
	
	if (!CScene::Init()) {
		return false;
	}

	// �G�t�F�N�V�A������
	//m_EffekseerManager.Init();

	m_nSclCnt = 0; // �g�k�^�C�}�[
	m_bScaleUp = true; // �g�咆�t���O
	m_vColor = XMFLOAT3(1.0f, 0.0f, 0.0f); // �g�F

	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	m_bStart = false;
	// �e�N�X�`���̓ǂݍ���
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

	// ���j���[�I���̃e�N�X�`���ǂݍ���
	for (int i = 0; i < MAX_MENUE; ++i)
	{
		hr = CreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			g_FileNameMenue[i],		// �t�@�C���̖��O
			&m_pTexMenue[i]);					// �ǂݍ��ރ������[
		if (FAILED(hr))
		{
			return hr;
		}
	}
	m_nTimer = 0;

	m_bOption = false;
	for (int i = 0; i < MAX_OPTION; ++i)
	{
		hr = CreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			g_FileNameOption[i],		// �t�@�C���̖��O
			&m_pTexOption[i]);					// �ǂݍ��ރ������[
		if (FAILED(hr))
		{
			return hr;
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
	//m_fVolume[MASTER_SOUND] = CSound::GetVolume(); // �T�E���h����
	//m_fVolume[BGM_SOUND] = CSound::GetVolume(); // �T�E���h����
	//m_fVolume[SE_SOUND] = CSound::GetVolume(); // �T�E���h����



	// BGM�Đ��J�n
	CSound::Play(BGM_TITLE);

	return true;
}

// �I������
void CTitle::Fin()
{
	// BGM�Đ���~
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

	// �G�t�F�N�V�A�I��
	//m_EffekseerManager.Uninit();

	CScene::Fin();
}

// �X�V
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


		// ���j���[�I��
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
				// �I�v�V����
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
				// �T�E���h
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

	// �G�t�F�N�V�A�X�V
	//m_EffekseerManager.Update();
}

// �`��
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

					// �g
					CPolygon::SetTexture(nullptr);
					CPolygon::SetPos(0.0f, fPosY + 6.0f);
					CPolygon::SetSize(250.0f + m_nSclCnt, 50.0f + m_nSclCnt);
					CPolygon::SetColor(m_vColor.x, m_vColor.y, m_vColor.z);
					CPolygon::SetAlpha(1.0f);
					CPolygon::Draw(pDC);

					// ���j���[�摜
					CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
					CPolygon::SetTexture(m_pTexMenue[i]);
					CPolygon::SetSize(SELECT_SIZE_X, SELECT_SIZE_Y);
					CPolygon::SetPos(0.0f, fPosY);
					CPolygon::Draw(pDC);
				}
				else
				{
					// �X�e�[�W�摜
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
				// �v���[�g
				CPolygon::SetTexture(nullptr);
				CPolygon::SetPos(0.0f, -40.0f);
				CPolygon::SetSize(400.0f, 200.0f);
				CPolygon::SetColor(0.5f, 0.3f, 0.1f);
				CPolygon::SetAlpha(0.4f);
				CPolygon::Draw(pDC);


				// �I�v�V�������j���[
				for (int i = 0; i < MAX_OPTION; ++i)
				{
					int fPosY = 0.0f;
					fPosY -= i * SELECT_SIZE_Y;
					fPosY -= i * SELECT_INTERVAL;
					//fPosY -= (SELECT_SIZE_Y - SELECT_INTERVAL) / 2.0f;

					if (i == m_nCurOption) {

						// �g
						CPolygon::SetTexture(nullptr);
						CPolygon::SetPos(0.0f, fPosY + 6.0f);
						CPolygon::SetSize(250.0f + m_nSclCnt, 50.0f + m_nSclCnt);
						CPolygon::SetColor(m_vColor.x, m_vColor.y, m_vColor.z);
						CPolygon::SetAlpha(1.0f);
						CPolygon::Draw(pDC);

						// ���j���[�摜
						CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
						CPolygon::SetTexture(m_pTexOption[i]);
						CPolygon::SetSize(SELECT_SIZE_X, SELECT_SIZE_Y);
						CPolygon::SetPos(0.0f, fPosY);
						CPolygon::Draw(pDC);
					}
					else
					{
						// �X�e�[�W�摜
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
				// �v���[�g
				CPolygon::SetTexture(nullptr);
				CPolygon::SetPos(0.0f, -150.0f);
				CPolygon::SetSize(900.0f, 400.0f);
				CPolygon::SetColor(0.5f, 0.3f, 0.1f);
				CPolygon::SetAlpha(0.4f);
				CPolygon::Draw(pDC);


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
						CPolygon::Draw(pDC);

						// ���j���[�摜
						CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
						CPolygon::SetTexture(m_pTexSound[i]);
						CPolygon::SetSize(SELECT_SIZE_X, SELECT_SIZE_Y);
						CPolygon::SetPos(-300.0f, fPosY);
						CPolygon::Draw(pDC);
					}
					else
					{
						// �X�e�[�W�摜
						CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
						CPolygon::SetTexture(m_pTexSound[i]);
						CPolygon::SetSize(SELECT_SIZE_X, SELECT_SIZE_Y);
						CPolygon::SetPos(-300.0f, fPosY);
						CPolygon::Draw(pDC);
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
						CPolygon::Draw(pDC);
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
						CPolygon::Draw(pDC);
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
						CPolygon::Draw(pDC);
					}
					break;
					}


				}
			}
		}
	}

	// �G�t�F�N�V�A�`��
	//m_EffekseerManager.Draw();
}
