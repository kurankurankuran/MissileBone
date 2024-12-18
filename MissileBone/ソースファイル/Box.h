// ’¼•û‘Ì [Box.h]
#pragma once
#include "Mesh.h"

class CBox : public CMesh
{
public:
	CBox();
	virtual ~CBox();

	HRESULT Init(XMFLOAT3 vBBox);
};
