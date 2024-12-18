// プレイヤー [Player.h]
#pragma once
#include "Model.h"
//#include "Missile.h"

#include "WeaponManager.h"

#include <vector>


//#include "Effect.h"
#include "Land.h"

const XMFLOAT3 PLAYER_POS = XMFLOAT3(0.0f, 0.0f, 0.0f);



enum EParts {
	PARTS_BODY = 0,
	PARTS_CANON,
	/*PARTS_MISSILE_1,
	PARTS_MISSILE_2,
	PARTS_MISSILE_3,
	PARTS_MISSILE_4,
	PARTS_MISSILE_5,
	PARTS_MISSILE_6,*/


	MAX_PARTS

};

enum EWepons {
	WEAPON_MISSILE = 0,
	WEAPON_MINE,
	WEAPON_MULTIMS,
	WEAPON_MACHINEGUN,

	MAX_WEAPON
};

// パーツ情報
struct TParts {
	int nModel;  // モデル番号
	int nParent; // 親パーツ番号
	XMFLOAT3 vPos; // オフセット位置
	XMFLOAT3 vAngle; // 角度
	XMFLOAT4X4 mWorld; // ワールド変換
	bool bUnit; // 結合フラグ
};

struct TPartsInfo {
	int nModel;
	int nParent;
	XMFLOAT3 vPos;
	XMFLOAT3 vAngle;
	bool bUnit;
};

class CWeaponManager;
class CPlayer : public CModel
{
private:
	float m_fCanonAngle;

	TParts m_parts[MAX_PARTS];

	int m_nWeaponType;  // 武器種別
	CWeaponManager* m_pWeaponManager; // 武器管理クラスポインタ

	int m_nEffectCnt;
	//CEffect m_Effect;

	CGameObj* m_pStageSpher; // ステージ球

	bool m_bDash;   // ダッシュ中フラグ
	int m_nDashCnt; // ダッシュカウント

	ID3D11ShaderResourceView*	m_pTextureWeaponType[MAX_WEAPON];	// 武器種テクスチャ
	ID3D11ShaderResourceView*	m_pTextureNum;	// 数字テクスチャ

	CLand* m_pLand;
public:
	CPlayer(CScene* pScene); // コンストラクタ
	virtual ~CPlayer();		 // デストラクタ

	virtual HRESULT Init();	 // 初期化
	virtual void Fin();		 // 終了処理
	virtual void Update();	 // 更新
	virtual void Draw();	 // 描画

	TParts* GetParts() { return m_parts; }
	
	void ChangeWeapon();
	void SetStageSpher(CGameObj* pObj) { m_pStageSpher = pObj; }

	void CollosionLand(XMFLOAT3* vMove);
};
