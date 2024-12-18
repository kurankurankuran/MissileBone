//=============================================================================
//
// �e��G�t�F�N�g���� [EffectManager.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once
#include "main.h"
#include "GameObj.h"
#include "Mesh.h"
#include <vector>
#include <list>
//*****************************************************************************
// �萔��`
//*****************************************************************************
enum EffectType {
	EFFECT_EXPLOSION = 0,	// �����G�t�F�N�g
	EFFECT_SMOKE,	// ��s�@�_�G�t�F�N�g
	
	MAX_EFFECTTYPE
};

struct EFFECT {
	bool        bLoop;  // ���[�v�̗L��
	XMFLOAT3	vPos;	// ���W
	XMFLOAT2	vScl;	// �g�k
	int			nType;	// �G�t�F�N�g���
	int			nAnim;	// �A�j���[�V�����Đ��ʒu
	int			nCount;	// �A�j���[�V���� �t���[����
	float		fAngle;	// �p�x
	float		fZoom;	// �g�嗦
	float       fAlpha; // ����
};



class CEffectManager : public CGameObj,public CMesh
{
private:
	std::vector<ID3D11ShaderResourceView*> m_pTexture;	// �e�N�X�`��
	static std::list<EFFECT>	m_Effect;	                // �G�t�F�N�g���

public:
	CEffectManager(CScene*);
	~CEffectManager();

	virtual HRESULT Init();		// ������
	virtual void Fin();			// �I������
	virtual void Update();		// �X�V
	virtual void Draw();		// �s���������`��

	static void StartEffect(EffectType EffectType, XMFLOAT3 pos, XMFLOAT2 scl, bool bLoop = false);
};
