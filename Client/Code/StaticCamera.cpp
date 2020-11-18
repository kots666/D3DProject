#include "stdafx.h"
#include "StaticCamera.h"
#include "Player.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 device) :
	Engine::CCamera(device)
{

}

CStaticCamera::~CStaticCamera()
{

}

HRESULT Client::CStaticCamera::Ready(const _vec3* eye, const _vec3* at, const _vec3* up, const _float& fovY, const _float& aspect, const _float& nearZ, const _float& farZ)
{
	m_eye = *eye;
	m_at = *at;
	m_up = *up;
	m_fovY = fovY;
	m_aspect = aspect;
	m_nearZ = nearZ;
	m_farZ = farZ;

	FAILED_CHECK_RETURN(Engine::CCamera::Ready(), E_FAIL);

	ShowCursor(false);
	
	return S_OK;
}

Client::_int Client::CStaticCamera::Update(const _float& deltaTime)
{
	
	
	_int ret = Engine::CCamera::Update(deltaTime);
	
	return ret;
}

HRESULT CStaticCamera::SetTarget(CGameObject * target)
{
	m_target = dynamic_cast<CPlayer*>(target);
	NULL_CHECK_RETURN(m_target, E_FAIL);

	return S_OK;
}

CStaticCamera * CStaticCamera::Create(LPDIRECT3DDEVICE9 device, const _vec3 * eye, const _vec3 * at, const _vec3 * up, const _float & fovY, const _float & aspect, const _float & nearZ, const _float & farZ)
{
	CStaticCamera*	instance = new CStaticCamera(device);

	if (FAILED(instance->Ready(eye, at, up, fovY, aspect, nearZ, farZ)))
		Client::SafeRelease(instance);

	return instance;
}

void CStaticCamera::Free()
{
	Engine::CCamera::Free();
}