//=============================================================================
//
// �V�[�� �N���X���� [Scene.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include "Scene.h"
#include "Title.h"
#include "StageSelect.h"
#include "Game.h"
#include "Game_2.h"
#include "Game_3.h"
#include "GameObj.h"
#include "GameOver.h"
#include "GameClear.h"
#include "Tutorial.h"

// �ÓI�����o
CScene* CScene::m_pScene = nullptr;
CScene* CScene::m_pTop = nullptr;
std::vector<CEventManager*> CScene::m_pEventManager;

CPause CScene::m_Pause;
//CEventManager CScene::m_EventManager;

// �R���X�g���N�^
CScene::CScene()
{
	m_id = SCENE_NONE;
	m_pObj = nullptr;

	m_pBack = nullptr;
	m_pNext = m_pTop;
	if (m_pTop) {
		m_pTop->m_pBack = this;
	}
	m_pTop = this;

	m_pLight = nullptr;
	m_pCamera = nullptr;
	m_pEffekseerManager = nullptr;
	//m_pEventManager = nullptr;
}

// �f�X�g���N�^
CScene::~CScene()
{
	for (auto itr = m_pEventManager.begin(); itr != m_pEventManager.end(); ++itr) {
		if (*itr) {
			SAFE_DELETE(*itr);
		}
	}
	m_pEventManager.clear();

	if (m_pNext) {
		m_pNext->m_pBack = m_pBack;
	}
	if (m_pBack) {
		m_pBack->m_pNext = m_pNext;
	} else {
		m_pTop = m_pNext;
	}
	m_pBack = m_pNext = nullptr;
}

// ������
bool CScene::Init()
{
	m_camera.Init();
	SetCamera(&m_camera);
	m_lightOff.Init();
	m_lightOff.SetDir(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetLight(&m_lightOff);
	return true;
}

// �I������
void CScene::Fin()
{

	m_pLight = nullptr;
	m_pCamera = nullptr;
}

// ������
bool CScene::InitAll()
{
	new CTitle;
	new CStageSelect;
	new CGame;
	new CGame_2;
	new CGame_3;
	new CGameOver;
	new CGameClear;
	new CTutorial;

	//Change(SCENE_GAME_1);
	Change(SCENE_SELECT);
	//Change(SCENE_GAME_1);	
	//Change(SCENE_GAMECLEAR);
	return true;
}

// �I������
void CScene::FinAll()
{
	Change(SCENE_NONE);

	CScene* pNext;
	for (CScene* pScene = m_pTop; pScene; pScene = pNext) {
		pNext = pScene->m_pNext;
		delete pScene;
	}
}

// �X�V
void CScene::UpdateAll()
{
	if (m_pScene) {
		m_pScene->Update();
		if (m_pScene->m_pCamera) {
			m_pScene->m_pCamera->Update();
		}
	}
}

// �`��
void CScene::DrawAll()
{
	if (m_pScene) {
		if (m_pScene->m_pCamera) {
			m_pScene->m_pCamera->Clear();
		}
		m_pScene->Draw();
	}
}

// �ύX
void CScene::Change(EScene scene)
{
	if (m_pScene) {
		m_pScene->Fin();
		m_pScene = nullptr;
	}
	if (scene) {
		for (CScene* pScene = m_pTop; pScene; pScene = pScene->m_pNext) {
			if (pScene->m_id == scene) {
				m_pScene = pScene;
				break;
			}
		}
		if (m_pScene) {
			m_pScene->Init();
		}
	}
}

// �I�u�W�F�N�g����
CGameObj* CScene::Find(int id, CGameObj* pStart)
{
	CGameObj* pObj = (pStart) ? pStart->GetNext() : m_pObj;
	for (; pObj; pObj = pObj->GetNext()) {
		if (id < 0 || pObj->GetID() == id)
			return pObj;
	}
	return nullptr;
}

// �����ݒ�
void CScene::SetLight(CLight* pLight)
{
	if (pLight) {
		m_pLight = pLight;
	} else {
		m_pLight = &m_lightOff;
	}
}

// �J�����ݒ�
void CScene::SetCamera(CCamera* pCamera)
{
	if (pCamera) {
		m_pCamera = pCamera;
	} else {
		m_pCamera = &m_camera;
	}
}

CEventManager* CScene::GetEventManager(Event_ID ID)
{
	for (int i = 0; i < m_pEventManager.size(); ++i) {
		if (m_pEventManager[i]->GetID() == ID) {
			return m_pEventManager[i];
		}
	}

	return nullptr;

}


// �C�x���g�ǉ�
void CScene::SetEventManager(CEventManager* pEventManager)
{
	m_pEventManager.push_back(pEventManager);
}