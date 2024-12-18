//=============================================================================
//
// ゲーム クラス定義 [Game.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once
#include "Scene.h"
#include "Player.h"
#include "StageSphere.h"
//#include "Pause.h"
//#include "EventManager.h"

// クラス定義
class CEnemy;
class CTutorial : public CScene
{
private:
	//bool m_bResult;

	CCamera m_camera;
	CLight m_light;
	CPlayer* m_pPlayer;
	int m_nBound;

	CStageSphere* m_pStageSphere;
	//CPause m_Pause;
	//CEventManager m_EventManager;
public:
	CTutorial();
	virtual ~CTutorial();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	int GetBound() { return m_nBound; }

	
};
