#include "TrailRect.h"

USING(Engine)

CTrailRect::CTrailRect(LPDIRECT3DDEVICE9 device, const _vec3* vtxPos, const _vec2* texUV, const _float& lifeTime) :
	CVIBuffer(device),
	m_lifeTime(lifeTime)
{
	memcpy(m_vtxPos, vtxPos, sizeof(_vec3) * 4);
	memcpy(m_texUV, texUV, sizeof(_vec2) * 4);
}

CTrailRect::CTrailRect(const CTrailRect& rhs) :
	CVIBuffer(rhs),
	m_lifeTime(rhs.m_lifeTime)
{
	//memcpy(m_vtxPos, rhs.m_vtxPos, sizeof(m_vtxPos));
	//memcpy(m_texUV, rhs.m_texUV, sizeof(m_texUV));
}

CTrailRect::~CTrailRect()
{
}

HRESULT CTrailRect::Ready(const _vec3 * pos, const _vec2 * uv)
{
	m_vtxCnt = 4;
	m_triCnt = 2;
	m_vtxSize = sizeof(VTXTRAIL);
	m_FVF = FVF_TRAIL;
	m_idxSize = sizeof(INDEX32);
	m_format = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready(), E_FAIL);

	VTXTRAIL* vertices = NULL;

	m_VB->Lock(0, 0, (void**)&vertices, NULL);
	// 1인자 : 어디서부터 잠글 것인가
	// 2인자 : 숫자가 0이면 전체 영역을 잠근다.
	// 3인자 : 인자값을 통해서 버텍스 버퍼 내 정점 중 첫번째 주소를 얻어온다.
	// 4인자 : 잠그는 형태를 묻는 인자, 정적 버퍼인 경우 0

	vertices[0].pos = pos[0];
	vertices[0].texUV = uv[0];

	vertices[1].pos = pos[1];
	vertices[1].texUV = uv[1];

	vertices[2].pos = pos[2];
	vertices[2].texUV = uv[2];

	vertices[3].pos = pos[3];
	vertices[3].texUV = uv[3];

	m_VB->Unlock();

	INDEX32* indices = NULL;

	m_IB->Lock(0, 0, (void**)&indices, NULL);

	indices[0] = { 0, 1, 2 };
	indices[1] = { 0, 2, 3 };

	m_IB->Unlock();

	return S_OK;
}

HRESULT CTrailRect::ChangePos(const _vec3 * vtxPos, const _vec2 * texUV)
{
	return S_OK;
}

CTrailRect* CTrailRect::Create(LPDIRECT3DDEVICE9 device, const _vec3* vtxPos, const _vec2* texUV, const _float& lifeTime)
{
	CTrailRect*	instance = new CTrailRect(device, vtxPos, texUV, lifeTime);

	if (FAILED(instance->Ready(vtxPos, texUV)))
		SafeRelease(instance);

	return instance;
}

CComponent * CTrailRect::Clone()
{
	return new CTrailRect(*this);
}

void CTrailRect::Free()
{
	CVIBuffer::Free();
}

