#include "stdafx.h"
#include "DynamicCamera.h"
#include "TerrainTex.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 device) :
	Engine::CCamera(device)
{

}

CDynamicCamera::~CDynamicCamera()
{

}

HRESULT CDynamicCamera::Ready(const _vec3* eye, const _vec3* at, const _vec3* up, const _float& fovY, const _float& aspect, const _float& nearZ, const _float& farZ)
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

_int CDynamicCamera::Update(const _float& deltaTime)
{
	KeyInput(deltaTime);

	if (m_isFix)
	{
		//MoveMouse();
		FixMouse();
	}
	
	_int ret = Engine::CCamera::Update(deltaTime);
	
	return ret;
}

void CDynamicCamera::KeyInput(const _float& deltaTime)
{
	_matrix matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	if (GetAsyncKeyState('W') & 0x8000)
	{
		_vec3 look;
		memcpy(look, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3 length = *D3DXVec3Normalize(&look, &look) * 5.f * deltaTime;

		m_eye += length;
		m_at += length;
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		_vec3 look;
		memcpy(look, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3 length = *D3DXVec3Normalize(&look, &look) * 5.f * deltaTime;

		m_eye -= length;
		m_at -= length;
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		_vec3 right;
		memcpy(right, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3 length = *D3DXVec3Normalize(&right, &right) * 5.f * deltaTime;

		m_eye -= length;
		m_at -= length;
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		_vec3 right;
		memcpy(right, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3 length = *D3DXVec3Normalize(&right, &right) * 5.f * deltaTime;

		m_eye += length;
		m_at += length;
	}

	// 마우스 전환
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		m_canClick ^= true;
		m_isFix ^= true;

		if (m_isFix) ShowCursor(false);
		else ShowCursor(true);
	}
}

void CDynamicCamera::MoveMouse()
{
	_matrix matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_long mouseMove = 0;

	// 마우스를 상하로 움직일 때
	if (mouseMove = Engine::GetDIMouseMove(Engine::DIMS_Y))
	{
		_vec3 right;
		memcpy(&right, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3 look = m_at - m_eye;
		_matrix matRot;
		_float degree = D3DXToDegree(m_fovY);
		_float yGap = WINCY / degree;

		D3DXMatrixRotationAxis(&matRot, &right, D3DXToRadian(mouseMove / yGap));

		D3DXVec3TransformNormal(&look, &look, &matRot);

		m_at = m_eye + look;
	}

	// 마우스를 좌우로 움직일 때
	if (mouseMove = Engine::GetDIMouseMove(Engine::DIMS_X))
	{
		_vec3 up = { 0.f, 1.f, 0.f };

		_vec3 look = m_at - m_eye;
		_matrix matRot;
		_float degree = D3DXToDegree(m_fovY);
		_float xGap = WINCX / degree;

		D3DXMatrixRotationAxis(&matRot, &up, D3DXToRadian(mouseMove / xGap));

		D3DXVec3TransformNormal(&look, &look, &matRot);

		m_at = m_eye + look;
	}
}

void CDynamicCamera::FixMouse()
{
	POINT ptMouse{ WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

CDynamicCamera * CDynamicCamera::Create(LPDIRECT3DDEVICE9 device, const _vec3 * eye, const _vec3 * at, const _vec3 * up, const _float & fovY, const _float & aspect, const _float & nearZ, const _float & farZ)
{
	CDynamicCamera*	instance = new CDynamicCamera(device);

	if (FAILED(instance->Ready(eye, at, up, fovY, aspect, nearZ, farZ)))
		SafeRelease(instance);

	return instance;
}

void CDynamicCamera::Free()
{
	Engine::CCamera::Free();
}