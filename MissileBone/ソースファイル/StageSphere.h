// ステージ球 [StageSphere.h]
#pragma once
#include "GameObj.h"
#include "Sphere.h"

class CStageSphere : public CGameObj
{
public:
	CStageSphere(CScene* pScene);
    ~CStageSphere();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	

private:

	int m_nHandle;

};
