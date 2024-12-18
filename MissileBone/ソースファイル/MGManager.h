// マシンガン管理クラス [MGManager.h]
#pragma once
#include "WeaponManager.h"
#include "Machinegun.h"
#include <vector>

#include "Player.h"

using namespace std;
namespace {
	const int MAX_MACHINEGUN = 120;
}

//struct TParts;

class CMGManager :public CWeaponManager
{
private:
	float m_fFireRate;
	ID3D11ShaderResourceView* m_pTex;
public:
	CMGManager(CScene* pScene);
	~CMGManager();

	virtual HRESULT Init(XMFLOAT4X4*);
	virtual void Fin();
	virtual void Update();

	virtual int Fire();

	virtual int Create( EGameObjType Type, CModel* pEnemy = nullptr);
	
	

	virtual void SetVisible(bool bVisible);

	enum  EfkType
	{
		EXPLOSION_TYPE = 0,
		MACHINGUN_TYPE,

		MAX_EFK
	};
};

TPartsInfo* GetMachinegunInitInfo();
