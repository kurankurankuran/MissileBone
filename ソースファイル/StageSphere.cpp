// ステージ球 [StageSphere.cpp]
#include "StageSphere.h"
#include "Scene.h"
#include "EffekseerManager.h"

namespace {
	EfkPath g_pszEfkPath = {
			u"Smoke_1.efk",
	};
}

CStageSphere::CStageSphere(CScene* pScene) : CGameObj(pScene)
{
	SetID(GOT_STAGESPHERE);
}

CStageSphere::~CStageSphere()
{

}


HRESULT CStageSphere::Init()
{
	HRESULT hr = S_OK;

	SetRadius(15000.0f);

	// エフェクシアファイルを読み込み
	if (!CEffekseerManager::Load(g_pszEfkPath.t_Path[0]))
	{
		return false;
	}

	m_nHandle = CEffekseerManager::Play(g_pszEfkPath.t_Path[0], true, 9999);
	CEffekseerManager::SetMatrix(m_nHandle, true,m_mWorld);
	
	return hr;
}

void CStageSphere::Fin()
{
	CEffekseerManager::Stop(m_nHandle, true);
}

void CStageSphere::Update()
{
	CGameObj::Update();

	CEffekseerManager::SetMatrix(m_nHandle, true,m_mWorld);
}