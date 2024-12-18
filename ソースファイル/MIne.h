// 地雷ミサイル [Mine.h]
#pragma once
#include "Weapon.h"
//#include "Effect.h"

class CEnemy;
class CMine : public CWeapon
{
private:
	//CEffect m_Effect;
public:
	CMine(CScene* pScene);
	~CMine();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	int Fire(XMFLOAT4X4* pWorld, EGameObjType Type, CModel* pEnemy = nullptr);

	int Fire();

	

};
