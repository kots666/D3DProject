#include "SphereCollider.h"

USING(Engine)

CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 device, _tchar* name, _matrix * boneMat, const _vec3 & offset, const _float & radius) :
	m_device(device),
	m_attachBone(boneMat),
	m_name(name),
	m_offset(offset),
	m_radius(radius),
	m_canCollide(false),
	m_isCollide(false),
	m_accTime(0.f)
{
	SafeAddRef(m_device);
}

CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 device, const _vec3 & pos, const _float & radius) :
	m_device(device),
	m_attachBone(nullptr),
	m_name(nullptr),
	m_offset(pos),
	m_radius(radius),
	m_canCollide(true),
	m_isCollide(false),
	m_accTime(0.f)
{
	SafeAddRef(m_device);
}

CSphereCollider::CSphereCollider(const CSphereCollider& rhs) :
	m_device(rhs.m_device),
	m_attachBone(rhs.m_attachBone),
	m_offset(rhs.m_offset),
	m_radius(rhs.m_radius),
	m_canCollide(rhs.m_canCollide),
	m_isCollide(rhs.m_isCollide),
	m_accTime(rhs.m_accTime)
{
	SafeAddRef(m_device);

	for (_uint i = 0; i < COL_END; ++i)
		m_texture[i] = rhs.m_texture[i];
}

CSphereCollider::~CSphereCollider()
{
	if (nullptr != m_name)
	{
		delete[] m_name;
		m_name = nullptr;
	}

	for (_uint i = 0; i < COL_END; ++i)
		SafeRelease(m_texture[i]);
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

	D3DXMatrixIdentity(&m_worldMat);

	m_scale = 0.01f;
	m_realRadius = m_scale * m_radius;
	
	return S_OK;
}

_int CSphereCollider::Update(const _float & deltaTime)
{
	if (m_isCollide)
	{
		m_accTime += deltaTime;

		if (m_accTime >= 1.f)
		{
			m_accTime = 0.f;
			m_isCollide = false;
		}
	}
	return 0;
}

void CSphereCollider::UpdateByBone(const _matrix* worldMat)
{
	_matrix tmpMat;
	D3DXMatrixIdentity(&tmpMat);
	tmpMat._41 = m_offset.x;
	tmpMat._42 = m_offset.y;
	tmpMat._43 = m_offset.z;

	if (nullptr == m_attachBone)
	{
		m_worldMat = tmpMat * *worldMat;
		return;
	}

	m_worldMat = tmpMat * *m_attachBone;

	if (nullptr == worldMat) return;

	m_worldMat *= *worldMat;
}

void CSphereCollider::Render()
{
#ifdef _DEBUG

	LPD3DXMESH tmpMesh;

	D3DXCreateSphere(m_device, m_radius, 10, 10, &tmpMesh, nullptr);

	if (nullptr == tmpMesh) return;

	m_device->SetTransform(D3DTS_WORLD, &m_worldMat);
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	if(m_isCollide)
		m_device->SetTexture(0, m_texture[1]);
	else
		m_device->SetTexture(0, m_texture[0]);
	
	tmpMesh->DrawSubset(0);

	m_device->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	tmpMesh->Release();
#endif
}

CSphereCollider * CSphereCollider::Create(LPDIRECT3DDEVICE9 device, const _vec3 & pos, const _float & radius)
{
	CSphereCollider* instance = new CSphereCollider(device, pos, radius);

	if (FAILED(instance->Ready()))
		SafeRelease(instance);

	return instance;
}

CSphereCollider * CSphereCollider::Create(LPDIRECT3DDEVICE9 device, _tchar* name, _matrix * boneMat, const _vec3 & offset, const _float & radius)
{
	CSphereCollider* instance = new CSphereCollider(device, name, boneMat, offset, radius);

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
	SafeRelease(m_device);
}

