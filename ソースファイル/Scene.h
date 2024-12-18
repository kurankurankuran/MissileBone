//=============================================================================
//
// シーン クラス定義 [Scene.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once
#include "Camera.h"
#include "Light.h"
#include "Pause.h"
#include "EventManager.h"
//#include "EffekseerManager.h"

#include <vector>

// 定数定義
enum EScene {
	SCENE_NONE = 0,		// (シーン無)
	SCENE_TITLE,		// タイトル画面
	SCENE_SELECT,		// ステージセレクト画面
	SCENE_TUTORIAL,		// チュートリアル画面
	SCENE_GAME_1,			// ゲーム画面
	SCENE_GAME_2,
	SCENE_GAME_3,
	SCENE_GAMEOVER,     // ゲームオーバー画面
	SCENE_GAMECLEAR,     // ゲームクリアー画面

	MAX_SCENE
};






// クラス定義
class CEffekseerManager;
class CGameObj;
class CScene {
protected:
	EScene m_id;
	CGameObj* m_pObj;
	
	static std::vector<CEventManager*> m_pEventManager;
	CEffekseerManager*           m_pEffekseerManager;
private:
	static CScene* m_pScene;
	static CScene* m_pTop;
	CScene* m_pBack;
	CScene* m_pNext;

	CCamera* m_pCamera;
	CCamera m_camera;
	CLight* m_pLight;
	CLight m_lightOff;

	static CPause m_Pause;
	
public:
	CScene();
	virtual ~CScene();

	virtual bool Init();
	virtual void Fin();
	virtual void Update() = 0;
	virtual void Draw() = 0;

	static bool InitAll();
	static void FinAll();
	static void UpdateAll();
	static void DrawAll();

	static void Change(EScene scene);

	CGameObj* GetObj() { return m_pObj; }
	void SetObj(CGameObj* pObj) { m_pObj = pObj; }
	CGameObj* Find(int id = -1, CGameObj* pStart = nullptr);

	CLight* GetLight() { return m_pLight; }
	void SetLight(CLight* pLight);
	CCamera* GetCamera() { return m_pCamera; }
	void SetCamera(CCamera* pCamera);

	static CScene* GetCurrent() { return m_pScene; }

	static CPause* GetPause() { return &m_Pause; }

	static void SetEventManager(CEventManager* pEventManager); //{ m_pEventManager pEventManager; }
	static CEventManager* GetEventManager(Event_ID);
	EScene GetID() { return m_id; }
};
