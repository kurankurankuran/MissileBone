#include "MineManager.h"
#include "Game.h"

namespace {
	const int g_nMaxLife = 600;
	const int g_nMaxLifeEnemy = 200;
	const int g_nReLoadCnt = 120;

	const int g_nChangeNum = 200.0f;
}


static TPartsInfo g_partsInit[MAX_MINE] =
{
{MODEL_MISSILE, MODEL_PLAYER_CANON, { 200.0f, -40.0f, 0.0f}, {0.0f, 180.0f, 0.0f}, true},
{MODEL_MISSILE, MODEL_PLAYER_CANON, { 165.0f, -40.0f, 0.0f}, {0.0f, 180.0f, 0.0f}, true},
{MODEL_MISSILE, MODEL_PLAYER_CANON, { 130.0f, -40.0f, 0.0f}, {0.0f, 180.0f, 0.0f}, true},
{MODEL_MISSILE, MODEL_PLAYER_CANON, { -130.0f, -40.0f, 0.0f}, {0.0f, 180.0f, 0.0f}, true},
{MODEL_MISSILE, MODEL_PLAYER_CANON, { -165.0f, -40.0f, 0.0f}, {0.0f, 180.0f, 0.0f}, true},
{MODEL_MISSILE, MODEL_PLAYER_CANON, { -200.0f, -40.0f, 0.0f}, {0.0f, 180.0f, 0.0f}, true},
{MODEL_MISSILE, MODEL_PLAYER_CANON, { 200.0f, -80.0f, 0.0f}, {0.0f, 180.0f, 0.0f}, true},
{MODEL_MISSILE, MODEL_PLAYER_CANON, { 165.0f, -80.0f, 0.0f}, {0.0f, 180.0f, 0.0f}, true},
{MODEL_MISSILE, MODEL_PLAYER_CANON, { 130.0f, -80.0f, 0.0f}, {0.0f, 180.0f, 0.0f}, true},
{MODEL_MISSILE, MODEL_PLAYER_CANON, { -130.0f, -80.0f, 0.0f}, {0.0f, 180.0f, 0.0f}, true},
{MODEL_MISSILE, MODEL_PLAYER_CANON, { -165.0f, -80.0f, 0.0f}, {0.0f, 180.0f, 0.0f}, true},
{MODEL_MISSILE, MODEL_PLAYER_CANON, { -200.0f, -80.0f, 0.0f}, {0.0f, 180.0f, 0.0f}, true},
};



CMineManager::CMineManager(CScene* pScene) : CWeaponManager(pScene)
{

}

CMineManager::~CMineManager()
{

}

HRESULT CMineManager::Init(XMFLOAT4X4* pWorld)
{
	m_nChangeNum = g_nChangeNum;
	m_nChangeCnt = m_nChangeNum;

	m_pParentWorld = pWorld;

	m_nReloadCnt = g_nReLoadCnt;

	for (int i = 0; i < MAX_MINE; ++i) {
		//CMissile* Missile = new CMissile(m_pScene);
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

void CMineManager::Fin()
{
	/*for (int i = 0; i < MAX_MISSILE; ++i) {
		if (m_pMissile[i]) {
			delete m_pMissile[i];
			m_pMissile[i] = nullptr;
		}
	}*/

	for (int i = 0; i < m_pWeapon.size(); ++i) {
		if (m_pWeapon[i]) {
			delete m_pWeapon[i];
			m_pWeapon[i] = nullptr;
		}
	}
	m_pWeapon.clear();
}

void CMineManager::Update()
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

int CMineManager::Fire()
{

	int nEmpty = 0;
	for (int i = 0; i < m_pWeapon.size(); ++i, ++nEmpty) {
		if (m_pWeapon[i]) {
			m_pWeapon[i]->Fire();
			m_pWeapon[i] = nullptr;

			
			m_WeaponParts[i].bUnit = false;

			if (nEmpty >= m_pWeapon.size() - 1)
				m_bCreate = true;

			CSound::Play(SE_SHOT);

			return 1;
		}
	}

	return 0;
}

int CMineManager::Create(EGameObjType Type, CModel* pEnemy)
{
	if (!m_pScene)
		return -1;

	if (!m_bCreate)
		return -1;


	CPlayer* pPlayer;
	pPlayer = (CPlayer*)m_pScene->Find(GOT_PLAYER);


	for (int i = 0; i < m_pWeapon.size(); ++i) {

		if (!m_pWeapon[i]) {
			m_pWeapon[i] = new CMine(m_pScene);

			m_pWeapon[i]->SetType(Type);
			m_pWeapon[i]->Init();
			m_pWeapon[i]->SetTarget(pEnemy);
			m_pWeapon[i]->SetBSVisible((EBoundingShape)((CGame*)m_pScene)->GetBound());



			m_pWeapon[i]->SetWorld(m_WeaponParts[i].mWorld);



			m_WeaponParts[i].bUnit = true;
			m_pWeapon[i]->SetUnit(m_WeaponParts[i].bUnit);

			m_bCreate = false;

		}

	}

	return 1;
}

void CMineManager::SetVisible(bool bVisible)
{
	if (m_pParent)
	{
		for (int i = 0; i < m_pWeapon.size(); ++i)
		{
			if (m_pWeapon[i]) {
				m_pWeapon[i]->SetVisible(bVisible);
			}
		}
	}
}


TPartsInfo* GetMineInitInfo()
{
	return g_partsInit;
}