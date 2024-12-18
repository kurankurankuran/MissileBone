// ’Ç”öƒ~ƒTƒCƒ‹ [Missile.h]
#pragma once
#include "Weapon.h"
//#include "Effect.h"

class CEnemy;
class CMissile : public CWeapon
{
private:
	
	//CEffect m_Effect;
public:
	CMissile(CScene* pScene);
	~CMissile();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	/*static*/ int Fire(XMFLOAT4X4* pWorld, EGameObjType Type, CModel* pEnemy = nullptr);

	int Fire();
	

};
