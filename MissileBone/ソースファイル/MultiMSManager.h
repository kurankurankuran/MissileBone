// ミサイル管理クラス [MSManager.h]
#pragma once
#include "WeaponManager.h"
#include "MultiMissile.h"
#include <vector>

#include "Player.h"

using namespace std;
namespace {
	const int MAX_MULTIMS = 14;
}

//struct TParts;

class CMultiMSManager :public CWeaponManager
{
private:


public:
	CMultiMSManager(CScene* pScene);
	~CMultiMSManager();

	virtual HRESULT Init(XMFLOAT4X4*);
	virtual void Fin();
	virtual void Update();

	virtual int Fire();
	void Fire(int);

	virtual int Create( EGameObjType Type, CModel* pEnemy = nullptr);



	virtual void SetVisible(bool bVisible);

};

TPartsInfo* GetMultiMSInitInfo();
