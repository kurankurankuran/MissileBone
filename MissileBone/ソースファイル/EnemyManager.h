// 敵管理クラス [EnemyManager.h]
#pragma once

#include "Enemy.h"
#include <vector>

using namespace std;


class CEnemyManager : public CGameObj
{
private:


protected:
	CPlayer* m_pPlayer; // プレイヤー
	CGameObj* m_pGameObj; // ステージ球（範囲外から戻ってくる用）

	CScene* m_pScene;
	int m_nWaveNum; // ウェーブ数
	int m_nWaveCurNum; // 現在のウェーブ番号
	vector<CEnemy*> m_pWave; // ウェーブ中の敵
	int m_nDeadCnt;   // 倒された敵数

	ID3D11ShaderResourceView*	m_pTextureNum;	// 数字テクスチャ
	ID3D11ShaderResourceView*	m_pTextureEnemy;	// 残敵テクスチャ
	ID3D11ShaderResourceView*	m_pTextureWave;	// Waveテクスチャ
	ID3D11ShaderResourceView*	m_pTextureSrush;	// スラッシュテクスチャ

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
