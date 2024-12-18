//=============================================================================
//
// �`���[�g���A���V�[�� �N���X���� [Tutorial.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include "Tutorial.h"
#include "Input.h"
#include "GameObj.h"
#include "Sound.h"
#include "Fade.h"
#include "DebugProc.h"
//#include "Enemy.h"
#include "Land.h"
//#include "Boss.h"
//#include "EnemyManager_1.h"
#include "EnemyManager_Tutorial.h"
#include "Pause.h"
#include "EventManager_Tutorial.h"
#include "EffectManager.h"
#include "EffekseerManager.h"

// �R���X�g���N�^
CTutorial::CTutorial() : CScene()
{
	m_id = SCENE_TUTORIAL;

	m_pPlayer = nullptr;
	m_nBound = 0;
}

// �f�X�g���N�^
CTutorial::~CTutorial()
{
}

// ������
bool CTutorial::Init()
{
	//m_bResult = false;

	// �|�[�Y������
	//m_Pause.Init();
	GetPause()->Init();

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
	// �G�t�F�N�g�Ǘ�
	new CEffectManager(this);
	// �v���C���[
	m_pPlayer = new CPlayer(this);
	m_pPlayer->SetStageSpher(m_pStageSphere);

	// �G�l�~�[
	CEnemyManager* pEnemyManager = new CEnemyManager_Tutorial(this);
	pEnemyManager->SetPlayer(m_pPlayer);
	pEnemyManager->SetTarget(m_pStageSphere);

	// �n�`���f��
	CLand* pLand = new CLand(this);
	//pLand->SetPlayer(m_pPlayer);
	
	// �S�I�u�W�F�N�g������
	if (FAILED(CGameObj::InitAll(m_pObj))) {
		return false;
	}

	// �C�x���g�Ǘ�������
	CEventManager_Tutorial* pEventManager = new CEventManager_Tutorial(this);
	m_pEventManager.push_back(pEventManager);
	for (int i = 0; i < m_pEventManager.size(); ++i) {
		m_pEventManager[i]->Init();
	}

	m_camera.SetPlayer(m_pPlayer);
	
	// BGM�Đ��J�n
	CSound::Play(BGM_TUTORIAL);

	return true;
}

// �I������
void CTutorial::Fin()
{
	// BGM�Đ���~
	CSound::Stop(BGM_TUTORIAL);

	// �S�I�u�W�F�N�g�I������
	CGameObj::FinAll(m_pObj);

	// �C�x���g�I��
	for (int i = 0; i < m_pEventManager.size(); ++i) {
		m_pEventManager[i]->Fin();
		SAFE_DELETE(m_pEventManager[i]);
	}
	m_pEventManager.clear();
	// �G�t�F�N�V�A�I��
	m_pEffekseerManager->Uninit();
	delete m_pEffekseerManager;


	// �|�[�Y�I��
	//m_Pause.Fin();
	GetPause()->Fin();


}

// �X�V
void CTutorial::Update()
{
	GetPause()->Update();

	

	// ���E��/���E�{�b�N�X�\���ؑ�
	if (CInput::GetKeyTrigger(VK_B)) {
		CGameObj* pObj = Find();
		m_nBound = (m_nBound + 3) % MAX_BOUND;
		//switch (m_nBound) {
		//case 0:	// ���E�\������
			while (pObj) {
				switch (pObj->GetID()) {
				case GOT_PLAYER:
				case GOT_ENEMY:
				case GOT_MISSILE:
					pObj->SetBSVisible((EBoundingShape)m_nBound);
					break;
				case GOT_STAGESPHERE:
					pObj->SetBSVisible(BOUND_SPHERE);
					break;
				}
				pObj = Find(-1, pObj);
			}
		
	}

	if (!GetPause()->GetPauseFlg()) {

		//GetEventManager()->Update();
		for (int i = 0; i < m_pEventManager.size(); ++i) {
			m_pEventManager[i]->Update();
		}
		bool bEventPause = false;
		for (int i = 0; i < m_pEventManager.size(); ++i) {
			if (m_pEventManager[i]->GetPause()) {
				bEventPause = true;
			}
		}

		if (/*!GetEventManager()->GetPause()*/!bEventPause) {
			// �S�L�����X�V
			CGameObj::UpdateAll(m_pObj);
			// �G�t�F�N�V�A�X�V
			m_pEffekseerManager->Update();
		}
	}


#ifdef _DEBUG
	/*if (!m_bResult) {
		if (CInput::GetKeyRelease(VK_BACK) ||
			CInput::GetKeyRelease(VK_DELETE)) {
			m_bResult = true;
			CFade::Out(SCENE_TITLE);
		}
	}*/
	if (CInput::GetKeyRelease(VK_P))
	{
		CFade::Out(SCENE_GAMECLEAR);
	}

#endif

	// �f�o�b�O�\��
	CDebugProc::Print("*** ����� ˮ��� ***\n");
	CDebugProc::Print("None/Sphere/AABB/OBB:[B]\n");
	CDebugProc::Print("\n");
}

// �`��
void CTutorial::Draw()
{
	// �S�L�����`��
	CGameObj::DrawAll(m_pObj);
	// �G�t�F�N�V�A�`��
	m_pEffekseerManager->Draw();

	//m_Pause.Draw();

	//GetEventManager()->Draw();
	for (int i = 0; i < m_pEventManager.size(); ++i) {
		m_pEventManager[i]->Draw();
	}

	GetPause()->Draw();
}
