//=============================================================================
//
// �^�C�g����ʃN���X��` [Title.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once
#include "Scene.h"
#include "Sound.h"
//#include "EffekseerManager.h"
typedef enum {
	STAGESELECT_MENUE = 0,
	OPTION_MENUE,
	//EXIT_MENUE,

	MAX_MENUE
}Title_Menue;

typedef enum {
	SOUND_OPTION = 0,
	EXIT_OPTION,
	
	MAX_OPTION
}Option_Menue;





// �N���X��`
class CTitle : public CScene
{
private:
	bool m_bStart;
	ID3D11ShaderResourceView* m_pTexTitle;
	ID3D11ShaderResourceView* m_pTexStart;
	ID3D11ShaderResourceView* m_pTexBG;
	int m_nTimer;
	bool m_bMenue;

	int m_nSclCnt; // �g�k�^�C�}�[
	bool m_bScaleUp; // �g�咆�t���O
	XMFLOAT3 m_vColor; // �g�F

	ID3D11ShaderResourceView* m_pTexMenue[MAX_MENUE]; // �X�e�[�W�ԍ�����
	Title_Menue m_nCurMenue;
	/*bool m_bInputUpNow;
	bool m_bInputUpOld;
	bool m_bInputDownNow;
	bool m_bInputDownOld;*/

	bool m_bOption;
	Option_Menue m_nCurOption;
	ID3D11ShaderResourceView* m_pTexOption[MAX_MENUE]; // �X�e�[�W�ԍ�����

	bool m_bSound;
	Sound_Menue m_nCurSound;
	ID3D11ShaderResourceView* m_pTexSound[MAX_SOUND]; // �X�e�[�W�ԍ�����

	//CEffekseerManager           m_EffekseerManager;
	

public:
	CTitle();
	virtual ~CTitle();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
};
