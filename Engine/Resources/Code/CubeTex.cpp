#include "CubeTex.h"

USING(Engine)

CCubeTex::CCubeTex(LPDIRECT3DDEVICE9 device)
	: CVIBuffer(device)
{

}

CCubeTex::CCubeTex(const CCubeTex& rhs)
	: CVIBuffer(rhs)
{

}

CCubeTex::~CCubeTex()
{

}

HRESULT CCubeTex::Ready()
{
	m_FVF = FVF_CUBE;
	m_triCnt = 12;
	m_vtxCnt = 8;
	m_vtxSize = sizeof(VTXCUBE);

	m_idxSize = sizeof(INDEX32);
	m_format = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready(), E_FAIL);

	VTXCUBE* vertices = NULL;

	m_VB->Lock(0, 0, (void**)&vertices, NULL);
	
	// 전면
	vertices[0].pos = _vec3(-0.5f, 0.5f, -0.5f);
	vertices[0].texUV = vertices[0].pos;

	vertices[1].pos = _vec3(0.5f, 0.5f, -0.5f);
	vertices[1].texUV = vertices[1].pos;

	vertices[2].pos = _vec3(0.5f, -0.5f, -0.5f);
	vertices[2].texUV = vertices[2].pos;

	vertices[3].pos = _vec3(-0.5f, -0.5f, -0.5f);
	vertices[3].texUV = vertices[3].pos;

	// 후면
	vertices[4].pos = _vec3(-0.5f, 0.5f, 0.5f);
	vertices[4].texUV = vertices[4].pos;

	vertices[5].pos = _vec3(0.5f, 0.5f, 0.5f);
	vertices[5].texUV = vertices[5].pos;

	vertices[6].pos = _vec3(0.5f, -0.5f, 0.5f);
	vertices[6].texUV = vertices[6].pos;

	vertices[7].pos = _vec3(-0.5f, -0.5f, 0.5f);
	vertices[7].texUV = vertices[7].pos;

	m_VB->Unlock();

	INDEX32* indices = nullptr;

	m_IB->Lock(0, 0, (void**)&indices, 0);

	// x+
	indices[0]._0 = 1;
	indices[0]._1 = 5;
	indices[0]._2 = 6;

	indices[1]._0 = 1;
	indices[1]._1 = 6;
	indices[1]._2 = 2;

	// x-
	indices[2]._0 = 4;
	indices[2]._1 = 0;
	indices[2]._2 = 3;

	indices[3]._0 = 4;
	indices[3]._1 = 3;
	indices[3]._2 = 7;

	// y+
	indices[4]._0 = 4;
	indices[4]._1 = 5;
	indices[4]._2 = 1;

	indices[5]._0 = 4;
	indices[5]._1 = 1;
	indices[5]._2 = 0;

	// y-
	indices[6]._0 = 3;
	indices[6]._1 = 2;
	indices[6]._2 = 6;

	indices[7]._0 = 3;
	indices[7]._1 = 6;
	indices[7]._2 = 7;

	// z+
	indices[8]._0 = 7;
	indices[8]._1 = 6;
	indices[8]._2 = 5;

	indices[9]._0 = 7;
	indices[9]._1 = 5;
	indices[9]._2 = 4;

	// z-
	indices[10]._0 = 0;
	indices[10]._1 = 1;
	indices[10]._2 = 2;

	indices[11]._0 = 0;
	indices[11]._1 = 2;
	indices[11]._2 = 3;

	m_IB->Unlock();

	return S_OK;
}


CCubeTex* CCubeTex::Create(LPDIRECT3DDEVICE9 device)
{
	CCubeTex* instance = new CCubeTex(device);

	if (FAILED(instance->Ready()))
		SafeRelease(instance);

	return instance;
}

CComponent * CCubeTex::Clone()
{
	return new CCubeTex(*this);
}

void CCubeTex::Free()
{
	CVIBuffer::Free();
}

