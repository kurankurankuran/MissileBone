// “GŠÇ—ƒNƒ‰ƒX1 [EnemyManager_2.h]
#pragma once

#include "EnemyManager.h"
#include <vector>

using namespace std;

//struct {
//	vector<CEnemy*> Wave;
//}EnemyWave;

class CEnemyManager_2 : public CEnemyManager
{
private:
	
protected:
	
public:
	CEnemyManager_2(CScene*);

	virtual HRESULT Init();
	virtual void CreateWave(int WaveNum);
};
