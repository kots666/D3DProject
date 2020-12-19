#include "SphereCollider.h"

USING(Engine)

CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 device, _matrix * boneMat, const _vec3 & offset, const _float & radius) :
	m_device(device),
	m_attachBone(boneMat),
	m_offset(offset),
	m_radius(radius)
{
	SafeAddRef(m_device);
}

CSphereCollider::CSphereCollider(const CSphereCollider& rhs) :
	m_device(rhs.m_device),
	m_attachBone(rhs.m_attachBone),
	m_offset(rhs.m_offset),
	m_radius(rhs.m_radius)
{
	SafeAddRef(m_device);

	for (_uint i = 0; i < COL_END; ++i)
		m_texture[i] = rhs.m_texture[i];
}

CSphereCollider::~CSphereCollider()
{

}

HRESULT CSphereCollider::Ready()
{
	for (_uint i = 0; i < COL_END; ++i)
	{
		m_device->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_texture[i], NULL);

		D3DLOCKED_RECT LockRect;
		ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

		m_texture[i]->LockRect(0, &LockRect, NULL, 0);

		*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f * i, 1.f * (1.f - i), 0.f, 1.f);

		m_texture[i]->UnlockRect(0);
	}
	
	return S_OK;
}

void CSphereCollider::UpdateByBone()
{
	if (nullptr == m_attachBone) return;

	//m_localMat = *m_attachBone;

	_matrix tmpMat;
	D3DXMatrixIdentity(&tmpMat);
	tmpMat._41 = m_offset.x;
	tmpMat._42 = m_offset.y;
	tmpMat._43 = m_offset.z;

	m_localMat = tmpMat * *m_attachBone;
}

void CSphereCollider::Render(COLLTYPE type, _matrix * worldMat)
{
#ifdef _DEBUG

	LPD3DXMESH tmpMesh;

	D3DXCreateSphere(m_device, m_radius, 10, 10, &tmpMesh, nullptr);

	if (nullptr == tmpMesh) return;

	m_localMat *= *worldMat;

	m_device->SetTransform(D3DTS_WORLD, &m_localMat);
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_device->SetTexture(0, m_texture[type]);
	tmpMesh->DrawSubset(0);

	m_device->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

#endif

	tmpMesh->Release();
}

CSphereCollider * CSphereCollider::Create(LPDIRECT3DDEVICE9 device, _matrix * boneMat, const _vec3 & offset, const _float & radius)
{
	CSphereCollider* instance = new CSphereCollider(device, boneMat, offset, radius);

	if (FAILED(instance->Ready()))
		SafeRelease(instance);

	return instance;
}

CComponent * CSphereCollider::Clone()
{
	return new CSphereCollider(*this);
}

void CSphereCollider::Free()
{
	for (_uint i = 0; i < COL_END; ++i)
		SafeRelease(m_texture[i]);

	SafeRelease(m_device);
}

