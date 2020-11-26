#include "RCTex.h"

USING(Engine)

CRCTex::CRCTex(LPDIRECT3DDEVICE9 device)
	: CVIBuffer(device)
{

}

CRCTex::CRCTex(const CRCTex& rhs)
	: CVIBuffer(rhs)
{

}

CRCTex::~CRCTex()
{
}

HRESULT CRCTex::Ready()
{
	m_vtxCnt = 4;
	m_triCnt = 2;
	m_vtxSize = sizeof(VTXTEX);
	m_FVF = FVF_TEX;
	m_idxSize = sizeof(INDEX32);
	m_format = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready(), E_FAIL);

	VTXTEX*	vertices = NULL;

	m_VB->Lock(0, 0, (void**)&vertices, NULL);
	// 1인자 : 어디서부터 잠글 것인가
	// 2인자 : 숫자가 0이면 전체 영역을 잠근다.
	// 3인자 : 인자값을 통해서 버텍스 버퍼 내 정점 중 첫번째 주소를 얻어온다.
	// 4인자 : 잠그는 형태를 묻는 인자, 정적 버퍼인 경우 0

	vertices[0].pos = _vec3(-0.5f, 0.5f, 0.f);
	vertices[0].texUV = _vec2(0.f, 0.f);

	vertices[1].pos = _vec3(0.5f, 0.5f, 0.f);
	vertices[1].texUV = _vec2(1.f, 0.f);

	vertices[2].pos = _vec3(0.5f, -0.5f, 0.f);
	vertices[2].texUV = _vec2(1.f, 1.f);

	vertices[3].pos = _vec3(-0.5f, -0.5f, 0.f);
	vertices[3].texUV = _vec2(0.f, 1.f);

	m_VB->Unlock();

	INDEX32* indices = NULL;

	m_IB->Lock(0, 0, (void**)&indices, NULL);

	indices[0] = { 0, 1, 2 };
	indices[1] = { 0, 2, 3 };

	m_IB->Unlock();

	return S_OK;
}


CRCTex* CRCTex::Create(LPDIRECT3DDEVICE9 device)
{
	CRCTex*	instance = new CRCTex(device);

	if (FAILED(instance->Ready()))
		SafeRelease(instance);

	return instance;
}

CComponent * CRCTex::Clone()
{
	return new CRCTex(*this);
}

void CRCTex::Free()
{
	CVIBuffer::Free();
}

