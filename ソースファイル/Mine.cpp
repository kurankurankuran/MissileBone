// 地雷ミサイル [Mine.cpp]
#include "Mine.h"
#include "Scene.h"
#include "Enemy.h"
#include "Explosion.h"
#include "Game.h"
#include "Sound.h"
#include "EffectManager.h"

// 定数
namespace {
	const float g_fSpeed = 60.0f;
	const float g_fRotateSpeed = 1.0f;	// 単位:度
	const float g_fSpeedEnemy = 50.0f;

	const int g_nMaxLife = 600;

	const int g_nDamage = 20;
}



// コンストラクタ
CMine::CMine(CScene* pScene) : CWeapon(pScene)
{
	SetID(GOT_MISSILE);


}

// デストラクタ
CMine::~CMine()
{
}

// 初期化
HRESULT CMine::Init()
{
	SetModel(MODEL_MISSILE);
	m_nEffectCnt = 0;
	//m_bUnit = false;
	m_nLife = g_nMaxLife;
	//m_fSpeed = g_fSpeed;

	// ワールド変換行列取得
	XMFLOAT4X4 mW = GetWorld();
	// モデル座標Y軸取得
	XMFLOAT3 vY = XMFLOAT3(mW._21, mW._22, mW._23);
	// 座標取得、クリア
	m_vPos = XMFLOAT3(mW._41, mW._42, mW._43);
	mW._41 = mW._42 = mW._43 = 0.0f;

	m_vPos.x -= vY.x * 30.0f;
	m_vPos.y -= vY.y * 30.0f;
	m_vPos.z -= vY.z * 30.0f;

	// ワールド変換行列に反映
	mW._41 = m_vPos.x;
	mW._42 = m_vPos.y;
	mW._43 = m_vPos.z;
	SetWorld(mW);

	//m_Effect.Init(m_pScene);

	return CModel::Init();
}

void CMine::Fin()
{
	//m_Effect.Fin();
	CModel::Fin();
}

