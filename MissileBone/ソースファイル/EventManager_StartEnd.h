// �Q�[���J�n�A�I���C�x���g�t���O�Ǘ��N���X EventManager_StartEnd.h
#pragma once
#include "main.h"
#include "EventManager.h"
#include "Camera.h"

typedef enum {
	START_STARTEND = 0,
	WAVE_STARTEND,
	END_STARTEND,
	
	MAX_STARTEND
}STARTEND;

class CEnemyManager;
class CEventManager_StartEnd :public CEventManager
{
private:
	ID3D11ShaderResourceView*	m_pTex[MAX_STARTEND];	// �Q�[���X�^�[�g�A�Q�[���N���A�[�e�N�X�`��
	ID3D11ShaderResourceView*	m_pTextureNum;	// �����e�N�X�`��
	float m_fAlpha;
	bool m_bCreate;
	CEnemyManager* m_pEnemyManager;
	CCamera* m_pCamera;
	bool m_bSetCameraMode; // �J�������[�h�ύX�t���O
	
protected:	
	

public:
	CEventManager_StartEnd(CScene*);
	~CEventManager_StartEnd();

	HRESULT Init();
	void Fin();
	void Update();
	void Draw();
};

