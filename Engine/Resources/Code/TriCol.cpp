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
	// 1인자 : 어디서부터 잠글 것인가
	// 2인자 : 숫자가 0이면 전체 영역을 잠근다.
	// 3인자 : 인자값을 통해서 버텍스 버퍼 내 정점 중 첫번째 주소를 얻어온다.
	// 4인자 : 잠그는 형태를 묻는 인자, 정적 버퍼인 경우 0

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

