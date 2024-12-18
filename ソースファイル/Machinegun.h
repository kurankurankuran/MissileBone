// ƒ}ƒVƒ“ƒKƒ“ [Machinegun.h]
#pragma once
#include "Weapon.h"

class CMesh;
class CEnemy;
class CMachinegun : public CWeapon
{
private:
	
	CMesh m_Mesh;
	int m_nHandle;
public:
	CMachinegun(CScene* pScene);
	~CMachinegun();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	/*static*/ int Fire(XMFLOAT4X4* pWorld, EGameObjType Type, CModel* pEnemy = nullptr);
	void SetEfkHandle(int nHandle) { m_nHandle = nHandle; }

	int Fire();
	
	CMesh* GetMesh() { return &m_Mesh; }
};
