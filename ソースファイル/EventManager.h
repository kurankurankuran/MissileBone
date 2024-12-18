// �C�x���g�t���O�Ǘ��N���X(���ۃN���X) EventManager.h
#pragma once
#include "main.h"
//#include "Scene.h"
#include <vector>

// �C�x���gID
enum Event_ID{
	TUTORIAL_EVENT_ID = 0,
	STARTEND_EVENT_ID,
	WAVESTART_EVENT_ID,

	MAX_EVENT_ID
};

// �e�N�X�`���t�F�[�h���
enum TEX_FADE {
	FADE_TEXNONE = 0,
	FADE_TEXIN,
	FADE_TEXKEEP,
	FADE_TEXOUT,

	MAX_TEXFADE
};

class CScene;
class CEventManager
{
private:
	Event_ID m_Event_ID;

protected:
	CScene* m_pScene;		// �����V�[��

	int m_nCurEventType; // ���݂̐i�s���C�x���g
	//int m_nStopTime[MAX_TUTORIAL];
	std::vector<int> m_nStopTime;
	int m_nStopCnt;
	bool m_bEventPause; // �C�x���g�������|�[�Y�̃t���O 

	//bool m_bFadeIn;  // �C�x���g�J�n���Ƀt�F�[�h�C���A�E�g����摜�ȂǂɎg���t���O
	//bool m_bFadeOut;
	int m_nTexFade; // �e�N�X�`���t�F�[�h��� 
	int m_nTexDrawCnt; // �e�N�X�`���`�掞�ԃJ�E���^�[

public:
	CEventManager(CScene*);
	~CEventManager();

	virtual HRESULT Init() = 0;
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	void StartEventType(int nType);
	int GetEventType(void) { return m_nCurEventType; }
	bool GetPause() { return m_bEventPause; }

	void SetEvent_ID(Event_ID Id) { m_Event_ID = Id; }
	Event_ID GetID() { return m_Event_ID; }

	void StartPause(int nStopCnt);
};

