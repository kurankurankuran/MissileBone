#include "EffekseerManager.h"
#include "main.h"
#include <stdio.h>
#include <string>
//#include <windows.h>

#include <XAudio2.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "xaudio2.lib")

//#pragma comment(lib, "Effekseer")
//#pragma comment(lib, "EffekseerRendererDX11")
//#pragma comment(lib, "EffekseerSoundXAudio2")

#define	EFK_CAM_POS_X		(0.0f)					// �J�����̎��_�����ʒu(X���W)
#define	EFK_CAM_POS_Y		(1300.0f)				// �J�����̎��_�����ʒu(Y���W)
#define	EFK_CAM_POS_Z		(-3000.0f)				// �J�����̎��_�����ʒu(Z���W)
#define EFK_MAX_NUM         (8000)                  // �ő吔
#define EFFECT_TEXTUREDIR	L"data/Efk/efk/"	// �e�N�X�`���t�H���_��

#include "Scene.h"
#include "GameObj.h"
#include "Player.h"

// �ÓI�����o
EffekseerRendererDX11::RendererRef CEffekseerManager::m_Renderer = nullptr;
Effekseer::ManagerRef CEffekseerManager::m_Manager = nullptr;
std::unordered_map<const char16_t*, Effekseer::EffectRef> CEffekseerManager::m_RegisteredEffectRef;
std::unordered_map</*::Effekseer::Handle*/int, EfkObj> CEffekseerManager::m_RegisteredEffectsLoop;
std::unordered_map</*::Effekseer::Handle*/int, EfkObj> CEffekseerManager::m_RegisteredEffectsNoLoop;
namespace {
	
	/*const char16_t g_pszRootPath[] = {
		u"data/Efk/",
	};*/
   
}

//*******************************************************************************
//
//
//
//*******************************************************************************







//*******************************************************************************
//
//
//
//*******************************************************************************
CEffekseerManager::CEffekseerManager(CScene* pScene)
{
	m_xa2 = nullptr;
	m_xa2_master = nullptr;
	m_pCamera = nullptr;
}

CEffekseerManager::~CEffekseerManager()
{
}

void CEffekseerManager::Init()
{
	// Effekseer�̃I�u�W�F�N�g�̓X�}�[�g�|�C���^�ŊǗ������B�ϐ����Ȃ��Ȃ�Ǝ����I�ɍ폜�����B
	// �������ACOM�̏I���O�ɍ폜�����Ƃ��������Ȃ�̂ŁA�X�R�[�v��ǉ�����B
	
	// Create a renderer of effects
	// �G�t�F�N�g�̃����_���[�̍쐬
	ID3D11Device* d3D11Device = GetDevice();
	ID3D11DeviceContext* d3D11DeviceContext = GetDeviceContext();
	m_Renderer = ::EffekseerRendererDX11::Renderer::Create(d3D11Device, d3D11DeviceContext, EFK_MAX_NUM);

	// Create a manager of effects
	// �G�t�F�N�g�̃}�l�[�W���[�̍쐬
	m_Manager = ::Effekseer::Manager::Create(EFK_MAX_NUM);
	
	// ������W�n��
	m_Manager->SetCoordinateSystem(::Effekseer::CoordinateSystem::LH);
	// Specify a projection matrix
	// ���e�s���ݒ�
	m_Renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovLH(
		VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z));
	
	// Sprcify rendering modules
		// �`�惂�W���[���̐ݒ�
	m_Manager->SetSpriteRenderer(m_Renderer->CreateSpriteRenderer());
	m_Manager->SetRibbonRenderer(m_Renderer->CreateRibbonRenderer());
	m_Manager->SetRingRenderer(m_Renderer->CreateRingRenderer());
	m_Manager->SetTrackRenderer(m_Renderer->CreateTrackRenderer());
	m_Manager->SetModelRenderer(m_Renderer->CreateModelRenderer());

	// Specify a texture, model, curve and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	m_Manager->SetTextureLoader(m_Renderer->CreateTextureLoader());
	m_Manager->SetModelLoader(m_Renderer->CreateModelLoader());
	m_Manager->SetMaterialLoader(m_Renderer->CreateMaterialLoader());
	m_Manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	// Specify sound modules
	// �T�E���h���W���[���̐ݒ�
	// Initialize COM
    // Initialize XAudio
	XAudio2Create(&m_xa2);
	m_xa2->CreateMasteringVoice(&m_xa2_master);
	m_Sound = ::EffekseerSound::Sound::Create(m_xa2, 16, 16);

	// Specify a metho to play sound from an instance of sound
	// ���Đ��p�C���X�^���X����Đ��@�\���w��
	m_Manager->SetSoundPlayer(m_Sound->CreateSoundPlayer());

	// Specify a sound data loader
	// It can be extended by yourself. It is loaded from a file on now.
	// �T�E���h�f�[�^�̓Ǎ��@�\��ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	m_Manager->SetSoundLoader(m_Sound->CreateSoundLoader());

	// Specify a position of view
	// ���_�ʒu���m��
	m_Position = ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f);	

}

