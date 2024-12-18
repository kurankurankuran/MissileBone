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

#define	EFK_CAM_POS_X		(0.0f)					// カメラの視点初期位置(X座標)
#define	EFK_CAM_POS_Y		(1300.0f)				// カメラの視点初期位置(Y座標)
#define	EFK_CAM_POS_Z		(-3000.0f)				// カメラの視点初期位置(Z座標)
#define EFK_MAX_NUM         (8000)                  // 最大数
#define EFFECT_TEXTUREDIR	L"data/Efk/efk/"	// テクスチャフォルダ名

#include "Scene.h"
#include "GameObj.h"
#include "Player.h"

// 静的メンバ
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
	// Effekseerのオブジェクトはスマートポインタで管理される。変数がなくなると自動的に削除される。
	// ただし、COMの終了前に削除されるとおかしくなるので、スコープを追加する。
	
	// Create a renderer of effects
	// エフェクトのレンダラーの作成
	ID3D11Device* d3D11Device = GetDevice();
	ID3D11DeviceContext* d3D11DeviceContext = GetDeviceContext();
	m_Renderer = ::EffekseerRendererDX11::Renderer::Create(d3D11Device, d3D11DeviceContext, EFK_MAX_NUM);

	// Create a manager of effects
	// エフェクトのマネージャーの作成
	m_Manager = ::Effekseer::Manager::Create(EFK_MAX_NUM);
	
	// 左手座標系に
	m_Manager->SetCoordinateSystem(::Effekseer::CoordinateSystem::LH);
	// Specify a projection matrix
	// 投影行列を設定
	m_Renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovLH(
		VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z));
	
	// Sprcify rendering modules
		// 描画モジュールの設定
	m_Manager->SetSpriteRenderer(m_Renderer->CreateSpriteRenderer());
	m_Manager->SetRibbonRenderer(m_Renderer->CreateRibbonRenderer());
	m_Manager->SetRingRenderer(m_Renderer->CreateRingRenderer());
	m_Manager->SetTrackRenderer(m_Renderer->CreateTrackRenderer());
	m_Manager->SetModelRenderer(m_Renderer->CreateModelRenderer());

	// Specify a texture, model, curve and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	m_Manager->SetTextureLoader(m_Renderer->CreateTextureLoader());
	m_Manager->SetModelLoader(m_Renderer->CreateModelLoader());
	m_Manager->SetMaterialLoader(m_Renderer->CreateMaterialLoader());
	m_Manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	// Specify sound modules
	// サウンドモジュールの設定
	// Initialize COM
    // Initialize XAudio
	XAudio2Create(&m_xa2);
	m_xa2->CreateMasteringVoice(&m_xa2_master);
	m_Sound = ::EffekseerSound::Sound::Create(m_xa2, 16, 16);

	// Specify a metho to play sound from an instance of sound
	// 音再生用インスタンスから再生機能を指定
	m_Manager->SetSoundPlayer(m_Sound->CreateSoundPlayer());

	// Specify a sound data loader
	// It can be extended by yourself. It is loaded from a file on now.
	// サウンドデータの読込機能を設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	m_Manager->SetSoundLoader(m_Sound->CreateSoundLoader());

	// Specify a position of view
	// 視点位置を確定
	m_Position = ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f);	

}

void CEffekseerManager::Uninit()
{
	// マップの破棄
	m_RegisteredEffectRef.clear();
	m_RegisteredEffectsLoop.clear();
	m_RegisteredEffectsNoLoop.clear();

	//***************************************:
	// Dispose the manager
	// マネージャーの破棄
	m_Manager.Reset();

	
	// Dispose the renderer
	// レンダラーの破棄
	m_Renderer.Reset();

	// Dispose the sound
	// サウンドの破棄
	m_Sound.Reset();
	
    // XAudio2の解放
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
    	// 時間を更新する
		if (itr->second.t_nTime < *pCnt)
		{
			// エフェクトにセットされている行列を取得
			::Effekseer::Matrix43 Mat = m_Manager->GetMatrix(itr->second.t_Handle);

			// エフェクトの終了
			m_Manager->StopEffect(itr->second.t_Handle);
			// エフェクトの再生
			itr->second.t_Handle = m_Manager->Play(m_RegisteredEffectRef[itr->second.t_szPath], 0, 0, 0);
			
			// エフェクトに行列をセット
			m_Manager->SetMatrix(itr->second.t_Handle, Mat);

			// カウントをリセット
			*pCnt -= itr->second.t_nTime;
		}
		m_Renderer->SetTime(*pCnt / 60.0f);
	
	}
	for (auto itr = m_RegisteredEffectsNoLoop.begin(); itr != m_RegisteredEffectsNoLoop.end(); ) {
		int* pCnt = &itr->second.t_nCnt;
		++(*pCnt);
		// Update a time
		// 時間を更新する
		m_Renderer->SetTime(*pCnt / 60.0f);
		if (itr->second.t_nTime < *pCnt)
		{
			// エフェクトの停止
			m_Manager->StopEffect(itr->second.t_Handle);
			itr = m_RegisteredEffectsNoLoop.erase(itr);
			
		}
		else
		{
			++itr;
		}
	}

	// Update the manager
	// マネージャーの更新
	m_Manager->Update();

	
}



