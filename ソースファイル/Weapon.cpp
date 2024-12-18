#include "Weapon.h"
#include "Enemy.h"
#include "Scene.h"


CWeapon::CWeapon(CScene* pScene) : CModel(pScene)
{
	m_pTarget = nullptr;
	m_EfkHandle = nullptr;
	m_nEffectCnt = 0;
}

CWeapon::~CWeapon()
{

}

bool CWeapon::SerchTarget()
{
	float vecTargetLen;
	float fZAngle;
	CEnemy* pEnemy;
	float fDist;

	pEnemy = (CEnemy*)m_pScene->Find(GOT_ENEMY);

	// ワールド変換行列取得
	XMFLOAT4X4 mW = GetWorld();
	XMFLOAT3 vZ = XMFLOAT3(mW._31, mW._32, mW._33);
	// 座標取得、クリア
	m_vPos = XMFLOAT3(mW._41, mW._42, mW._43);
	

	if (m_pTarget == nullptr) {
		while (pEnemy) {
			if (pEnemy->GetVisible()) {
				XMFLOAT3& vPos = pEnemy->GetPos();
				fDist = FLT_MAX;
				XMFLOAT3 vTarget;
				vTarget.x = vPos.x - m_vPos.x;
				vTarget.y = vPos.y - m_vPos.y;
				vTarget.z = vPos.z - m_vPos.z;
				XMVECTOR vecTarget = XMLoadFloat3(&vTarget);
				// ターゲットへのベクトルの長さ取得
				XMStoreFloat(&vecTargetLen, XMVector3Length(vecTarget));

				// 正規化
				vecTarget = XMVector3Normalize(vecTarget);
				// 追跡範囲内（ミサイル前方 45度の円錐以内）にいる場合のみ追跡
				fZAngle = XMVectorGetX(XMVector3AngleBetweenNormals(XMLoadFloat3(&vZ), vecTarget));

				if (fZAngle <= 3.14159265359f / 4.0f &&/*dist*/vecTargetLen <= 2500.0f) {
					if (!m_pTarget) {
						fDist = vecTargetLen/*dist*/;
						m_pTarget = pEnemy;
					}
					else {
						if (fDist > vecTargetLen/*dist*/) {
							fDist = vecTargetLen/*dist*/;
							m_pTarget = pEnemy;
						}
					}
				}
			}
			pEnemy = (CEnemy*)m_pScene->Find(GOT_ENEMY, pEnemy);
		}
	}

	if (m_pTarget)
	{
		return true;
	}
	else
	{
		return false;
	}
}