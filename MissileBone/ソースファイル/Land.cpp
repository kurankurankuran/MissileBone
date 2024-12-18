// 地面 [Land.cpp]
#include "Land.h"
#include "DebugProc.h"

#ifdef _DEBUG
int nNum = -1;
#endif // !_DEBUG


// コンストラクタ
CLand::CLand(CScene* pScene) : CModel(pScene)
{
	SetID(GOT_LAND);

	m_pVertex = nullptr;
	m_nVertex = 0;
	m_pIndex = nullptr;
	m_nIndex = 0;

}

// デストラクタ
CLand::~CLand()
{
}

// 初期化
HRESULT CLand::Init()
{
	SetModel(MODEL_LAND);

	// ワールド変換行列取得
	XMFLOAT4X4 mW = GetWorld();
	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTrans;
	mtxWorld = XMLoadFloat4x4(&mW);

	m_vScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vPos = XMFLOAT3(0.0f, -400.0f, 0.0f);

	mtxScl = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	mtxWorld = XMMatrixMultiply(mtxScl, mtxWorld);
	
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vAngle.x), XMConvertToRadians(m_vAngle.y), XMConvertToRadians(m_vAngle.z));
	mtxWorld = XMMatrixMultiply(mtxRot, mtxWorld);

	mtxTrans = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	mtxWorld = XMMatrixMultiply(mtxTrans, mtxWorld);
	
	XMStoreFloat4x4(&mW, mtxWorld);
	SetWorld(mW);


	// 頂点配列、インデックス配列を取得しておく
	CAssimpModel* pModel = GetAssimp(MODEL_LAND);
	pModel->GetVertexCount(&m_nVertex, &m_nIndex);
	m_pVertex = new TAssimpVertex[m_nVertex];
	m_pIndex = new UINT[m_nIndex];
	pModel->GetVertex(m_pVertex, m_pIndex);

	return CModel::Init();
}

// 終了処理
void CLand::Fin()
{
	CModel::Fin();

	// 頂点配列、インデックス配列の解放
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);

}

void CLand::Update()  // 更新処理
{
	// ワールド変換行列取得
	XMFLOAT4X4 mW = GetWorld();
	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTrans;
	mtxWorld = /*XMLoadFloat4x4(&mW);*/XMMatrixIdentity();
	mtxScl = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	mtxWorld = XMMatrixMultiply(mtxScl, mtxWorld);

	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vAngle.x), XMConvertToRadians(m_vAngle.y), XMConvertToRadians(m_vAngle.z));
	mtxWorld = XMMatrixMultiply(mtxRot, mtxWorld);

	mtxTrans = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	mtxWorld = XMMatrixMultiply(mtxTrans, mtxWorld);

	XMStoreFloat4x4(&mW, mtxWorld);
	SetWorld(mW);

	CModel::Update();
}

void CLand::Draw()
{
	int nCurCullNum = GetCurCullMode();
	if (nCurCullNum != MAX_CULLMODE)
	{
		SetCullMode(CULLMODE_NONE);
	}	
	CModel::Draw();	
	SetCullMode(nCurCullNum);
	
}

