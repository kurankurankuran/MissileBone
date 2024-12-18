// �G�Ǘ��N���X [EnemyManager.h]
#pragma once

#include "Enemy.h"
#include <vector>

using namespace std;


class CEnemyManager : public CGameObj
{
private:


protected:
	CPlayer* m_pPlayer; // �v���C���[
	CGameObj* m_pGameObj; // �X�e�[�W���i�͈͊O����߂��Ă���p�j

	CScene* m_pScene;
	int m_nWaveNum; // �E�F�[�u��
	int m_nWaveCurNum; // ���݂̃E�F�[�u�ԍ�
	vector<CEnemy*> m_pWave; // �E�F�[�u���̓G
	int m_nDeadCnt;   // �|���ꂽ�G��

	ID3D11ShaderResourceView*	m_pTextureNum;	// �����e�N�X�`��
	ID3D11ShaderResourceView*	m_pTextureEnemy;	// �c�G�e�N�X�`��
	ID3D11ShaderResourceView*	m_pTextureWave;	// Wave�e�N�X�`��
	ID3D11ShaderResourceView*	m_pTextureSrush;	// �X���b�V���e�N�X�`��

	XMFLOAT3 m_vCameraTargetPos;
	XMFLOAT2 m_fFreeBulletMode;
public:
	CEnemyManager(CScene*);
	~CEnemyManager();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	void SetPlayer(CPlayer*);
	void SetTarget(CGameObj*);
	virtual void CreateWave(int WaveNum);

	int GetCurWaveNum() { return m_nWaveCurNum; }
	int GetWaveEnemyNum();
	int GetWaveEnemyMax() { return m_pWave.size(); }

	void DestroyEnemy();

	XMFLOAT3& GetCameraTargetPos() { return m_vCameraTargetPos; }
	XMFLOAT2 GetfFreeBulletMode() { return m_fFreeBulletMode; }
};
