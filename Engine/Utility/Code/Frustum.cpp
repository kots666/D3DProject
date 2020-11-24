#include "Frustum.h"

USING(Engine)

CFrustum::CFrustum(LPDIRECT3DDEVICE9 device) :
	m_device(device)
{
	SafeAddRef(m_device);
}

CFrustum::CFrustum(const CFrustum & rhs) :
	m_device(rhs.m_device)	
{
	for (_uint i = 0; i < 6; ++i)
		m_plane[i] = rhs.m_plane[i];

	for (_uint i = 0; i < 8; ++i)
		m_point[i] = rhs.m_point[i];

	SafeAddRef(m_device);
}

CFrustum::~CFrustum()
{

}

HRESULT CFrustum::Ready()
{
	m_point[0] = _vec3(-1.f, 1.f, 0.f);
	m_point[1] = _vec3(1.f, 1.f, 0.f);
	m_point[2] = _vec3(1.f, -1.f, 0.f);
	m_point[3] = _vec3(-1.f, -1.f, 0.f);

	m_point[4] = _vec3(-1.f, 1.f, 1.f);
	m_point[5] = _vec3(1.f, 1.f, 1.f);
	m_point[6] = _vec3(1.f, -1.f, 1.f);
	m_point[7] = _vec3(-1.f, -1.f, 1.f);

	return S_OK;
}

_bool CFrustum::IsInFrustumForObject(const _vec3* worldPos, const _float& radius)
{
	FAILED_CHECK_RETURN(Ready(), false);

	_matrix matProj, matView;
	m_device->GetTransform(D3DTS_PROJECTION, &matProj);
	m_device->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_point[i], &m_point[i], &matProj);
		D3DXVec3TransformCoord(&m_point[i], &m_point[i], &matView);
	}

	//x+
	D3DXPlaneFromPoints(&m_plane[0], &m_point[1], &m_point[5], &m_point[6]);

	//x-
	D3DXPlaneFromPoints(&m_plane[1], &m_point[4], &m_point[0], &m_point[3]);

	//y+
	D3DXPlaneFromPoints(&m_plane[2], &m_point[4], &m_point[5], &m_point[1]);

	//y-
	D3DXPlaneFromPoints(&m_plane[3], &m_point[3], &m_point[2], &m_point[6]);

	//z+
	D3DXPlaneFromPoints(&m_plane[4], &m_point[7], &m_point[6], &m_point[5]);

	//z-
	D3DXPlaneFromPoints(&m_plane[5], &m_point[0], &m_point[1], &m_point[2]);


	return IsInFrustum(worldPos);
}

_bool CFrustum::IsInFrustum(const _vec3* pos)
{
	_float result = 0.f;

	for (_uint i = 0; i < 6; ++i)
	{
		result = D3DXPlaneDotCoord(&m_plane[i], pos);
		
		if (result > 0.f)
			return false;
	}

	return true;
}

CFrustum* CFrustum::Create(LPDIRECT3DDEVICE9 device)
{
	CFrustum* instance = new CFrustum(device);

	if (FAILED(instance->Ready()))
		SafeRelease(instance);

	return instance;
}

void CFrustum::Free()
{
	SafeRelease(m_device);
}

