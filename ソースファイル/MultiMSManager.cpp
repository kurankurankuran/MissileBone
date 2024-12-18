#include "MultiMSManager.h"
#include "Game.h"
#include "Sound.h"

namespace {
	const int g_nMaxLife = 600;
	const int g_nMaxLifeEnemy = 200;
	const int g_nReLoadCnt = 120;

	const int g_nChangeNum = 300.0f;
}


static TPartsInfo g_partsInit[MAX_MULTIMS] =
{
{MODEL_MISSILE, MODEL_PLAYER_CANON, {    0.0f,   0.0f,   600.0f}, {   0.0f,   0.0f,   0.0f}, true}, // 前
{MODEL_MISSILE, MODEL_PLAYER_CANON, {    0.0f,   0.0f,  -600.0f}, {   0.0f, 180.0f,   0.0f}, true}, // 後
{MODEL_MISSILE, MODEL_PLAYER_CANON, {  600.0f,   0.0f,     0.0f}, {   0.0f,  90.0f,   0.0f}, true}, // 右
{MODEL_MISSILE, MODEL_PLAYER_CANON, { -600.0f,   0.0f,     0.0f}, {   0.0f, -90.0f,   0.0f}, true}, // 左
{MODEL_MISSILE, MODEL_PLAYER_CANON, {    0.0f,  600.0f,    0.0f}, { -90.0f,   0.0f,   0.0f}, true}, // 上
{MODEL_MISSILE, MODEL_PLAYER_CANON, {    0.0f, -600.0f,    0.0f}, {  90.0f,   0.0f,   0.0f}, true}, // 下
{MODEL_MISSILE, MODEL_PLAYER_CANON, {  300.0f,  300.0f,  300.0f}, { -45.0f,  45.0f,   0.0f}, true}, // 右上前
{MODEL_MISSILE, MODEL_PLAYER_CANON, {  300.0f,  300.0f, -300.0f}, { -45.0f, 135.0f,   0.0f}, true}, // 右上後
{MODEL_MISSILE, MODEL_PLAYER_CANON, {  300.0f, -300.0f,  300.0f}, {  45.0f,  45.0f,   0.0f}, true}, // 右下前
{MODEL_MISSILE, MODEL_PLAYER_CANON, {  300.0f, -300.0f, -300.0f}, {  45.0f, 135.0f,   0.0f}, true}, // 右下後
{MODEL_MISSILE, MODEL_PLAYER_CANON, { -300.0f,  300.0f,  300.0f}, { -45.0f, -45.0f,   0.0f}, true}, // 左上前
{MODEL_MISSILE, MODEL_PLAYER_CANON, { -300.0f,  300.0f, -300.0f}, { -45.0f,-135.0f,   0.0f}, true}, // 左上後
{MODEL_MISSILE, MODEL_PLAYER_CANON, { -300.0f, -300.0f,  300.0f}, {  45.0f, -45.0f,   0.0f}, true}, // 左下前
{MODEL_MISSILE, MODEL_PLAYER_CANON, { -300.0f, -300.0f, -300.0f}, {  45.0f,-135.0f,   0.0f}, true}, // 左下後

};


CMultiMSManager::CMultiMSManager(CScene* pScene) : CWeaponManager(pScene)
{

}

CMultiMSManager::~CMultiMSManager()
{

}

HRESULT CMultiMSManager::Init(XMFLOAT4X4* pWorld)
{
	m_nChangeNum = g_nChangeNum;
	m_nChangeCnt = m_nChangeNum;

	m_pParentWorld = pWorld;

	m_nReloadCnt = g_nReLoadCnt;
	m_nMagazine = MAX_MULTIMS;

	for (int i = 0; i < MAX_MULTIMS; ++i) {

		m_pWeapon.push_back(nullptr);
	}

	// 武器サイズ分のvector作成
	m_WeaponParts.clear();
	for (int i = 0; i < m_pWeapon.size(); ++i) {

		TParts Weapon;

		Weapon.nModel = g_partsInit[i].nModel;
		Weapon.nParent = g_partsInit[i].nParent;
		Weapon.vPos = g_partsInit[i].vPos;
		Weapon.vAngle = g_partsInit[i].vAngle;
		Weapon.bUnit = g_partsInit[i].bUnit;

		XMMATRIX mtx = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(Weapon.vAngle.x),
			XMConvertToRadians(Weapon.vAngle.y),
			XMConvertToRadians(Weapon.vAngle.z)) *
			XMMatrixTranslation(Weapon.vPos.x,
				Weapon.vPos.y, Weapon.vPos.z);

		if (Weapon.nParent >= 0) {
			mtx *= XMLoadFloat4x4(m_pParentWorld);
		}
		XMStoreFloat4x4(&Weapon.mWorld, mtx);

		m_WeaponParts.push_back(Weapon);
	}

	return true;
}

