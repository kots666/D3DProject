#include "Collider.h"

USING(Engine)

CCollider::CCollider(LPDIRECT3DDEVICE9 device) :
	m_device(device)
{
	SafeAddRef(m_device);
}

CCollider::CCollider(const CCollider& rhs)
	: m_device(rhs.m_device)
	, m_VB(rhs.m_VB)
	, m_IB(rhs.m_IB)
{
	for (_uint i = 0; i < COL_END; ++i)
		m_texture[i] = rhs.m_texture[i];
	
	SafeAddRef(m_VB);
	SafeAddRef(m_IB);
}

CCollider::~CCollider()
{

}

HRESULT CCollider::Ready(const _vec3* vtxPos, const _ulong& numVtxCnt, const _ulong& stride)
{
	// 인자로 받아들인 메쉬에 맞는 사이즈의 박스를 생성
	D3DXComputeBoundingBox(vtxPos, numVtxCnt, sizeof(_vec3), &m_minPos, &m_maxPos);


	FAILED_CHECK_RETURN(m_device->CreateVertexBuffer(sizeof(VTXCUBE) * 8,
		0, // 정적버퍼로 할당하겠다는 옵션
		FVF_CUBE,
		D3DPOOL_MANAGED,
		&m_VB,
		NULL),
		E_FAIL);

	FAILED_CHECK_RETURN(m_device->CreateIndexBuffer(sizeof(INDEX32) * 12,
		0, // 정적버퍼로 할당하겠다는 옵션
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&m_IB,
		NULL),
		E_FAIL);

	VTXCUBE* vertices = NULL;

	m_VB->Lock(0, 0, (void**)&vertices, NULL);

	// 전면
	vertices[0].pos = _vec3(m_minPos.x, m_maxPos.y, m_minPos.z);
	vertices[0].texUV = vertices[0].pos;

	vertices[1].pos = _vec3(m_maxPos.x, m_maxPos.y, m_minPos.z);
	vertices[1].texUV = vertices[1].pos;

	vertices[2].pos = _vec3(m_maxPos.x, m_minPos.y, m_minPos.z);
	vertices[2].texUV = vertices[2].pos;

	vertices[3].pos = _vec3(m_minPos.x, m_minPos.y, m_minPos.z);
	vertices[3].texUV = vertices[3].pos;

	// 후면
	vertices[4].pos = _vec3(m_minPos.x, m_maxPos.y, m_maxPos.z);
	vertices[4].texUV = vertices[4].pos;

	vertices[5].pos = _vec3(m_maxPos.x, m_maxPos.y, m_maxPos.z);
	vertices[5].texUV = vertices[5].pos;

	vertices[6].pos = _vec3(m_maxPos.x, m_minPos.y, m_maxPos.z);
	vertices[6].texUV = vertices[6].pos;

	vertices[7].pos = _vec3(m_minPos.x, m_minPos.y, m_maxPos.z);
	vertices[7].texUV = vertices[7].pos;

	m_VB->Unlock();

	INDEX32* indicies = nullptr;

	m_IB->Lock(0, 0, (void**)&indicies, 0);

	// x+
	indicies[0]._0 = 1;
	indicies[0]._1 = 5;
	indicies[0]._2 = 6;

	indicies[1]._0 = 1;
	indicies[1]._1 = 6;
	indicies[1]._2 = 2;

	// x-
	indicies[2]._0 = 4;
	indicies[2]._1 = 0;
	indicies[2]._2 = 3;

	indicies[3]._0 = 4;
	indicies[3]._1 = 3;
	indicies[3]._2 = 7;

	// y+
	indicies[4]._0 = 4;
	indicies[4]._1 = 5;
	indicies[4]._2 = 1;

	indicies[5]._0 = 4;
	indicies[5]._1 = 1;
	indicies[5]._2 = 0;

	// y-
	indicies[6]._0 = 3;
	indicies[6]._1 = 2;
	indicies[6]._2 = 6;

	indicies[7]._0 = 3;
	indicies[7]._1 = 6;
	indicies[7]._2 = 7;

	// z+
	indicies[8]._0 = 7;
	indicies[8]._1 = 6;
	indicies[8]._2 = 5;

	indicies[9]._0 = 7;
	indicies[9]._1 = 5;
	indicies[9]._2 = 4;

	// z-
	indicies[10]._0 = 0;
	indicies[10]._1 = 1;
	indicies[10]._2 = 2;

	indicies[11]._0 = 0;
	indicies[11]._1 = 2;
	indicies[11]._2 = 3;

	m_IB->Unlock();

	for (_uint i = 0; i < COL_END; ++i)
	{
		m_device->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_texture[i], NULL);

		D3DLOCKED_RECT		LockRect;
		ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

		m_texture[i]->LockRect(0, &LockRect, NULL, 0);

		*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f * i, 1.f * (1.f - i), 0.f, 1.f);

		m_texture[i]->UnlockRect(0);
	}
	

	return S_OK;
}

CCollider* CCollider::Create(LPDIRECT3DDEVICE9 device, const _vec3* vtxPos, const _ulong& numVtxCnt, const _ulong& stride)
{
	CCollider* instance = new CCollider(device);

	if (FAILED(instance->Ready(vtxPos, numVtxCnt, stride)))
		SafeRelease(instance);

	return instance;
}

void CCollider::Render(COLLTYPE type, const _matrix* colliderMatrix)
{
	m_collMatrix = *colliderMatrix;

#ifdef _DEBUG
	m_device->SetTransform(D3DTS_WORLD, colliderMatrix);

	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_device->SetTexture(0, m_texture[type]);

	m_device->SetStreamSource(0, m_VB, 0, sizeof(VTXCUBE));
	m_device->SetFVF(FVF_CUBE);
	m_device->SetIndices(m_IB);
	m_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_device->SetRenderState(D3DRS_LIGHTING, TRUE);
#endif
}

CComponent * CCollider::Clone()
{
	return new CCollider(*this);
}

void CCollider::Free()
{
	for (_uint i = 0; i < COL_END; ++i)
		SafeRelease(m_texture[i]);

	SafeRelease(m_VB);
	SafeRelease(m_IB);
	SafeRelease(m_device);
}

