//=============================================================================
//
// ゲーム クラス定義 [Game_3.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once
#include "Scene.h"
#include "Player.h"
#include "StageSphere.h"
//#include "Pause.h"

// クラス定義
class CEnemy;
class CGame_3 : public CScene
{
private:
	//bool m_bResult;

	CCamera m_camera;
	CLight m_light;
	CPlayer* m_pPlayer;
	int m_nBound;

	CStageSphere* m_pStageSphere;
	//CPause m_Pause;
public:
	CGame_3();
	virtual ~CGame_3();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	int GetBound() { return m_nBound; }

	//static CPause* GetPause() { return &m_Pause; }
};
