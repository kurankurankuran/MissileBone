// íäè€ïêäÌÉNÉâÉX [Weapon.h]
#pragma once
#include "Model.h"

class CEnemy;
class CWeapon : public CModel
{
private:
	

	
	

protected:
	bool m_bUnit;
	//int m_nLife;
	CModel* m_pTarget;
	EGameObjType m_Type;
	float m_fSpeed;
	int m_nEffectCnt;
public:
	CWeapon(CScene* pScene);
	~CWeapon();

	virtual HRESULT Init() = 0;
	virtual void Update() = 0;

	virtual int Fire(XMFLOAT4X4* pWorld, EGameObjType Type, CModel* pEnemy = nullptr) = 0;

	virtual int Fire() = 0;

	void SetUnit(bool bUnit) { m_bUnit = bUnit; }
	//void SetLife(int nLife) { m_nLife = nLife; }
	void SetTarget(CModel* pTarget) { m_pTarget = pTarget; }
	void SetType(EGameObjType Type) { m_Type = Type; }

	CModel* GetTarget() { return m_pTarget; }
	EGameObjType GetType() { return m_Type; }

	bool SerchTarget();
	bool GetUnit() { return m_bUnit; }

	
};
