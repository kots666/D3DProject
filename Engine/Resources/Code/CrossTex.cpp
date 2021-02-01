#include "CrossTex.h"

USING(Engine)

CCrossTex::CCrossTex(LPDIRECT3DDEVICE9 device)
	: CVIBuffer(device)
{

}

CCrossTex::CCrossTex(const CCrossTex& rhs)
	: CVIBuffer(rhs)
{

}

CCrossTex::~CCrossTex()
{
}

HRESULT CCrossTex::Ready()
{
	m_vtxCnt = 8;
	m_triCnt = 4;
	m_vtxSize = sizeof(VTXTEX);
	m_FVF = FVF_TEX;
	m_idxSize = sizeof(INDEX32);
	m_format = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready(), E_FAIL);

	VTXTEX*	vertices = NULL;

	m_VB->Lock(0, 0, (void**)&vertices, NULL);
	// 1���� : ��𼭺��� ��� ���ΰ�
	// 2���� : ���ڰ� 0�̸� ��ü ������ ��ٴ�.
	// 3���� : ���ڰ��� ���ؼ� ���ؽ� ���� �� ���� �� ù��° �ּҸ� ���´�.
	// 4���� : ��״� ���¸� ���� ����, ���� ������ ��� 0

	vertices[0].pos = _vec3(-0.5f, 0.5f, 0.f);
	vertices[0].texUV = _vec2(0.f, 0.f);

	vertices[1].pos = _vec3(0.5f, 0.5f, 0.f);
	vertices[1].texUV = _vec2(1.f, 0.f);

	vertices[2].pos = _vec3(0.5f, -0.5f, 0.f);
	vertices[2].texUV = _vec2(1.f, 1.f);

	vertices[3].pos = _vec3(-0.5f, -0.5f, 0.f);
	vertices[3].texUV = _vec2(0.f, 1.f);

	vertices[4].pos = _vec3(0.f, 0.5f, -0.5f);
	vertices[4].texUV = _vec2(0.f, 0.f);

	vertices[5].pos = _vec3(0.f, 0.5f, 0.5f);
	vertices[5].texUV = _vec2(1.f, 0.f);

	vertices[6].pos = _vec3(0.f, -0.5f, 0.5f);
	vertices[6].texUV = _vec2(1.f, 1.f);

	vertices[7].pos = _vec3(0.f, -0.5f, -0.5f);
	vertices[7].texUV = _vec2(0.f, 1.f);

	m_VB->Unlock();

	INDEX32* indices = NULL;

	m_IB->Lock(0, 0, (void**)&indices, NULL);

	indices[0] = { 0, 1, 2 };
	indices[1] = { 0, 2, 3 };
	indices[2] = { 4, 5, 6 };
	indices[3] = { 4, 6, 7 };

	m_IB->Unlock();

	return S_OK;
}


CCrossTex* CCrossTex::Create(LPDIRECT3DDEVICE9 device)
{
	CCrossTex*	instance = new CCrossTex(device);

	if (FAILED(instance->Ready()))
		SafeRelease(instance);

	return instance;
}

CComponent * CCrossTex::Clone()
{
	return new CCrossTex(*this);
}

void CCrossTex::Free()
{
	CVIBuffer::Free();
}

