#pragma once
#include "Scene.h"
#include "Player.h"
#include "Camera.h"
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <EffekseerSoundXAudio2.h>
#include <unordered_map>

#pragma comment(lib, "Effekseer")
#pragma comment(lib, "EffekseerRendererDX11")
#pragma comment(lib, "EffekseerSoundXAudio2")

// Efkオブジェクトパラメータ
struct EfkObj{

	Effekseer::Handle t_Handle;// ハンドル
	const char16_t* t_szPath;
	int t_nTime;               // １サイクル時間
	int t_nCnt;                // サイクルカウント
	EfkObj()
	{
	}

	EfkObj(const char16_t* path, Effekseer::Handle _Handle,int nTime,  int nCnt = 0)
	{
		t_Handle = _Handle;
		t_szPath = path;
		t_nTime = nTime;
		t_nCnt = nCnt;
	}
	
};

class CEffekseerManager
{
public:
	CEffekseerManager(CScene*);
	~CEffekseerManager();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	//void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }

	
	// Effekseerの位置セット
	static void SetLocation(Effekseer::Handle _Handle, XMFLOAT3 vPos);
	// Effekseerの拡縮セット
	static void SetScale(Effekseer::Handle _Handle, XMFLOAT3 vScl);
	// Effekseerの回転セット
	static void SetRotation(Effekseer::Handle _Handle, XMFLOAT3 vRot);

	// Effekseer内カメラマトリクス更新
	void UpdateCameraMatrix();

	// 回転、座標
	static void SetMatrix(/*Effekseer::Handle _Handle*/int nIdx, bool bLoop, XMFLOAT4X4& world);
	// Efkファイル読み込み
	static bool Load(const char16_t* path);
	// Efk再生開始
	static /*::Effekseer::Handle*/int Play(const char16_t* path, bool bLoop, int32_t nTime);
	// Efk終了
	static void Stop(/*Effekseer::Handle*/int,bool);

	static int FindRegisterIdx(std::unordered_map<int, EfkObj>, int);
private:
	
	
	EffekseerSound::SoundRef m_Sound;
	IXAudio2* m_xa2;
	IXAudio2MasteringVoice* m_xa2_master;
	
	// Effekseer内カメラ視点位置を確定
	Effekseer::Vector3D m_Position;
	
	

	// カメラポインタ
	CCamera* m_pCamera;

	// レンダー
	static EffekseerRendererDX11::RendererRef m_Renderer;
	// マネージャー
	static Effekseer::ManagerRef m_Manager;
	// 読み込んだファイルにインデックス付して管理
	static std::unordered_map<const char16_t*, Effekseer::EffectRef> m_RegisteredEffectRef;
	//ループさせる再生中のエフェクトが入る
	static std::unordered_map</*::Effekseer::Handle*/int, EfkObj> m_RegisteredEffectsLoop;
	//ループさせない再生中のエフェクトが入る
	static std::unordered_map</*::Effekseer::Handle*/int, EfkObj> m_RegisteredEffectsNoLoop;
};


