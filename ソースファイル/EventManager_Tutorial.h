// �`���[�g���A���C�x���g�t���O�Ǘ��N���X EventManager_Tutorial.h
#pragma once
#include "main.h"
#include "EventManager.h"
#include "Camera.h"

typedef enum {
	MOVE_TUTORIAL = 0,
	ATTACK_TUTORIAL,
	FIGHT_TUTORIAL,

	MAX_TUTORIAL
}TUTORIAL_TYPE;

namespace {
	const int MAX_TEXTURE = 3;
}

class CEnemyManager;
class CEventManager_Tutorial :public CEventManager
{
private:
	ID3D11ShaderResourceView*	m_pTex[MAX_TEXTURE];	// �|�[�Y���j���[�e�N�X�`��
	ID3D11ShaderResourceView*	m_pTexMoveTutorial;    
	ID3D11ShaderResourceView*	m_pTexWeaponTutorial;
	ID3D11ShaderResourceView*	m_pTexPauseTutorial;
	float m_fAlpha/*[MAX_TEXTURE]*/;
	CCamera* m_pCamera;
	CEnemyManager* m_pEnemyManager;
	bool m_bSetCameraMode; // �J�������[�h�ύX�t���O
	bool m_bEvent;         // �C�x���g�i�s�t���O
protected:	
	
public:
	CEventManager_Tutorial(CScene*);
	~CEventManager_Tutorial();

	HRESULT Init();
	void Fin();
	void Update();
	void Draw();
};

