//=============================================================================
//
// ���f�� �N���X���� [Model.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include "Model.h"
#include "Scene.h"
#include "Input.h"
#include "DebugProc.h"
#include "Explosion.h"

// 3D���f�� �t�@�C����
namespace {
	LPCSTR g_pszModelPath[MAX_MODEL] = {
		
		"data/model/x-35_fbx/x-35_fbx.FBX",
		"data/model/x-35_fbx/untitled.fbx",
		"data/model/AIM-120/AIM-120.fbx",          // �~�T�C��
		"data/model/jet/jet.fbx",                  // �G���f��
		"data/model/jet/jet.fbx",                  // ���i���邾���̓G���f��
		"data/model/jet/jet.fbx",                  // �{�X���f��
		"data/model/sphere.fbx",                   // �G���́i��~�j���f��
		"data/model/sky.fbx",                      // �X�J�C �h�[��
		"data/model/land.fbx",                     // �n�ʃ��f��
	};
}

// �ÓI�����o
CAssimpModel* CModel::m_pModels[MAX_MODEL] = { nullptr };

// �R���X�g���N�^
CModel::CModel(CScene* pScene) : CGameObj(pScene)
{
	m_pModel = nullptr;
	m_pExp = nullptr;
	m_uExpTimer = 0;
	m_ModelType = MAX_MODEL;
}

// �f�X�g���N�^
CModel::~CModel()
{
}

// �S���f���Ǎ�
HRESULT CModel::LoadAll()
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// Assimp�p�V�F�[�_������
	if (!CAssimpModel::InitShader(pDevice))
		return E_FAIL;

	// �S���f���Ǎ�
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

// �S���f�����
void CModel::ReleaseAll()
{
	// �S���f�����
	for (int i = 0; i < MAX_MODEL; ++i) {
		if (m_pModels[i]) {
			m_pModels[i]->Release();
			delete m_pModels[i];
			m_pModels[i] = nullptr;
		}
	}

	// Assimp�p�V�F�[�_�I������
	CAssimpModel::UninitShader();
}

// ������
HRESULT CModel::Init()
{
	HRESULT hr = CGameObj::Init();
	if (FAILED(hr))
		return hr;

	return hr;
}

// �I������
void CModel::Fin()
{
	CGameObj::Fin();

	// �����G�t�F�N�g�j��
	if (m_pExp) {
		m_pExp->Fin();
		delete m_pExp;
		m_pExp = nullptr;
	}
}

// �X�V
void CModel::Update()
{
	//CGameObj::Update();
	// �����G�t�F�N�g�X�V
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

// �`��
void CModel::Draw()
{
	if (!m_bVisible) return;
	if (m_pModel) {
	
		ID3D11DeviceContext* pDC = GetDeviceContext();
		m_pModel->Draw(pDC, GetWorld(), eOpacityOnly);

		
	}
}

// �����������`��
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

// ���f���ݒ�
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
	// �J�����ƌ�����ݒ�
	if (m_pModel && m_pScene) {
		m_pModel->SetCamera(m_pScene->GetCamera());
		m_pModel->SetLight(m_pScene->GetLight());
	}
	// ���E���擾
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



// Assimp���f���擾
CAssimpModel* CModel::GetAssimp(EModel model)
{
	if (model < 0 || model >= MAX_MODEL)
		return nullptr;
	return m_pModels[model];
}

// �����J�n
void CModel::StartExplosion()
{
	if (m_uExpTimer || !m_bVisible) return;
	SetVisible(false);
	m_pExp->Start(&GetWorld());
	m_uExpTimer = 3 * 60;
}
