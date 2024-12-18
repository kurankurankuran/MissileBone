//=============================================================================
//
// �Q�[���N���A�[��ʃN���X��` [GameClear.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once
#include "Scene.h"

// �N���X��`
class CGameClear : public CScene
{
private:
	bool m_bStart;
	ID3D11ShaderResourceView* m_pTexGameClear;
	ID3D11ShaderResourceView* m_pTexThanks;
	ID3D11ShaderResourceView* m_pTexStart;
	ID3D11ShaderResourceView* m_pTexBG;
	int m_nTimer;

public:
	CGameClear();
	virtual ~CGameClear();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
};
