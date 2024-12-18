// ミサイル管理クラス [MSManager.h]
#pragma once
#include "WeaponManager.h"
#include "Missile.h"
#include <vector>

#include "Player.h"

using namespace std;
namespace {
	const int MAX_MISSILE = 6;
}

//struct TParts;

class CMSManager :public CWeaponManager
{
private:
	

public:
	CMSManager(CScene* pScene);
	~CMSManager();

	virtual HRESULT Init(XMFLOAT4X4*);
	virtual void Fin();
	virtual void Update();

	virtual int Fire();

	virtual int Create( EGameObjType Type, CModel* pEnemy = nullptr);
	
	

	virtual void SetVisible(bool bVisible);

};

TPartsInfo* GetMissileInitInfo();
