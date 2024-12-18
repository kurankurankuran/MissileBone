// ミサイル管理クラス [MSManager.h]
#pragma once
#include "WeaponManager.h"
#include "Mine.h"
#include <vector>
#include "Player.h"
using namespace std;
namespace {
	const int MAX_MINE = 12;
}

//struct TParts;

class CMineManager :public CWeaponManager
{
private:


public:
	CMineManager(CScene* pScene);
	~CMineManager();

	virtual HRESULT Init(XMFLOAT4X4*);
	virtual void Fin();
	virtual void Update();

	virtual int Fire();

	virtual int Create( EGameObjType Type, CModel* pEnemy = nullptr);



	virtual void SetVisible(bool bVisible);

};

TPartsInfo* GetMineInitInfo();