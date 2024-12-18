//=============================================================================
//
// ゲーム クラス実装 [Game.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include "Game_3.h"
#include "Input.h"
#include "GameObj.h"
#include "Sound.h"
#include "Fade.h"
#include "DebugProc.h"
#include "Enemy.h"
#include "Land.h"
#include "Boss.h"
#include "EnemyManager_3.h"
#include "Pause.h"
#include "EventManager_StartEnd.h"
#include "EffectManager.h"
#include "EffekseerManager.h"

// コンストラクタ
CGame_3::CGame_3() : CScene()
{
	m_id = SCENE_GAME_3;

	m_pPlayer = nullptr;
	m_nBound = 0;
}

// デストラクタ
CGame_3::~CGame_3()
{
}

// 初期化
bool CGame_3::Init()
{
	//m_bResult = false;

	// ポーズ初期化
	//m_Pause.Init();
	GetPause()->Init();

	

	// オブジェクト生成
	m_camera.Init();
	m_camera.SetSkyDome(CModel::GetAssimp(MODEL_SKY));
	SetCamera(&m_camera);

	m_light.Init();
	SetLight(&m_light);
	// エフェクシア初期化
	m_pEffekseerManager = new CEffekseerManager(this);
	m_pEffekseerManager->Init();
	m_pEffekseerManager->SetCamera(&m_camera);
	
	// ステージ境界球
	m_pStageSphere = new CStageSphere(this);
	// エフェクト管理
	new CEffectManager(this);
	// プレイヤー
	m_pPlayer = new CPlayer(this);
	m_pPlayer->SetStageSpher(m_pStageSphere);

	
	


	// エネミー
	CEnemyManager* pEnemyManager = new CEnemyManager_3(this);
	pEnemyManager->SetPlayer(m_pPlayer);
	pEnemyManager->SetTarget(m_pStageSphere);

	// 地形モデル
	CLand* pLand = new CLand(this);
	//pLand->SetPlayer(m_pPlayer);
	
	// 全オブジェクト初期化
	if (FAILED(CGameObj::InitAll(m_pObj))) {
		return false;
	}

	// イベント管理初期化
	CEventManager_StartEnd* pEventManager = new CEventManager_StartEnd(this);
	m_pEventManager.push_back(pEventManager);
	for (int i = 0; i < m_pEventManager.size(); ++i) {
		m_pEventManager[i]->Init();
	}

	m_camera.SetPlayer(m_pPlayer);
	
	// BGM再生開始
	CSound::Play(BGM_GAME_3);

	return true;
}

// 終了処理
void CGame_3::Fin()
{
	// BGM再生停止
	CSound::Stop(BGM_GAME_3);

	// 全オブジェクト終了処理
	CGameObj::FinAll(m_pObj);

	// イベント終了
	for (int i = 0; i < m_pEventManager.size(); ++i) {
		m_pEventManager[i]->Fin();
		SAFE_DELETE(m_pEventManager[i]);
	}
	m_pEventManager.clear();
	// エフェクシア終了
	m_pEffekseerManager->Uninit();
	delete m_pEffekseerManager;
	// ポーズ終了
	//m_Pause.Fin();
	GetPause()->Fin();
}

// 更新
void CGame_3::Update()
{
	//m_Pause.Update();
	GetPause()->Update();

	// 境界球/境界ボックス表示切替
	if (CInput::GetKeyTrigger(VK_B)) {
		CGameObj* pObj = Find();
		m_nBound = (m_nBound + 3) % MAX_BOUND;
		//switch (m_nBound) {
		//case 0:	// 境界表示無し
			while (pObj) {
				switch (pObj->GetID()) {
				case GOT_PLAYER:
				case GOT_ENEMY:
				case GOT_BOSS:
				case GOT_MISSILE:
					/*pObj->SetBBoxVisible(false);
					pObj->SetBSphereVisible(false);*/
					pObj->SetBSVisible((EBoundingShape)m_nBound);
					break;
				case GOT_STAGESPHERE:
					pObj->SetBSVisible(BOUND_SPHERE);
					break;
				}
				pObj = Find(-1, pObj);
			}
		
	}

	if (!GetPause()->GetPauseFlg()) {
		// イベント管理更新

		//GetEventManager()->Update();
		for (int i = 0; i < m_pEventManager.size(); ++i) {
			m_pEventManager[i]->Update();
		}
		bool bEventPause = false;
		for (int i = 0; i < m_pEventManager.size(); ++i) {
			if (m_pEventManager[i]->GetPause()) {
				bEventPause = true;
			}
		}

		if (/*!GetEventManager()->GetPause()*/!bEventPause) {
			// 全キャラ更新
			CGameObj::UpdateAll(m_pObj);
			// エフェクシア更新
			m_pEffekseerManager->Update();
		}
		else if (GetEventManager(STARTEND_EVENT_ID)->GetEventType() == END_STARTEND)
		{
			// ボスのみ更新
			Find(GOT_BOSS)->Update();
		}
	}


#ifdef _DEBUG
	/*if (!m_bResult) {
		if (CInput::GetKeyRelease(VK_BACK) ||
			CInput::GetKeyRelease(VK_DELETE)) {
			m_bResult = true;
			CFade::Out(SCENE_TITLE);
		}
	}*/
	if (CInput::GetKeyRelease(VK_P))
	{
		CFade::Out(SCENE_GAMECLEAR);
	}

#endif

	// デバッグ表示
	CDebugProc::Print("*** ｷｮｳｶｲ ﾋｮｳｼﾞ ***\n");
	CDebugProc::Print("None/Sphere/AABB/OBB:[B]\n");
	CDebugProc::Print("\n");
}

// 描画
void CGame_3::Draw()
{
	// 全キャラ描画
	CGameObj::DrawAll(m_pObj);
	// エフェクシア描画
	m_pEffekseerManager->Draw();
	//GetEventManager()->Draw();
	for (int i = 0; i < m_pEventManager.size(); ++i) {
		m_pEventManager[i]->Draw();
	}


	//m_Pause.Draw();
	GetPause()->Draw();
}
