// “GŠÇ—ƒNƒ‰ƒX1 [EnemyManager_1.h]
#pragma once

#include "EnemyManager.h"
#include <vector>

using namespace std;

class CEnemyManager_StageSelect : public CEnemyManager
{
private:
	
protected:
	
public:
	CEnemyManager_StageSelect(CScene*);

	virtual HRESULT Init();
	virtual void Draw();
	virtual void CreateWave(int WaveNum);
};