void CEffekseerManager::Uninit()
{
	// �}�b�v�̔j��
	m_RegisteredEffectRef.clear();
	m_RegisteredEffectsLoop.clear();
	m_RegisteredEffectsNoLoop.clear();

	//***************************************:
	// Dispose the manager
	// �}�l�[�W���[�̔j��
	m_Manager.Reset();

	
	// Dispose the renderer
	// �����_���[�̔j��
	m_Renderer.Reset();

	// Dispose the sound
	// �T�E���h�̔j��
	m_Sound.Reset();
	
    // XAudio2�̉��
	if (m_xa2_master != nullptr)
	{
		m_xa2_master->DestroyVoice();
		m_xa2_master = nullptr;
	}
	ES_SAFE_RELEASE(m_xa2);
}

void CEffekseerManager::Update()
{
	for (auto itr = m_RegisteredEffectsLoop.begin(); itr != m_RegisteredEffectsLoop.end(); ++itr) {
		int* pCnt = &itr->second.t_nCnt;
		++(*pCnt);
		// Update a time
    	// ���Ԃ��X�V����
		if (itr->second.t_nTime < *pCnt)
		{
			// �G�t�F�N�g�ɃZ�b�g����Ă���s����擾
			::Effekseer::Matrix43 Mat = m_Manager->GetMatrix(itr->second.t_Handle);

			// �G�t�F�N�g�̏I��
			m_Manager->StopEffect(itr->second.t_Handle);
			// �G�t�F�N�g�̍Đ�
			itr->second.t_Handle = m_Manager->Play(m_RegisteredEffectRef[itr->second.t_szPath], 0, 0, 0);
			
			// �G�t�F�N�g�ɍs����Z�b�g
			m_Manager->SetMatrix(itr->second.t_Handle, Mat);

			// �J�E���g�����Z�b�g
			*pCnt -= itr->second.t_nTime;
		}
		m_Renderer->SetTime(*pCnt / 60.0f);
	
	}
	for (auto itr = m_RegisteredEffectsNoLoop.begin(); itr != m_RegisteredEffectsNoLoop.end(); ) {
		int* pCnt = &itr->second.t_nCnt;
		++(*pCnt);
		// Update a time
		// ���Ԃ��X�V����
		m_Renderer->SetTime(*pCnt / 60.0f);
		if (itr->second.t_nTime < *pCnt)
		{
			// �G�t�F�N�g�̒�~
			m_Manager->StopEffect(itr->second.t_Handle);
			itr = m_RegisteredEffectsNoLoop.erase(itr);
			
		}
		else
		{
			++itr;
		}
	}

	// Update the manager
	// �}�l�[�W���[�̍X�V
	m_Manager->Update();

	
}



