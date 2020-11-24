#include "Optimization.h"

USING(Engine)

COptimization::COptimization(LPDIRECT3DDEVICE9 device) :
	m_device(device), m_frustum(nullptr)
{
	SafeAddRef(m_device);
}

COptimization::COptimization(const COptimization& rhs) :
	m_device(rhs.m_device), m_frustum(rhs.m_frustum)
{
	SafeAddRef(m_device);
}

COptimization::~COptimization()
{

}

HRESULT COptimization::Ready()
{
	m_frustum = CFrustum::Create(m_device);
	NULL_CHECK_RETURN(m_frustum, E_FAIL);

	return S_OK;
}

_bool COptimization::IsInFrustumForObject(const _vec3* worldPos, const _float& radius)
{
	return m_frustum->IsInFrustumForObject(worldPos, radius);
}

COptimization* COptimization::Create(LPDIRECT3DDEVICE9 device)
{
	COptimization* instance = new COptimization(device);

	if (FAILED(instance->Ready()))
		SafeRelease(instance);
	
	return instance;
}

CComponent* COptimization::Clone()
{
	return new COptimization(*this);
}

void COptimization::Free()
{
	if (!m_isClone)
		SafeRelease(m_frustum);

	SafeRelease(m_device);
}

