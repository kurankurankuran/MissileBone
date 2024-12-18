#include "MGManager.h"
#include "Game.h"
#include "Input.h"
#include "Sound.h"
#include "Texture.h"
#include "Polygon.h"
#include "EffekseerManager.h"


namespace {


	const int g_nMaxLife = 600;
	const int g_nMaxLifeEnemy = 200;
	const int g_nReLoadCnt = 120;

	const int g_nChangeNum = 100.0f;
	const float g_fFireRate = 0.05f * 60.0f;

	//const LPCWSTR g_pszPathTex = L"data/texture/effect000.png";

	

	EfkPath g_pszEfkPath = {
		u"Explosion_2.efk",
		u"Machingun.efk",
	};
}


static TPartsInfo g_partsInit = 
{
  MODEL_MISSILE, MODEL_PLAYER_CANON, { 0.0f, -50.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, true
};


CMGManager::CMGManager(CScene* pScene) : CWeaponManager(pScene)
{
	
}

CMGManager::~CMGManager()
{

}

HRESULT CMGManager::Init(XMFLOAT4X4* pWorld)
{
	m_nMagazine = MAX_MACHINEGUN;
	m_fFireRate = 0.0f;
	m_nChangeNum = g_nChangeNum;
	m_nChangeCnt = m_nChangeNum;

	m_pParentWorld = pWorld;

	m_nReloadCnt = g_nReLoadCnt;

	for (int i = 0; i < MAX_MACHINEGUN; ++i) {
		
		m_pWeapon.push_back(nullptr);
	}

	// 武器サイズ分のvector作成
	m_WeaponParts.clear();
	for (int i = 0; i < m_pWeapon.size(); ++i) {

		TParts Weapon;

		Weapon.nModel = g_partsInit.nModel;
		Weapon.nParent = g_partsInit.nParent;
		Weapon.vPos = g_partsInit.vPos;
		Weapon.vAngle = g_partsInit.vAngle;
		Weapon.bUnit = g_partsInit.bUnit;

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

	/*ID3D11Device* pDevice = GetDevice();
	HRESULT hr = CreateTextureFromFile(pDevice, g_pszPathTex, &m_pTex);
	if (FAILED(hr)) {
		Fin();
		return false;
	}*/
	
	// エフェクシアファイルを読み込み
	if (!CEffekseerManager::Load(g_pszEfkPath.t_Path[EXPLOSION_TYPE]))
	{
		return false;
	}
	if (!CEffekseerManager::Load(g_pszEfkPath.t_Path[MACHINGUN_TYPE]))
	{
		return false;
	}

	return true;
}

void CMGManager::Fin()
{
	//SAFE_RELEASE(m_pTex);
	for (int i = 0; i < m_pWeapon.size(); ++i) {
		if (m_pWeapon[i]) {
			delete m_pWeapon[i];
			m_pWeapon[i] = nullptr;
		}
	}
	m_pWeapon.clear();
}

void CMGManager::Update()
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

		

		if (m_nChangeCnt > 0) {
			--m_nChangeCnt;
		}
		if (CInput::GetKeyPress(VK_L))
		{
			--m_fFireRate;
		}
		else
		{
			m_fFireRate = g_fFireRate;
		}
	}



	
}

int CMGManager::Fire()
{
	
	if (m_fFireRate > 0.0f)return 0;// 発射レート
	m_fFireRate = g_fFireRate;      // 発射レートリセット

	int nEmpty = 0;
	for (int i = 0; i < m_pWeapon.size(); ++i, ++nEmpty) {
		if (m_pWeapon[i]) {
			// プレイヤーのワールド行列をかける
			// 武器更新
			if (m_WeaponParts[i].bUnit) {
				XMMATRIX mtx = XMMatrixRotationRollPitchYaw(
					XMConvertToRadians(m_WeaponParts[i].vAngle.x),
					XMConvertToRadians(m_WeaponParts[i].vAngle.y),
					XMConvertToRadians(m_WeaponParts[i].vAngle.z)) *
					XMMatrixTranslation(m_WeaponParts[i].vPos.x, m_WeaponParts[i].vPos.y, m_WeaponParts[i].vPos.z);
				//if (m_WeaponParts[i].nParent < 0) {
				//	//mtx *= XMLoadFloat4x4(&GetWorld());
				//}
				//else {
					mtx *= XMLoadFloat4x4(m_pParentWorld);
				//}
				XMStoreFloat4x4(&m_WeaponParts[i].mWorld, mtx);

				m_pWeapon[i]->SetWorld(m_WeaponParts[i].mWorld);
			}
			int nHandle = CEffekseerManager::Play(g_pszEfkPath.t_Path[CMGManager::EfkType::MACHINGUN_TYPE], true, 10);
			CEffekseerManager::SetMatrix(nHandle, true, m_pWeapon[i]->GetWorld());
			CMachinegun* pMachingnun = (CMachinegun*)m_pWeapon[i];
			pMachingnun->SetEfkHandle(nHandle);
			
			m_pWeapon[i]->Fire();
			m_pWeapon[i] = nullptr;

			m_WeaponParts[i].bUnit = false;
			if (nEmpty >= MAX_MACHINEGUN - 1)
				m_bCreate = true;

			--m_nMagazine;
			CSound::Play(SE_SHOT);

			

			return 1;
		}
	}

	return 0;
}

int CMGManager::Create( EGameObjType Type, CModel* pEnemy)
{
	if (!m_pScene)
		return -1;

	if (!m_bCreate)
		return -1;


	CPlayer* pPlayer;
	pPlayer = (CPlayer*)m_pScene->Find(GOT_PLAYER);


	for (int i = 0 ; i < m_pWeapon.size(); ++i) {
		
		if (!m_pWeapon[i]) {
			m_pWeapon[i] = new CMachinegun(m_pScene);

			m_pWeapon[i]->SetType(Type);
			m_pWeapon[i]->SetEfkHandle(&g_pszEfkPath);
			m_pWeapon[i]->Init();
			m_pWeapon[i]->SetTarget(pEnemy);
			m_pWeapon[i]->SetBSVisible((EBoundingShape)((CGame*)m_pScene)->GetBound());
			m_pWeapon[i]->SetWorld(m_WeaponParts[i].mWorld);
			

			m_WeaponParts[i].bUnit = true;
			m_pWeapon[i]->SetUnit(m_WeaponParts[i].bUnit);

			/*CMachinegun* pMachinegum = (CMachinegun*)m_pWeapon[i];
			pMachinegum->GetMesh()->SetTexture(m_pTex);
			pMachinegum->GetMesh()->SetPimitiveType(PT_UNDEFINED);*/

			m_bCreate = false;
			
		}
		
	}
	m_nMagazine = MAX_MACHINEGUN;

	return 1;
}


void CMGManager::SetVisible(bool bVisible)
{
	if (m_pParent)
	{
		for (int i = 0; i < MAX_MACHINEGUN; ++i)
		{
			if (m_pWeapon[i]) {
				m_pWeapon[i]->SetVisible(bVisible);
			}
		}
	}
}

TPartsInfo* GetMachinegunInitInfo()
{
	return &g_partsInit;
}