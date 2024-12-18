//=============================================================================
//
// モデル クラス実装 [Model.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include "Model.h"
#include "Scene.h"
#include "Input.h"
#include "DebugProc.h"
#include "Explosion.h"

// 3Dモデル ファイル名
namespace {
	LPCSTR g_pszModelPath[MAX_MODEL] = {
		
		"data/model/x-35_fbx/x-35_fbx.FBX",
		"data/model/x-35_fbx/untitled.fbx",
		"data/model/AIM-120/AIM-120.fbx",          // ミサイル
		"data/model/jet/jet.fbx",                  // 敵モデル
		"data/model/jet/jet.fbx",                  // 直進するだけの敵モデル
		"data/model/jet/jet.fbx",                  // ボスモデル
		"data/model/sphere.fbx",                   // 敵球体（停止）モデル
		"data/model/sky.fbx",                      // スカイ ドーム
		"data/model/land.fbx",                     // 地面モデル
	};
}

// 静的メンバ
CAssimpModel* CModel::m_pModels[MAX_MODEL] = { nullptr };

// コンストラクタ
CModel::CModel(CScene* pScene) : CGameObj(pScene)
{
	m_pModel = nullptr;
	m_pExp = nullptr;
	m_uExpTimer = 0;
	m_ModelType = MAX_MODEL;
}

// デストラクタ
CModel::~CModel()
{
}

// 全モデル読込
HRESULT CModel::LoadAll()
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// Assimp用シェーダ初期化
	if (!CAssimpModel::InitShader(pDevice))
		return E_FAIL;

	// 全モデル読込
	HRESULT hr = S_OK;
	for (int i = 0; i < MAX_MODEL; ++i) {
		SAFE_DELETE(m_pModels[i]);
		m_pModels[i] = new CAssimpModel();
		if (!m_pModels[i]->Load(pDevice, pDC, g_pszModelPath[i])) {
			hr = E_FAIL;
			break;
		}
	}
	return hr;
}

// 全モデル解放
void CModel::ReleaseAll()
{
	// 全モデル解放
	for (int i = 0; i < MAX_MODEL; ++i) {
		if (m_pModels[i]) {
			m_pModels[i]->Release();
			delete m_pModels[i];
			m_pModels[i] = nullptr;
		}
	}

	// Assimp用シェーダ終了処理
	CAssimpModel::UninitShader();
}

// 初期化
HRESULT CModel::Init()
{
	HRESULT hr = CGameObj::Init();
	if (FAILED(hr))
		return hr;

	return hr;
}

// 終了処理
void CModel::Fin()
{
	CGameObj::Fin();

	// 爆発エフェクト破棄
	if (m_pExp) {
		m_pExp->Fin();
		delete m_pExp;
		m_pExp = nullptr;
	}
}

// 更新
void CModel::Update()
{
	//CGameObj::Update();
	// 爆発エフェクト更新
	if (m_pExp) {
		if (m_uExpTimer) {
			--m_uExpTimer;
			if (!m_uExpTimer) {
				m_pExp->FinDeb();
				SetVisible();
			}
		}
		m_pExp->Update();
	}
}

// 描画
void CModel::Draw()
{
	if (!m_bVisible) return;
	if (m_pModel) {
	
		ID3D11DeviceContext* pDC = GetDeviceContext();
		m_pModel->Draw(pDC, GetWorld(), eOpacityOnly);

		
	}
}

// 半透明部分描画
void CModel::DrawAlpha()
{
	if (m_bVisible) {
		if (m_pModel) {
			ID3D11DeviceContext* pDC = GetDeviceContext();
			m_pModel->Draw(pDC, GetWorld(), eTransparentOnly);
		}
		CGameObj::DrawAlpha();
	}
	if (m_pExp) {
		m_pExp->DrawAlpha();
	}
}

// モデル設定
void CModel::SetModel(EModel model)
{

	if (m_pExp) {
		m_pExp->Fin();
		delete m_pExp;
		m_pExp = nullptr;
	}

	if (model < 0 || model >= MAX_MODEL) {
		m_pModel = nullptr;

		m_ModelType = MAX_MODEL;
	}
	else {
		m_pModel = m_pModels[model];
		m_ModelType = model;
	}
	// カメラと光源を設定
	if (m_pModel && m_pScene) {
		m_pModel->SetCamera(m_pScene->GetCamera());
		m_pModel->SetLight(m_pScene->GetLight());
	}
	// 境界情報取得
	if (m_pModel) {
		SetCenter(m_pModel->GetCenter());
		SetBBox(m_pModel->GetBBox());
		SetRadius(m_pModel->GetRadius());

		m_pExp = new CExplosion(m_pScene);
		m_pExp->SetModel(m_pModel);
		m_pExp->Init();
	}
	else {
		SetCenter(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetBBox(XMFLOAT3(0.5f, 0.5f, 0.5f));
		SetRadius(0.5f);
	}
}



// Assimpモデル取得
CAssimpModel* CModel::GetAssimp(EModel model)
{
	if (model < 0 || model >= MAX_MODEL)
		return nullptr;
	return m_pModels[model];
}

// 爆発開始
void CModel::StartExplosion()
{
	if (m_uExpTimer || !m_bVisible) return;
	SetVisible(false);
	m_pExp->Start(&GetWorld());
	m_uExpTimer = 3 * 60;
}
