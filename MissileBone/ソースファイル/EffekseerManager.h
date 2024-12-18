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

// Efk�I�u�W�F�N�g�p�����[�^
struct EfkObj{

	Effekseer::Handle t_Handle;// �n���h��
	const char16_t* t_szPath;
	int t_nTime;               // �P�T�C�N������
	int t_nCnt;                // �T�C�N���J�E���g
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

	
	// Effekseer�̈ʒu�Z�b�g
	static void SetLocation(Effekseer::Handle _Handle, XMFLOAT3 vPos);
	// Effekseer�̊g�k�Z�b�g
	static void SetScale(Effekseer::Handle _Handle, XMFLOAT3 vScl);
	// Effekseer�̉�]�Z�b�g
	static void SetRotation(Effekseer::Handle _Handle, XMFLOAT3 vRot);

	// Effekseer���J�����}�g���N�X�X�V
	void UpdateCameraMatrix();

	// ��]�A���W
	static void SetMatrix(/*Effekseer::Handle _Handle*/int nIdx, bool bLoop, XMFLOAT4X4& world);
	// Efk�t�@�C���ǂݍ���
	static bool Load(const char16_t* path);
	// Efk�Đ��J�n
	static /*::Effekseer::Handle*/int Play(const char16_t* path, bool bLoop, int32_t nTime);
	// Efk�I��
	static void Stop(/*Effekseer::Handle*/int,bool);

	static int FindRegisterIdx(std::unordered_map<int, EfkObj>, int);
private:
	
	
	EffekseerSound::SoundRef m_Sound;
	IXAudio2* m_xa2;
	IXAudio2MasteringVoice* m_xa2_master;
	
	// Effekseer���J�������_�ʒu���m��
	Effekseer::Vector3D m_Position;
	
	

	// �J�����|�C���^
	CCamera* m_pCamera;

	// �����_�[
	static EffekseerRendererDX11::RendererRef m_Renderer;
	// �}�l�[�W���[
	static Effekseer::ManagerRef m_Manager;
	// �ǂݍ��񂾃t�@�C���ɃC���f�b�N�X�t���ĊǗ�
	static std::unordered_map<const char16_t*, Effekseer::EffectRef> m_RegisteredEffectRef;
	//���[�v������Đ����̃G�t�F�N�g������
	static std::unordered_map</*::Effekseer::Handle*/int, EfkObj> m_RegisteredEffectsLoop;
	//���[�v�����Ȃ��Đ����̃G�t�F�N�g������
	static std::unordered_map</*::Effekseer::Handle*/int, EfkObj> m_RegisteredEffectsNoLoop;
};


