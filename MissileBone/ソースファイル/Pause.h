//=============================================================================
//
// �ꎞ��~���� [pause.h]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#pragma once
//#include "Scene.h"

#include "main.h"
#include "Sound.h"

// �|�[�Y���j���[
typedef enum 
{
	PAUSE_MENU_CONTINUE = 0,		// �R���e�B�j���[
	PAUSE_MENU_RETRY,					// ���g���C
	PAUSE_MENU_SOUND,
	PAUSE_MENU_QUIT,						// �N�C�b�g

	PAUSE_MENU_MAX
} PAUSE_MENU;

//const int NUM_PAUSE_MENU = 3;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//enum Sound_Menue;
class CPause
{
private:
	PAUSE_MENU m_nSelectMenu;	// �I�����j���[

	bool m_bPause;              // �|�[�Y�t���O

	/*bool m_bInputUpNow;
	bool m_bInputUpOld;
	bool m_bInputDownNow;
	bool m_bInputDownOld;*/

	ID3D11ShaderResourceView*	m_pTextureControl;	// �R���g���[���L�[�e�N�X�`��
	ID3D11ShaderResourceView*	m_pTextureMenue[PAUSE_MENU_MAX];	// �|�[�Y���j���[�e�N�X�`��




	int m_nSclCnt; // �g�k�^�C�}�[
	bool m_bScaleUp; // �g�咆�t���O
	XMFLOAT3 m_vColor; // �g�F
	bool m_bSound;
	Sound_Menue m_nCurSound;
	ID3D11ShaderResourceView* m_pTexSound[MAX_SOUND]; // �X�e�[�W�ԍ�����
public:
	CPause();
	~CPause();

	HRESULT Init();
	void Fin();
	void Update();
	void Draw();

	//void SetPauseMenu(void);
	PAUSE_MENU GetPauseMenu(void);
	void ResetPauseMenu(void);

	bool GetPauseFlg() { return m_bPause; }
};