// レイとの当たり判定
bool CLand::Collision(XMFLOAT3 vP0, XMFLOAT3 vW, XMFLOAT3* pX, XMFLOAT3* pN)
{
	float fDistance = 999999.0f;
	bool bCol = false;
	XMFLOAT3 X;
	XMVECTOR N;

	

	// 全ての三角形について
	for
		(UINT i = 0; i < m_nIndex; i += 3) {
		// 三角形の頂点座標
		XMFLOAT3 V0 = m_pVertex[m_pIndex[i + 0]].vPos;
		XMFLOAT3 V1 = m_pVertex[m_pIndex[i + 1]].vPos;
		XMFLOAT3 V2 = m_pVertex[m_pIndex[i + 2]].vPos;

		/*
		XMVECTOR vecV0 = XMVector3TransformCoord(XMVectorSet(V0.x, V0.y, V0.z, 1.0f), XMLoadFloat4x4(&m_mWorld));
		XMVECTOR vecV1 = XMVector3TransformCoord(XMVectorSet(V1.x, V1.y, V1.z, 1.0f), XMLoadFloat4x4(&m_mWorld));
		XMVECTOR vecV2 = XMVector3TransformCoord(XMVectorSet(V2.x, V2.y, V2.z, 1.0f), XMLoadFloat4x4(&m_mWorld));

		XMStoreFloat3(&V0, vecV0);
		XMStoreFloat3(&V1, vecV1);
		XMStoreFloat3(&V2, vecV2);
		*/
		V0.x *= m_vScale.x; V0.y *= m_vScale.y; V0.z *= m_vScale.z;
		V1.x *= m_vScale.x; V1.y *= m_vScale.y; V1.z *= m_vScale.z;
		V2.x *= m_vScale.x; V2.y *= m_vScale.y; V2.z *= m_vScale.z;

		V0.x += m_vPos.x; V0.y += m_vPos.y; V0.z += m_vPos.z;
		V1.x += m_vPos.x; V1.y += m_vPos.y; V1.z += m_vPos.z;
		V2.x += m_vPos.x; V2.y += m_vPos.y; V2.z += m_vPos.z;

		// 辺に対応するベクトル
		XMFLOAT3 V0V1(V1.x - V0.x, V1.y - V0.y, V1.z - V0.z);
		XMFLOAT3 V1V2(V2.x - V1.x, V2.y - V1.y, V2.z - V1.z);
		XMFLOAT3 V2V0(V0.x - V2.x, V0.y - V2.y, V0.z - V2.z);

		//XMFLOAT3 V1V0(V0.x - V1.x, V0.y - V1.y, V0.z - V1.z);
		//XMFLOAT3 V2V1(V1.x - V2.x, V1.y - V2.y, V1.z - V2.z);
		//XMFLOAT3 V0V2(V2.x - V0.x, V2.y - V0.y, V2.z - V0.z);
		// 三角形(=平面)の法線ベクトルを求める
		/*XMVECTOR*/ N = XMVector3Normalize(
			XMVector3Cross(XMLoadFloat3(&V0V1),
				XMLoadFloat3(&V1V2)));

		// 三角形の表側にいるか裏側にいるか
		// 三角形の重心
		XMFLOAT3 vCenter;
		vCenter.x = (V0.x + V1.x + V2.x) / 3.0f;
		vCenter.y = (V0.y + V1.y + V2.y) / 3.0f;
		vCenter.z = (V0.z + V1.z + V2.z) / 3.0f;
		float dot0;
		XMStoreFloat(&dot0, XMVector3Dot(N, XMVectorSet(
			vP0.x - vCenter.x,
			vP0.y - vCenter.y,
			vP0.z - vCenter.z,
			1.0f
		)/* XMLoadFloat3(&vP0)*/));


		float dot;

		if (dot0 < 0.0f)
		{
			N = -N;// 裏にいるなら法線逆

		/*	XMFLOAT3 vecW;
			XMStoreFloat3(&vecW, XMVector3Normalize(XMLoadFloat3(&vW)) * 10.0f);
			vP0.x += vecW.x;
			vP0.y += vecW.y;
			vP0.z += vecW.z;*/


			// tの式(t=(N・(V0-P0))/(N・W))の分母を求める
			float base;
			XMStoreFloat(&base, XMVector3Dot(N,
				XMLoadFloat3(&vW)));
			if (base == 0.0f)
			{
				continue; // 面とレイが平行
			}
			// 分子を求める
			XMFLOAT3 P0V0(V0.x - vP0.x, V0.y - vP0.y, V0.z - vP0.z);
			//float dot;
			XMStoreFloat(&dot, XMVector3Dot(N,
				XMLoadFloat3(&P0V0)));
			// tから交点を求める
			float t = dot / base;
			if (t < 0.0f)
				continue; // 交点が始点の後ろ

			X = XMFLOAT3(
				vP0.x + t * vW.x,
				vP0.y + t * vW.y,
				vP0.z + t * vW.z);
			// 内外判定
			XMFLOAT3 V0X(X.x - V0.x, X.y - V0.y, X.z - V0.z);
			XMStoreFloat(&dot, XMVector3Dot(N,
				XMVector3Cross(XMLoadFloat3(&V0X),
					XMLoadFloat3(&V0V1))));
			if (dot < 0.0f)
				continue;
			XMFLOAT3 V1X(X.x - V1.x, X.y - V1.y, X.z - V1.z);
			XMStoreFloat(&dot, XMVector3Dot(N,
				XMVector3Cross(XMLoadFloat3(&V1X),
					XMLoadFloat3(&V1V2))));
			if (dot < 0.0f)
				continue;
			XMFLOAT3 V2X(X.x - V2.x, X.y - V2.y, X.z - V2.z);
			XMStoreFloat(&dot, XMVector3Dot(N,
				XMVector3Cross(XMLoadFloat3(&V2X),
					XMLoadFloat3(&V2V0))));
			if (dot < 0.0f)
				continue;
		}
		else
		{
			// tの式(t=(N・(V0-P0))/(N・W))の分母を求める
			float base;
			XMStoreFloat(&base, XMVector3Dot(N,
				XMLoadFloat3(&vW)));
			if (base == 0.0f)
			{
				continue; // 面とレイが平行
			}
			// 分子を求める
			XMFLOAT3 P0V0(V0.x - vP0.x, V0.y - vP0.y, V0.z - vP0.z);
			/*float dot;*/
			XMStoreFloat(&dot, XMVector3Dot(N,
				XMLoadFloat3(&P0V0)));
			// tから交点を求める
			float t = dot / base;
			if (t < 0.0f)
				continue; // 交点が始点の後ろ

			X = XMFLOAT3(
				vP0.x + t * vW.x,
				vP0.y + t * vW.y,
				vP0.z + t * vW.z);
			// 内外判定
			XMFLOAT3 V0X(X.x - V0.x, X.y - V0.y, X.z - V0.z);
			XMStoreFloat(&dot, XMVector3Dot(N,
				XMVector3Cross(XMLoadFloat3(&V0V1),
					XMLoadFloat3(&V0X))));
			if (dot < 0.0f)
				continue;
			XMFLOAT3 V1X(X.x - V1.x, X.y - V1.y, X.z - V1.z);
			XMStoreFloat(&dot, XMVector3Dot(N,
				XMVector3Cross(XMLoadFloat3(&V1V2),
					XMLoadFloat3(&V1X))));
			if (dot < 0.0f)
				continue;
			XMFLOAT3 V2X(X.x - V2.x, X.y - V2.y, X.z - V2.z);
			XMStoreFloat(&dot, XMVector3Dot(N,
				XMVector3Cross(XMLoadFloat3(&V2V0),
					XMLoadFloat3(&V2X))));
			if (dot < 0.0f)
				continue;




			// 貫通しているか
			float dot1;
			float dot2;
			XMFLOAT3 vP1 = XMFLOAT3(
				(vP0.x + vW.x) - vCenter.x,
				(vP0.y + vW.y) - vCenter.y,
				(vP0.z + vW.z) - vCenter.z);
			XMFLOAT3 vP2 = XMFLOAT3(
				vP0.x - vCenter.x,
				vP0.y - vCenter.y,
				vP0.z - vCenter.z);
			XMStoreFloat(&dot1, XMVector3Dot(N, XMLoadFloat3(&vP2)));
			XMStoreFloat(&dot2, XMVector3Dot(N, XMLoadFloat3(&vP1)));
			dot = dot1 * dot2;

			if (dot > 0.0f) // 符号が異なると衝突している
			{
				continue;
			}
			float distance;
			XMStoreFloat(&distance, XMVector3Length(XMVectorSet(X.x - vP2.x, X.y - vP2.y, X.z - vP2.z, 1.0f)));

			if (fDistance < distance)
			{
				continue;
			}
			fDistance = distance;
			bCol = true;
		}



		//// 貫通しているか
		//float dot1;
		//float dot2;
		//XMFLOAT3 vP1 = XMFLOAT3(
		//	vP0.x + vW.x - m_vPos.x,
		//	vP0.y + vW.y - m_vPos.y,
		//	vP0.z + vW.z - m_vPos.z);
		//XMFLOAT3 vP2 = XMFLOAT3(
		//	vP0.x - m_vPos.x,
		//	vP0.y - m_vPos.y,
		//	vP0.z - m_vPos.z);
		//XMStoreFloat(&dot1, XMVector3Dot(N, XMLoadFloat3(&vP2)));
		//XMStoreFloat(&dot2, XMVector3Dot(N, XMLoadFloat3(&vP1)));
		//dot = dot1 * dot2;

		//if (dot > 0.0f) // 符号が異なると衝突している
		//{
		//	continue;
		//}
		//float distance;
		//XMStoreFloat(&distance, XMVector3Length(XMVectorSet(X.x - vP2.x, X.y - vP2.y, X.z - vP2.z, 1.0f)));

		//if (fDistance < distance)
		//{
		//	continue;
		//}
		//fDistance = distance;
		//bCol = true;

#ifdef _DEBUG
		nNum = i;




#endif // !_DEBUG

	}
	if (bCol) {
		// 当たり
		if (pX) *pX = X;
		if (pN) XMStoreFloat3(pN, N);

#ifdef _DEBUG
		CDebugProc::Print("ColMeshNumber:[%d]\n", nNum);
#endif // !_DEBUG

		return true;// 当たっている
	}


#ifdef _DEBUG
	CDebugProc::Print("ColMeshNumber:[%d]\n", nNum);

	if (nNum < 0)
	{
		nNum = 0;
	}
	XMFLOAT3 _V0 = m_pVertex[m_pIndex[nNum + 0]].vPos;
	XMFLOAT3 _V1 = m_pVertex[m_pIndex[nNum + 1]].vPos;
	XMFLOAT3 _V2 = m_pVertex[m_pIndex[nNum + 2]].vPos;

	_V0.x *= m_vScale.x; _V0.y *= m_vScale.y; _V0.z *= m_vScale.z;
	_V1.x *= m_vScale.x; _V1.y *= m_vScale.y; _V1.z *= m_vScale.z;
	_V2.x *= m_vScale.x; _V2.y *= m_vScale.y; _V2.z *= m_vScale.z;

	_V0.x += m_vPos.x; _V0.y += m_vPos.y; _V0.z += m_vPos.z;
	_V1.x += m_vPos.x; _V1.y += m_vPos.y; _V1.z += m_vPos.z;
	_V2.x += m_vPos.x; _V2.y += m_vPos.y; _V2.z += m_vPos.z;

	XMFLOAT3 _V0V1(_V1.x - _V0.x, _V1.y - _V0.y, _V1.z - _V0.z);
	XMFLOAT3 _V1V2(_V2.x - _V1.x, _V2.y - _V1.y, _V2.z - _V1.z);
	XMFLOAT3 _V2V0(_V0.x - _V2.x, _V0.y - _V2.y, _V0.z - _V2.z);

	N = XMVector3Normalize(
		XMVector3Cross(XMLoadFloat3(&_V0V1),
			XMLoadFloat3(&_V1V2)));

	XMFLOAT3 _vCenter;
	_vCenter.x = (_V0.x + _V1.x + _V2.x) / 3.0f;
	_vCenter.y = (_V0.y + _V1.y + _V2.y) / 3.0f;
	_vCenter.z = (_V0.z + _V1.z + _V2.z) / 3.0f;
	float _dot0;
	XMStoreFloat(&_dot0, XMVector3Dot(N, XMVectorSet(
		vP0.x - _vCenter.x,
		vP0.y - _vCenter.y,
		vP0.z - _vCenter.z,
		1.0f
	)));


	// tの式(t=(N・(V0-P0))/(N・W))の分母を求める
	float _base;
	XMStoreFloat(&_base, XMVector3Dot(N,
		XMLoadFloat3(&vW)));
	if (_base == 0.0f)
	{
		//continue; // 面とレイが平行
	}
	// 分子を求める
	XMFLOAT3 _P0V0(_V0.x - vP0.x, _V0.y - vP0.y, _V0.z - vP0.z);
	float _dot;
	XMStoreFloat(&_dot, XMVector3Dot(N,
		XMLoadFloat3(&_P0V0)));
	// tから交点を求める
	float t = _dot / _base;
	if (t < 0.0f)
	{
		//continue; // 交点が始点の後ろ
	}


	X = XMFLOAT3(
		vP0.x + t * vW.x,
		vP0.y + t * vW.y,
		vP0.z + t * vW.z);
	// 内外判定
	XMFLOAT3 _V0X(X.x - _V0.x, X.y - _V0.y, X.z - _V0.z);
	XMStoreFloat(&_dot, XMVector3Dot(N,
		XMVector3Cross(XMLoadFloat3(&_V0V1),
			XMLoadFloat3(&_V0X))));

	float _dot_0;
	_dot_0 = _dot;
	if (_dot < 0.0f)
	{
		//continue;
		
	}
	XMFLOAT3 _V1X(X.x - _V1.x, X.y - _V1.y, X.z - _V1.z);
	XMStoreFloat(&_dot, XMVector3Dot(N,
		XMVector3Cross(XMLoadFloat3(&_V1V2),
			XMLoadFloat3(&_V1X))));
	float _dot_1;
	_dot_1 = _dot;
	if (_dot < 0.0f)
	{
		//continue;
		int a;
		a = 0;
	}

	XMFLOAT3 _V2X(X.x - _V2.x, X.y - _V2.y, X.z - _V2.z);
	XMStoreFloat(&_dot, XMVector3Dot(N,
		XMVector3Cross(XMLoadFloat3(&_V2V0),
			XMLoadFloat3(&_V2X))));
	float _dot_2;
	_dot_2 = _dot;
	if (_dot < 0.0f)
	{
		//continue;
		int a;
		a = 0;
	}


#endif // !_DEBUG

	return false; // 当たっていない
}