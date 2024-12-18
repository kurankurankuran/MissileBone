#pragma once
#include "main.h"

#define	VIEW_ANGLE			(45.0f)										// 視野角
#define	VIEW_ASPECT			((float)SCREEN_WIDTH/(float)SCREEN_HEIGHT)	// アスペクト比
#define	VIEW_NEAR_Z			(1.0f)										// NearZ値
#define	VIEW_FAR_Z			(50000.0f)									// FarZ値

enum ECameraMode {
	STATIC_CAMERAMODE = 0,// 定点カメラ
	FPS_CAMERAMODE,// 一人称カメラ
	BULLET_CAMERAMODO,// バレットタイム風カメラ
	TPS_CAMERAMODE,// 三人称カメラ
	TPS_BUCK_CAMERAMODE,// 三人称背後カメラ
	BULLET_FREE_CAMERAMODE,// バレットタイム風カメラ(自由箇所)

	MAX_CAMERAMODE
};


class CAssimpModel;
class CPlayer;
class CGameObj;
class CCamera
{
private:
	DirectX::XMFLOAT3 m_vPos;		// 視点
	DirectX::XMFLOAT3 m_vTarget;	// 注視点
	DirectX::XMFLOAT3 m_vUp;		// 上方ベクトル

	float m_fAspectRatio;			// 縦横比
	float m_fFovY;					// 視野角(Degree)
	float m_fNearZ;					// 前方クリップ距離
	float m_fFarZ;					// 後方クリップ距離

	DirectX::XMFLOAT4X4 m_mtxWorld;	// ワールド マトリックス
	DirectX::XMFLOAT4X4 m_mtxView;	// ビュー マトリックス
	DirectX::XMFLOAT4X4 m_mtxProj;	// プロジェクション マトリックス

	//DirectX::XMFLOAT3 m_vAngle;		// カメラの角度
	//float m_fLengthInterval;		// カメラの視点と注視点の距離

	CAssimpModel* m_pSky;			// スカイ ドーム

	float m_fRadius;				// 半径

	CPlayer* m_pPlayer;				// 自機
	int m_nMode;					// カメラモード
	float m_fAngle;					// カメラアングル

	DirectX::XMFLOAT4 m_frus[6];	// 視錐台
	DirectX::XMFLOAT4 m_frusw[6];	// 視錐台(ワールド空間)

	DirectX::XMFLOAT3 m_fRotValue;

	
	DirectX::XMFLOAT2 m_fFreeBulletMode;
	//CGameObj* m_pTargetObj;
public:
	CCamera();

	virtual void Init();
	virtual void Update();
	virtual void Clear();

	void SetPos(DirectX::XMFLOAT3& vPos) { m_vPos = vPos; }
	DirectX::XMFLOAT3& GetPos() { return m_vPos; }
	void SetPos(float x, float y, float z) { m_vPos.x = x, m_vPos.y = y, m_vPos.z = z; }
	void SetTarget(DirectX::XMFLOAT3& vTarget) { m_vTarget = vTarget; }
	void SetTarget(float x, float y, float z) { m_vTarget.x = x, m_vTarget.y = y, m_vTarget.z = z; }
	XMFLOAT3 GetTargetPos() { return m_vTarget; }
	void SetUpVector(DirectX::XMFLOAT3& vUp) { m_vUp = vUp; }
	void SetUpVector(float x, float y, float z) { m_vUp.x = x, m_vUp.y = y, m_vUp.z = z; }
	XMFLOAT3 GetUpVector() { return m_vUp; }
	void SetWorldMatrix(DirectX::XMFLOAT4X4& mtxWorld);
	DirectX::XMFLOAT4X4& GetWorldMatrix() { return m_mtxWorld; }
	DirectX::XMFLOAT4X4& CalcWorldMatrix();
	void SetFovY(float fFovY);
	void SetAspectRatio(float fAspect);
	void SetRangeZ(float fNearZ, float fFarZ);
	void UpdateMatrix();
	DirectX::XMFLOAT4X4& GetViewMatrix() { return m_mtxView; }
	DirectX::XMFLOAT4X4& GetProjMatrix() { return m_mtxProj; }
	//DirectX::XMFLOAT3& GetAngle() { return m_vAngle; }
	void SetSkyDome(CAssimpModel* pSky) { m_pSky = pSky; }
	CAssimpModel* GetSkyDome() { return m_pSky; }
	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	void SetPlayer(CPlayer* pPlayer);
	int CollisionViewFrustum(XMFLOAT3 vCenter,float fRadius);//可視判定(0:非表示,-1:部分, 1:表示)
	void SetMode(int nMode) { m_nMode = nMode; }
	int GetMode() { return m_nMode; }
	//void SetTargetObj(CGameObj* pObj) { m_pTargetObj = pObj; }
	//CGameObj* GetTargetObj() { return m_pTargetObj; }
	void SetFreeBulletMode(XMFLOAT2 fFreeBulletMode) { m_fFreeBulletMode = fFreeBulletMode; } // フリーバレットモード時の半径とカメラの高さを設定
private:
	void CreateViewFrustum();
	void UpdateViewFrustum();
};
