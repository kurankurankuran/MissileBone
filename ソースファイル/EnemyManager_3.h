// “GŠÇ—ƒNƒ‰ƒX3 [EnemyManager_3.h]
#pragma once

#include "EnemyManager.h"
#include <vector>

using namespace std;

//struct {
//	vector<CEnemy*> Wave;
//}EnemyWave;

class CEnemyManager_3 : public CEnemyManager
{
private:
	
protected:
	
public:
	CEnemyManager_3(CScene*);

	virtual HRESULT Init();
	virtual void CreateWave(int WaveNum);
};
