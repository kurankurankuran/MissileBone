// “GŠÇ—ƒNƒ‰ƒX1 [EnemyManager_1.h]
#pragma once

#include "EnemyManager.h"
#include <vector>

using namespace std;

//struct {
//	vector<CEnemy*> Wave;
//}EnemyWave;

class CEnemyManager_1 : public CEnemyManager
{
private:
	
protected:
	
public:
	CEnemyManager_1(CScene*);

	virtual HRESULT Init();
	virtual void CreateWave(int WaveNum);
};
