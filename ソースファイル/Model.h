//=============================================================================
//
// ���f�� �N���X��` [Model.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once
#include "GameObj.h"
#include "AssimpModel.h"

#define PLAYER_BS_RAD (200.0f)
#define PLAYER_CENTER_X (0.0f)
#define PLAYER_CENTER_Y (0.0f)
#define PLAYER_CENTER_Z (0.0f)

#define ENEMY_BS_RAD (50.0f)
#define ENEMY_CENTER_X (0.0f)
#define ENEMY_CENTER_Y (0.0f)
#define ENEMY_CENTER_Z (0.0f)

#define MISSILE_BS_RAD (50.0f)
#define MISSILE_CENTER_X (0.0f)
#define MISSILE_CENTER_Y (0.0f)
#define MISSILE_CENTER_Z (0.0f)

enum EModel {
	
	MODEL_PLAYER = 0,
	MODEL_PLAYER_CANON,
	MODEL_MISSILE,		// �~�T�C��
	MODEL_ENEMY,		// �G���f��
	MODEL_ENEMY_STRATE,	// ���i���邾���̓G���f��
	MODEL_BOSS,		    // �{�X���f��
	MODEL_SPHERE,		// �G���́i��~�j���f��
	MODEL_SKY,		    // �X�J�C �h�[��
	MODEL_LAND,			// �n�ʃ��f��

	MAX_MODEL
};

typedef struct EfkPath {
	const char16_t t_Path[][256];
};

class CExplosion;
class CModel : public CGameObj
{
private:
	static CAssimpModel* m_pModels[MAX_MODEL];
	CAssimpModel* m_pModel;

	EModel m_ModelType;
protected:
	CExplosion* m_pExp;	// �����G�t�F�N�g
	UINT m_uExpTimer;	// �������^�C�}�[
	EfkPath* m_EfkHandle; //  �}�l�[�W���[����Efk�t�@�C�����g�p���邽�߂̃n���h�����󂯎��p
public:
	CModel(CScene* pScene);
	virtual ~CModel();

	static HRESULT LoadAll();
	static void ReleaseAll();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
	virtual void DrawAlpha();

	void SetModel(EModel model);
	EModel GetModelType(void) { return m_ModelType; }
	//void SetModel(EModel model, float fRadius);
	static CAssimpModel* GetAssimp(EModel model);
	
	void SetCamera(CCamera* camera) { m_pModel->SetCamera(camera); }
	void SetLight(CLight* light) { m_pModel->SetLight(light); }

	void StartExplosion();

	/*void SetEfkHandle(const char16_t* pHandle) { m_EfkHandle = pHandle; }*/
	void SetEfkHandle(EfkPath* pHandle) { m_EfkHandle = pHandle; }
};