void CEffekseerManager::Draw()
{
	// カメラ更新
	UpdateCameraMatrix();


	// Begin to rendering effects
		// エフェクトの描画開始処理を行う。
	m_Renderer->BeginRendering();

	for (auto itr = m_RegisteredEffectsLoop.begin(); itr != m_RegisteredEffectsLoop.end(); ++itr) {
			
		// Render effects
		// エフェクトの描画を行う。
		m_Manager->DrawHandle(itr->second.t_Handle);
	}

	for (auto itr = m_RegisteredEffectsNoLoop.begin(); itr != m_RegisteredEffectsNoLoop.end(); ++itr) {

		// Render effects
		// エフェクトの描画を行う。
		m_Manager->DrawHandle(itr->second.t_Handle);
	}



	// Finish to rendering effects
	// エフェクトの描画終了処理を行う。
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


// Effekseerの位置セット
void CEffekseerManager::SetLocation(Effekseer::Handle _Handle, XMFLOAT3 vPos)
{
	Effekseer::Vector3D v3Pos;
	v3Pos.X = vPos.x;
	v3Pos.Y = vPos.x;
	v3Pos.Z = vPos.x;

	m_Manager->SetLocation(_Handle, v3Pos);
}

// Effekseerの拡縮セット
void CEffekseerManager::SetScale(Effekseer::Handle _Handle, XMFLOAT3 vScl)
{
	
	m_Manager->SetScale(_Handle, vScl.x, vScl.y, vScl.z);
}

// Effekseerの回転セット
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
	// カメラ行列を設定
	m_Renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(
			m_Position,
			::Effekseer::Vector3D(vTarget.x, vTarget.y, vTarget.z),
			::Effekseer::Vector3D(vUp.x, vUp.y, vUp.z)));
	
	
	//m_Renderer->SetRenderMode(::Effekseer::RenderMode::Wireframe);
}

//// Effekseer内カメラプロジェクションマトリクス更新
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

// Efkファイル読み込み
bool CEffekseerManager::Load(const char16_t* path)
{
	auto itr = m_RegisteredEffectRef.find(path);
	if (itr != m_RegisteredEffectRef.end()) {
		// 設定されている場合の処理


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
		// 設定されていない場合の処理
		m_RegisteredEffectRef[path] = Effekseer::Effect::Create(m_Manager, (const char16_t*)BasePath);
		if (!(&m_RegisteredEffectRef[path]))
		{
			return false;
		}
		return true;
	}
}

// Efkファイル再生
/*::Effekseer::Handle*/int CEffekseerManager::Play(const char16_t* path, bool bLoop, int32_t nTime)
{
	auto itr = m_RegisteredEffectRef.find(path);
	if (itr != m_RegisteredEffectRef.end()) {
		// 設定されている場合の処理
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
		// 設定されていない場合の処理
		return -1;
	}
}

void CEffekseerManager::Stop(/*Effekseer::Handle _Handle*/int nIdx, bool bLoop)
{
	if (bLoop)
	{
		// エフェクトの停止
		m_Manager->StopEffect(m_RegisteredEffectsLoop[nIdx].t_Handle);
		m_RegisteredEffectsLoop.erase(nIdx);
	}
	else
	{
		// エフェクトの停止
		m_Manager->StopEffect(m_RegisteredEffectsNoLoop[nIdx].t_Handle);
		m_RegisteredEffectsNoLoop.erase(nIdx);
	}

	
}

// Registerの空いているインデックス検索用
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


