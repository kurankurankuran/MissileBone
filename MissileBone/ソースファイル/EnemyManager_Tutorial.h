#pragma once
#include "EnemyManager.h"

class CEnemyManager_Tutorial :public CEnemyManager
{
private:

public:
	CEnemyManager_Tutorial(CScene*);

	virtual HRESULT Init();
	virtual void Update();
	virtual void CreateWave(int WaveNum);

};
