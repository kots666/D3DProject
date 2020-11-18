#include "TriCol.h"

USING(Engine)
CTriCol::CTriCol(LPDIRECT3DDEVICE9 device)
	: CVIBuffer(device)
{

}

CTriCol::CTriCol(const CTriCol& rhs)
	: CVIBuffer(rhs)
{

}

CTriCol::~CTriCol()
{
}

HRESULT CTriCol::Ready()
{
	m_vtxCnt = 3;
	m_triCnt = 1;
	m_vtxSize = sizeof(VTXCOL);
	m_FVF = FVF_COL;

	m_idxSize = sizeof(INDEX16);
	m_format = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready(), E_FAIL);

	VTXCOL*	vertices = NULL;

	m_VB->Lock(0, 0, (void**)&vertices, NULL);
	// 1���� : ��𼭺��� ��� ���ΰ�
	// 2���� : ���ڰ� 0�̸� ��ü ������ ��ٴ�.
	// 3���� : ���ڰ��� ���ؼ� ���ؽ� ���� �� ���� �� ù��° �ּҸ� ���´�.
	// 4���� : ��״� ���¸� ���� ����, ���� ������ ��� 0

	vertices[0].pos = _vec3(0.f, 0.5f, 0.f);
	vertices[0].color = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	vertices[1].pos = _vec3(0.5f, -0.5f, 0.f);
	vertices[1].color = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	vertices[2].pos = _vec3(-0.5f, -0.5f, 0.f);
	vertices[2].color = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	m_VB->Unlock();

	INDEX16* indices = NULL;

	m_IB->Lock(0, 0, (void**)&indices, NULL);

	indices[0] = { 0, 1, 2 };

	m_IB->Unlock();

	return S_OK;
}


CTriCol* CTriCol::Create(LPDIRECT3DDEVICE9 device)
{
	CTriCol*	instance = new CTriCol(device);

	if (FAILED(instance->Ready()))
		SafeRelease(instance);

	return instance;
}

CComponent * CTriCol::Clone()
{
	return new CTriCol(*this);
}

void CTriCol::Free()
{
	CVIBuffer::Free();
}

