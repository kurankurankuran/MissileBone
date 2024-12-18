// 武器管理抽象管理クラス [WeaponManager.h]
#pragma once

#include "Weapon.h"
#include <vector>
#include "Player.h"
using namespace std;

struct TParts;
class CWeaponManager
{
private:
	
	
protected:
	CScene* m_pScene;
	CGameObj* m_pParent;
	bool      m_bVisible;
	bool      m_bOldVisible;

	vector<TParts> m_WeaponParts;
	vector<CWeapon*> m_pWeapon; // 武器実態ポインタ
	
	int m_nMagazine; // 装填数
	int m_nReloadCnt;
	bool m_bCreate;

	XMFLOAT4X4* m_pParentWorld;

	int m_nChangeNum;
	int m_nChangeCnt;
public:
	CWeaponManager(CScene* pScene);
	~CWeaponManager();

	virtual HRESULT Init(XMFLOAT4X4*) = 0;
	virtual void Fin() = 0;
	virtual void Update() = 0;

	virtual int Fire() = 0;

	virtual int Create( EGameObjType Type, CModel* pEnemy = nullptr) = 0;
	void AllDelete();

	void SetWorld(int i, XMFLOAT4X4& pWorld);


	virtual void SetVisible(bool bVisible) = 0;

	int GetWeaponSize() { return m_pWeapon.size(); }

	bool GetWeaponUnit(int i) { return m_pWeapon[i]->GetUnit(); }

	void SetParent(CGameObj* Obj) { m_pParent = Obj; }

	bool GetChangeFlg();

	int GetMagazineNum() { return m_nMagazine; }
};