void CEffekseerManager::Draw()
{
	// �J�����X�V
	UpdateCameraMatrix();


	// Begin to rendering effects
		// �G�t�F�N�g�̕`��J�n�������s���B
	m_Renderer->BeginRendering();

	for (auto itr = m_RegisteredEffectsLoop.begin(); itr != m_RegisteredEffectsLoop.end(); ++itr) {
			
		// Render effects
		// �G�t�F�N�g�̕`����s���B
		m_Manager->DrawHandle(itr->second.t_Handle);
	}

	for (auto itr = m_RegisteredEffectsNoLoop.begin(); itr != m_RegisteredEffectsNoLoop.end(); ++itr) {

		// Render effects
		// �G�t�F�N�g�̕`����s���B
		m_Manager->DrawHandle(itr->second.t_Handle);
	}



	// Finish to rendering effects
	// �G�t�F�N�g�̕`��I���������s���B
	m_Renderer->EndRendering();
}

void CEffekseerManager::SetMatrix(/*Effekseer::Handle _Handle*/int nIdx,bool bLoop, XMFLOAT4X4& world)
{
	Effekseer::Matrix43 Mat;
	Mat.Value[0][0] = world._11;
	Mat.Value[0][1] = world._12;
	Mat.Value[0][2] = world._13;
	Mat.Value[1][0] = world._21;
	Mat.Value[1][1] = world._22;
	Mat.Value[1][2] = world._23;
	Mat.Value[2][0] = world._31;
	Mat.Value[2][1] = world._32;
	Mat.Value[2][2] = world._33;
	Mat.Value[3][0] = world._41;
	Mat.Value[3][1] = world._42;
	Mat.Value[3][2] = world._43;

	if (bLoop)
	{
		m_Manager->SetMatrix(m_RegisteredEffectsLoop[nIdx].t_Handle, Mat);
	}
	else
	{
		m_Manager->SetMatrix(m_RegisteredEffectsNoLoop[nIdx].t_Handle, Mat);
	}
	//m_Manager->SetMatrix(_Handle, Mat);
	//m_Manager->SetBaseMatrix(_Handle, Mat);
}


// Effekseer�̈ʒu�Z�b�g
void CEffekseerManager::SetLocation(Effekseer::Handle _Handle, XMFLOAT3 vPos)
{
	Effekseer::Vector3D v3Pos;
	v3Pos.X = vPos.x;
	v3Pos.Y = vPos.x;
	v3Pos.Z = vPos.x;

	m_Manager->SetLocation(_Handle, v3Pos);
}

// Effekseer�̊g�k�Z�b�g
void CEffekseerManager::SetScale(Effekseer::Handle _Handle, XMFLOAT3 vScl)
{
	
	m_Manager->SetScale(_Handle, vScl.x, vScl.y, vScl.z);
}

// Effekseer�̉�]�Z�b�g
void CEffekseerManager::SetRotation(Effekseer::Handle _Handle, XMFLOAT3 vRot)
{
	m_Manager->SetRotation(_Handle, vRot.x, vRot.y, vRot.z);
}

void CEffekseerManager::UpdateCameraMatrix()
{
	XMFLOAT3 vTarget = m_pCamera->GetTargetPos();
	m_Position.X = m_pCamera->GetPos().x;
	m_Position.Y = m_pCamera->GetPos().y;
	m_Position.Z = m_pCamera->GetPos().z;
	XMFLOAT3 vUp = m_pCamera->GetUpVector();
	// Specify a camera matrix
	// �J�����s���ݒ�
	m_Renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(
			m_Position,
			::Effekseer::Vector3D(vTarget.x, vTarget.y, vTarget.z),
			::Effekseer::Vector3D(vUp.x, vUp.y, vUp.z)));
	
	
	//m_Renderer->SetRenderMode(::Effekseer::RenderMode::Wireframe);
}