// 更新
void CMine::Update()
{
	if (!m_bUnit) {
		//// 爆発エフェクト更新
		//if (m_pExp) {
		//	if (m_uExpTimer) {
		//		--m_uExpTimer;
		//		if (!m_uExpTimer) {
		//			Fin();
		//			delete this;
		//			return;
		//		}
		//	}
		//	m_pExp->Update();
		//}
		if (!m_bVisible) {
			Fin();
			delete this;
			return;
		}

		if (!m_uExpTimer) {
			--m_nLife;
			if (m_nLife <= 0) {
				m_nLife = g_nMaxLife;
				//StartExplosion();
				m_bVisible = false;
				return;
			}
		}

		// ワールド変換行列取得
		XMFLOAT4X4 mW = GetWorld();
		// モデル座標軸取得
		XMFLOAT3 vX = XMFLOAT3(mW._11, mW._12, mW._13);
		XMFLOAT3 vY = XMFLOAT3(mW._21, mW._22, mW._23);
		XMFLOAT3 vZ = XMFLOAT3(mW._31, mW._32, mW._33);
		// 座標取得、クリア
		m_vPos = XMFLOAT3(mW._41, mW._42, mW._43);
		mW._41 = mW._42 = mW._43 = 0.0f;


		float vecTargetLen;
		float fZAngle;


		// ターゲットが有効?
		if (!m_pTarget || !m_pTarget->GetVisible()) {
			switch (m_Type) {
				CEnemy* pEnemy;
				CPlayer* pPlayer;
				float fDist;

			case GOT_ENEMY:
				m_fSpeed = g_fSpeed;


				pEnemy = (CEnemy*)m_pScene->Find(GOT_ENEMY);
				m_pTarget = nullptr;

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
					if (pEnemy->GetID() == GOT_BOSS)break;
					pEnemy = (CEnemy*)m_pScene->Find(GOT_ENEMY, pEnemy);
					if (!pEnemy) {
						pEnemy = (CEnemy*)m_pScene->Find(GOT_BOSS, pEnemy);
					}
				}
				break;
			case GOT_PLAYER:
				m_fSpeed = g_fSpeedEnemy;

				pPlayer = (CPlayer*)m_pScene->Find(GOT_PLAYER);
				m_pTarget = nullptr;

				if (pPlayer->GetVisible()) {
					XMFLOAT3& vPos = pPlayer->GetPos();

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

					if (fZAngle <= 3.14159265359f / 4.0f &&/*dist*/vecTargetLen <= 2500.0f)
						m_pTarget = pPlayer;

				}
				break;
			}
		}
		else {
			//ターゲットへのベクトルを取得
			XMFLOAT3& vTargetPos = m_pTarget->GetPos();
			XMFLOAT3 vTarget;
			vTarget.x = vTargetPos.x - m_vPos.x;
			vTarget.y = vTargetPos.y - m_vPos.y;
			vTarget.z = vTargetPos.z - m_vPos.z;


			XMVECTOR vecTarget = XMLoadFloat3(&vTarget);
			// ターゲットへのベクトルの長さ取得
			XMStoreFloat(&vecTargetLen, XMVector3Length(vecTarget));
			// 正規化
			vecTarget = XMVector3Normalize(vecTarget);



			// 追跡範囲内（ミサイル前方 60度の円錐以内）にいる場合のみ追跡
			fZAngle = XMVectorGetX(XMVector3AngleBetweenNormals(XMLoadFloat3(&vZ), vecTarget));

			if (fZAngle <= 3.14159265359f / 4.0f && vecTargetLen <= 2500.0f)
			{
				// ターゲットが上下左右どちらか
				XMMATRIX mR = XMMatrixIdentity();
				XMVECTOR axisX = XMLoadFloat3(&vX);
				XMVECTOR axisY = XMLoadFloat3(&vY);
				float fRight = vX.x * vTarget.x + vX.y * vTarget.y + vX.z * vTarget.z;	// 内積
				if (fRight > 0.0f)		// 右
					mR = XMMatrixRotationAxis(axisY, XMConvertToRadians(g_fRotateSpeed));
				else if (fRight < 0.0f)	// 左
					mR = XMMatrixRotationAxis(axisY, XMConvertToRadians(-g_fRotateSpeed));
				float fUp = vY.x * vTarget.x + vY.y * vTarget.y + vY.z * vTarget.z;		// 内積
				if (fUp > 0.0f)			// 上
					mR *= XMMatrixRotationAxis(axisX, XMConvertToRadians(-g_fRotateSpeed));
				else if (fUp < 0.0f)	// 下
					mR *= XMMatrixRotationAxis(axisX, XMConvertToRadians(g_fRotateSpeed));

				XMStoreFloat4x4(&mW, XMMatrixMultiply(XMLoadFloat4x4(&mW), mR));
			}
			else
			{
				m_pTarget = nullptr;
			}
		}




		// 前進
		if (m_pTarget) {
			m_vPos.x += vZ.x * m_fSpeed;
			m_vPos.y += vZ.y * m_fSpeed;
			m_vPos.z += vZ.z * m_fSpeed;
		}


		// ワールド変換行列に反映
		mW._41 = m_vPos.x;
		mW._42 = m_vPos.y;
		mW._43 = m_vPos.z;
		SetWorld(mW);

		// 衝突判定
		if (!m_uExpTimer) {
			if (m_pTarget) {
				switch (((CGame*)m_pScene)->GetBound()) {
				case BOUND_AABB:	// 境界ボックス
					if (CollisionAABB(m_pTarget)) {
						m_pTarget->AddLife(-g_nDamage);
						StartExplosion();
						CSound::Play(SE_DEFEND);

						/*if (m_pTarget->GetLife() <= 0) {

							m_pTarget->StartExplosion();
							m_pTarget->SetLife(m_pTarget->GetMaxLife());

							CSound::Play(SE_EXPLOSION);

						}*/
					}
					break;
				case BOUND_SPHERE:	// 境界球
					if (CollisionBS(m_pTarget)) {
						m_pTarget->AddLife(-g_nDamage);
						StartExplosion();
						CSound::Play(SE_DEFEND);

						/*if (m_pTarget->GetLife() <= 0) {

							m_pTarget->StartExplosion();
							m_pTarget->SetLife(m_pTarget->GetMaxLife());

							CSound::Play(SE_EXPLOSION);

						}*/
					}
					break;
					//case BOUND_OBB:     // OBB
					//	if (CollisionOBB(m_pTarget)) {
					//		m_pTarget->StartExplosion();
					//		StartExplosion();
					//	}
					//	break;
				default:
					if (CollisionOBB(m_pTarget)) {
						m_pTarget->AddLife(-g_nDamage);
						StartExplosion();
						CSound::Play(SE_DEFEND);

						/*if (m_pTarget->GetLife() <= 0) {

							m_pTarget->StartExplosion();
							m_pTarget->SetLife(m_pTarget->GetMaxLife());

							CSound::Play(SE_EXPLOSION);

						}*/
					}
					break;
				}
			}
		}


		++m_nEffectCnt;
		if (m_bVisible && !(m_nEffectCnt % 4)) {
			// スモークエフェクト
			XMFLOAT3 EffePos;
			EffePos = m_vPos;
			XMFLOAT3 EffeZ = XMFLOAT3(m_mWorld._31, m_mWorld._32, m_mWorld._33);
			EffePos.x -= EffeZ.x * 20;
			EffePos.y -= EffeZ.y * 20;
			EffePos.z -= EffeZ.z * 20;
			//m_Effect.SetEffect(SMOKE_EFFECT, EffePos, XMFLOAT2(20.0f, 20.0f));
			CEffectManager::StartEffect(EFFECT_SMOKE, EffePos, XMFLOAT2(20.0f, 20.0f),true);
		}

		//m_Effect.Update();
	}

}



void CMine::Draw()
{

	//m_Effect.Draw();
	

	CModel::Draw();
}

// 発射
int CMine::Fire(XMFLOAT4X4* pWorld, EGameObjType Type, CModel* pModel)
{
	CScene* pScene = CScene::GetCurrent();
	if (!pScene)
		return -1;
	CMine* pMissile = new CMine(pScene);
	pMissile->SetWorld(*pWorld);
	pMissile->Init();
	pMissile->m_pTarget = pModel;
	pMissile->m_Type = Type;

	pMissile->SetBSVisible((EBoundingShape)((CGame*)pScene)->GetBound());



	return 1;
}

int CMine::Fire()
{

	m_bUnit = false;

	return 1;
}

