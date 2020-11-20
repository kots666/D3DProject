#include "Camera.h"

USING(Engine)

CCamera::CCamera(LPDIRECT3DDEVICE9 device) :
	CGameObject(device)
{
	
}

CCamera::~CCamera()
{

}

HRESULT CCamera::Ready()
{
	D3DXMatrixLookAtLH(&m_matView, &m_eye, &m_at, &m_up);
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fovY, m_aspect, m_nearZ, m_farZ);
	
	m_device->SetTransform(D3DTS_VIEW, &m_matView);
	m_device->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

_int CCamera::Update(const _float& deltaTime)
{
	D3DXMatrixLookAtLH(&m_matView, &m_eye, &m_at, &m_up);
	m_device->SetTransform(D3DTS_VIEW, &m_matView);

	return 0;
}

void CCamera::Free()
{
	CGameObject::Free();
}