void CMultiMSManager::Fin()
{

	for (int i = 0; i < m_pWeapon.size(); ++i) {
		if (m_pWeapon[i]) {
			delete m_pWeapon[i];
			m_pWeapon[i] = nullptr;
		}
	}
	m_pWeapon.clear();
}

void CMultiMSManager::Update()
{
	if (m_pParent) {
		m_bOldVisible = m_bVisible;
		m_bVisible = m_pParent->GetVisible();
		if (m_bVisible != m_bOldVisible) {
			if (m_bVisible) {
				SetVisible(true);
			}
			else
			{
				SetVisible(false);
			}
		}

		for (int i = 0; i < m_pWeapon.size(); ++i) {
			if (m_pWeapon[i] == nullptr)continue;
			if (m_pWeapon[i]->GetUnit()) {
				if (m_pWeapon[i]->SerchTarget()) {
					Fire(i);
					CSound::Play(SE_SHOT);
				}
			}
		}

		if (!m_bCreate && m_nMagazine <= 0) {
			--m_nReloadCnt;
			if (m_nReloadCnt <= 0) {
				m_bCreate = true;
				m_nReloadCnt = g_nReLoadCnt;
			}
		}
		

		// 武器更新
		for (int i = 0; i < m_pWeapon.size(); ++i) {

			if (m_WeaponParts[i].bUnit) {
				XMMATRIX mtx = XMMatrixRotationRollPitchYaw(
					XMConvertToRadians(m_WeaponParts[i].vAngle.x),
					XMConvertToRadians(m_WeaponParts[i].vAngle.y),
					XMConvertToRadians(m_WeaponParts[i].vAngle.z)) *
					XMMatrixTranslation(m_WeaponParts[i].vPos.x, m_WeaponParts[i].vPos.y, m_WeaponParts[i].vPos.z);
				if (m_WeaponParts[i].nParent < 0) {
					//mtx *= XMLoadFloat4x4(&GetWorld());
				}
				else {
					mtx *= XMLoadFloat4x4(m_pParentWorld);
				}
				XMStoreFloat4x4(&m_WeaponParts[i].mWorld, mtx);


				m_pWeapon[i]->SetWorld(m_WeaponParts[i].mWorld);

			}

		}

		if (m_nChangeCnt > 0) {
			--m_nChangeCnt;
		}
	}

	
}

int CMultiMSManager::Fire()
{
	int nSuccese = 0; // 発射成功の有無
	
	for (int i = 0; i < m_pWeapon.size(); ++i) {
		if (m_pWeapon[i]) {
			m_pWeapon[i]->Fire();
			m_pWeapon[i] = nullptr;
			--m_nMagazine;

			m_WeaponParts[i].bUnit = false;
			nSuccese = 1;

			CSound::Play(SE_SHOT);
		}
	}



	return nSuccese;
}

void CMultiMSManager::Fire(int nNum)
{
	m_pWeapon[nNum]->Fire();
	m_pWeapon[nNum] = nullptr;
	--m_nMagazine;
	m_WeaponParts[nNum].bUnit = false;
	
}

int CMultiMSManager::Create(EGameObjType Type, CModel* pEnemy)
{
	if (!m_pScene)
		return -1;

	if (!m_bCreate)
		return -1;

	if (m_nReloadCnt < 120) {
		return -1;
	}


	CPlayer* pPlayer;
	pPlayer = (CPlayer*)m_pScene->Find(GOT_PLAYER);


	for (int i = 0; i < m_pWeapon.size(); ++i) {

		if (!m_pWeapon[i]) {
			m_pWeapon[i] = new CMultiMissile(m_pScene);
			
			m_pWeapon[i]->SetType(Type);
			m_pWeapon[i]->Init();
			m_pWeapon[i]->SetTarget(pEnemy);
			m_pWeapon[i]->SetBSVisible((EBoundingShape)((CGame*)m_pScene)->GetBound());



			m_pWeapon[i]->SetWorld(m_WeaponParts[i].mWorld);



			m_WeaponParts[i].bUnit = true;
			m_pWeapon[i]->SetUnit(m_WeaponParts[i].bUnit);

		}

	}

	m_nMagazine = MAX_MULTIMS;
	m_bCreate = false;

	return 1;
}



void CMultiMSManager::SetVisible(bool bVisible)
{
	if (m_pParent)
	{
		for (int i = 0; i < MAX_MULTIMS; ++i)
		{
			if (m_pWeapon[i]) {
				m_pWeapon[i]->SetVisible(bVisible);
			}
		}
	}
}

TPartsInfo* GetMultiMSInitInfo()
{
	return g_partsInit;
}