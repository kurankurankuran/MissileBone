//=============================================================================
//
// �X�e�[�W�Z���N�g���� [StageSelect.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once

#include "Scene.h"
#include "StageSphere.h"

// �|�[�Y���j���[
typedef enum
{
	/* �X�e�[�W�ǉ��̏ꍇ���ɒǋL */
	TUTORIAL,
	STAGE_1,	
	STAGE_2,
	STAGE_3,
	//STAGE_4,
	//STAGE_5,

	MAX_STAGE_SELECT
} STAGE_MENU;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

class CStageSelect :public CScene
{
private:
	//ID3D11ShaderResourceView* m_pTexBG; // �w�i
	ID3D11ShaderResourceView* m_pTexStage[MAX_STAGE_SELECT]; // �X�e�[�W�ԍ�����

	STAGE_MENU m_nCurrentSelect;

	//bool m_bInputUpNow;
	//bool m_bInputUpOld;
	//bool m_bInputDownNow;
	//bool m_bInputDownOld;

	
	int m_nSclCnt; // �g�k�^�C�}�[
	bool m_bScaleUp; // �g�咆�t���O
	XMFLOAT3 m_vColor; // �g�F


	CCamera m_camera;
	CLight m_light;
	CStageSphere* m_pStageSphere;
public:

	CStageSelect();
	~CStageSelect();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	//void SetStageSelect(void);
	//STAGE_MENU GetStageSelect(void);
};