//// Effekseer���J�����v���W�F�N�V�����}�g���N�X�X�V
//void CEffekseerManager::SetProjMatrix(XMFLOAT4X4& ProjMatrix)
//{
//	m_pCamera->GetProjMatrix();
//	
//	Effekseer::Matrix43 Mat;
//	Mat.Value[0][0] = world._11;
//	Mat.Value[0][1] = world._12;
//	Mat.Value[0][2] = world._13;
//	Mat.Value[1][0] = world._21;
//	Mat.Value[1][1] = world._22;
//	Mat.Value[1][2] = world._23;
//	Mat.Value[2][0] = world._31;
//	Mat.Value[2][1] = world._32;
//	Mat.Value[2][2] = world._33;
//	Mat.Value[3][0] = world._41;
//	Mat.Value[3][1] = world._42;
//	Mat.Value[3][2] = world._43;
//
//	m_Renderer->SetProjectionMatrix()
//}
//

// Efk�t�@�C���ǂݍ���
bool CEffekseerManager::Load(const char16_t* path)
{
	auto itr = m_RegisteredEffectRef.find(path);
	if (itr != m_RegisteredEffectRef.end()) {
		// �ݒ肳��Ă���ꍇ�̏���


		return true;
	}
	else {
		LPCWSTR _path = (LPCWSTR)path;
		wchar_t BasePath[256];
			lstrcpyW(BasePath, EFFECT_TEXTUREDIR);
		int	nLen = lstrlenW(BasePath);
		lstrcpyW(&BasePath[nLen],
			_path);

		//const char16_t directli[] = { u16str_1 };
		// �ݒ肳��Ă��Ȃ��ꍇ�̏���
		m_RegisteredEffectRef[path] = Effekseer::Effect::Create(m_Manager, (const char16_t*)BasePath);
		if (!(&m_RegisteredEffectRef[path]))
		{
			return false;
		}
		return true;
	}
}

// Efk�t�@�C���Đ�
/*::Effekseer::Handle*/int CEffekseerManager::Play(const char16_t* path, bool bLoop, int32_t nTime)
{
	auto itr = m_RegisteredEffectRef.find(path);
	if (itr != m_RegisteredEffectRef.end()) {
		// �ݒ肳��Ă���ꍇ�̏���
		::Effekseer::Handle _Handle = m_Manager->Play(m_RegisteredEffectRef[path], 0, 0, 0);
		
		if (bLoop)
		{
			int nIdx = FindRegisterIdx(m_RegisteredEffectsLoop, _Handle);
			m_RegisteredEffectsLoop[nIdx] = EfkObj(path, _Handle, nTime);
			return nIdx;
		}
		else
		{
			int nIdx = FindRegisterIdx(m_RegisteredEffectsNoLoop, _Handle);
			m_RegisteredEffectsNoLoop[nIdx] = EfkObj(path, _Handle, nTime);
			return nIdx;
		}
		/*return _Handle;*/
	}
	else {
		// �ݒ肳��Ă��Ȃ��ꍇ�̏���
		return -1;
	}
}

void CEffekseerManager::Stop(/*Effekseer::Handle _Handle*/int nIdx, bool bLoop)
{
	if (bLoop)
	{
		// �G�t�F�N�g�̒�~
		m_Manager->StopEffect(m_RegisteredEffectsLoop[nIdx].t_Handle);
		m_RegisteredEffectsLoop.erase(nIdx);
	}
	else
	{
		// �G�t�F�N�g�̒�~
		m_Manager->StopEffect(m_RegisteredEffectsNoLoop[nIdx].t_Handle);
		m_RegisteredEffectsNoLoop.erase(nIdx);
	}

	
}

// Register�̋󂢂Ă���C���f�b�N�X�����p
int CEffekseerManager::FindRegisterIdx(std::unordered_map<int, EfkObj> map, int Idx)
{
	if (map.find(Idx) == map.end())
	{
		return Idx;
	}
	else
	{
		return FindRegisterIdx(map, Idx + 1);
	}
}


