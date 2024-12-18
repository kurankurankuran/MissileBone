#include "EventManager.h"



CEventManager::CEventManager(CScene* pScene)
{
	m_pScene = pScene;
}

CEventManager::~CEventManager()
{

}

void CEventManager::Fin()
{
	// テクスチャの解放
}

void CEventManager::Update()
{
	// フラグ管理
	if (!m_bEventPause)return;

	--m_nStopCnt;
	if (m_nStopCnt <= 0) {
		// ポーズ解除
		m_bEventPause = false;
	}

}

void CEventManager::Draw()
{

}

void CEventManager::StartEventType(int nType)
{
	if (/*m_nCurEventType == nType ||*/ nType < 0 || m_nStopTime.size() <= nType)
	{
		return;
	}


	m_nCurEventType = nType;
	m_nStopCnt = m_nStopTime[m_nCurEventType];
	m_bEventPause = true;

	m_nTexFade = FADE_TEXIN;
	/*m_bFadeIn = false;
	m_bFadeOut = false;*/
}

void CEventManager::StartPause(int nStopCnt)
{
	m_bEventPause = true;
	m_nStopCnt = nStopCnt; 
}