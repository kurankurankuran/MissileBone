// マルチミサイル [MultiMissile.h]
#pragma once
#include "Weapon.h"
//#include "Effect.h"

class CEnemy;
class CMultiMissile : public CWeapon
{
private:
	
	//CEffect m_Effect;
public:
	CMultiMissile(CScene* pScene);
	~CMultiMissile();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	int Fire(XMFLOAT4X4* pWorld, EGameObjType Type, CModel* pEnemy = nullptr);

	int Fire();

	
};
