// �v���C���[ [Player.h]
#pragma once
#include "Model.h"
//#include "Missile.h"

#include "WeaponManager.h"

#include <vector>


//#include "Effect.h"
#include "Land.h"

const XMFLOAT3 PLAYER_POS = XMFLOAT3(0.0f, 0.0f, 0.0f);



enum EParts {
	PARTS_BODY = 0,
	PARTS_CANON,
	/*PARTS_MISSILE_1,
	PARTS_MISSILE_2,
	PARTS_MISSILE_3,
	PARTS_MISSILE_4,
	PARTS_MISSILE_5,
	PARTS_MISSILE_6,*/


	MAX_PARTS

};

enum EWepons {
	WEAPON_MISSILE = 0,
	WEAPON_MINE,
	WEAPON_MULTIMS,
	WEAPON_MACHINEGUN,

	MAX_WEAPON
};

// �p�[�c���
struct TParts {
	int nModel;  // ���f���ԍ�
	int nParent; // �e�p�[�c�ԍ�
	XMFLOAT3 vPos; // �I�t�Z�b�g�ʒu
	XMFLOAT3 vAngle; // �p�x
	XMFLOAT4X4 mWorld; // ���[���h�ϊ�
	bool bUnit; // �����t���O
};

struct TPartsInfo {
	int nModel;
	int nParent;
	XMFLOAT3 vPos;
	XMFLOAT3 vAngle;
	bool bUnit;
};

class CWeaponManager;
class CPlayer : public CModel
{
private:
	float m_fCanonAngle;

	TParts m_parts[MAX_PARTS];

	int m_nWeaponType;  // ������
	CWeaponManager* m_pWeaponManager; // ����Ǘ��N���X�|�C���^

	int m_nEffectCnt;
	//CEffect m_Effect;

	CGameObj* m_pStageSpher; // �X�e�[�W��

	bool m_bDash;   // �_�b�V�����t���O
	int m_nDashCnt; // �_�b�V���J�E���g

	ID3D11ShaderResourceView*	m_pTextureWeaponType[MAX_WEAPON];	// �����e�N�X�`��
	ID3D11ShaderResourceView*	m_pTextureNum;	// �����e�N�X�`��

	CLand* m_pLand;
public:
	CPlayer(CScene* pScene); // �R���X�g���N�^
	virtual ~CPlayer();		 // �f�X�g���N�^

	virtual HRESULT Init();	 // ������
	virtual void Fin();		 // �I������
	virtual void Update();	 // �X�V
	virtual void Draw();	 // �`��

	TParts* GetParts() { return m_parts; }
	
	void ChangeWeapon();
	void SetStageSpher(CGameObj* pObj) { m_pStageSpher = pObj; }

	void CollosionLand(XMFLOAT3* vMove);
};
