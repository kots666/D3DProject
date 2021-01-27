#include "stdafx.h"
#include "DynamicCamera.h"
#include "Player.h"
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

	m_direction = { 0.f, 0.f, 1.f };
	
	return S_OK;
}

_int CDynamicCamera::Update(const _float& deltaTime)
{
	KeyInput(deltaTime);

	if (m_isFix)
	{
		MoveMouse();
		FixMouse();
	}
	
	return 0;
}

_int CDynamicCamera::LateUpdate(const _float & deltaTime)
{
	Engine::CTransform* playerTransCom = dynamic_cast<Engine::CTransform*>(Engine::GetComponent(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	if (nullptr == playerTransCom) return 0;

	_vec3 playerPos;
	playerTransCom->GetInfo(Engine::INFO_POS, &playerPos);
	playerPos.y += 1.5f;

	_vec3 dir = { 0.f, 0.f, 1.f };
	_float dist = 6.f;

	_matrix rotXMat, rotYMat;
	D3DXMatrixRotationX(&rotXMat, D3DXToRadian(m_xDegree));
	D3DXMatrixRotationY(&rotYMat, D3DXToRadian(m_yDegree));

	rotXMat *= rotYMat;

	D3DXVec3TransformNormal(&dir, &dir, &rotXMat);
	D3DXVec3Normalize(&dir, &dir);

	m_at = playerPos;

	m_eye = playerPos - (dir * dist);

	UpdateShake(deltaTime);

	_int ret = Engine::CCamera::Update(deltaTime);

	return ret;
}

void CDynamicCamera::ShakeCamera(const _float & duration, const _float & magnitude, const _bool& isForced)
{
	if (!m_isShake)
	{
		m_isShake = true;

		m_shakeDuration = duration;
		m_shakeMagnitude = magnitude;
	}

	if (isForced)
	{
		m_isShake = true;

		m_shakeDuration = duration;
		m_shakeMagnitude = magnitude;
	}
}

void CDynamicCamera::UpdateShake(const _float& deltaTime)
{
	if (m_isShake)
	{
		m_shakeDuration -= deltaTime;

		if (0 >= m_shakeDuration)
		{
			m_isShake = false;
			m_shakeDuration = 0.f;
			m_shakeMagnitude = 0.f;
			return;
		}

		_vec3 dir = m_at - m_eye;
		D3DXVec3Normalize(&dir, &dir);

		_vec3 up = m_up;
		D3DXVec3Normalize(&up, &up);

		_vec3 right;
		D3DXVec3Cross(&right, &up, &dir);

		_vec3 randVec = right * (((rand() % 21) / 10.f) - 1.f);

		m_eye += randVec * m_shakeMagnitude;

		randVec = up * (((rand() % 21) / 10.f) - 1.f);

		m_eye += randVec * m_shakeMagnitude;
	}
}

void CDynamicCamera::KeyInput(const _float& deltaTime)
{
	// 마우스 전환
	if (Engine::GetDIKeyDownState(VK_LCONTROL))
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
	_float mouseSense = 60.f;

	// 마우스를 상하로 움직일 때
	if (mouseMove = Engine::GetDIMouseMove(Engine::DIMS_Y))
	{
		m_xDegree += mouseSense * _float(mouseMove) / WINCY;

		if (25.f > m_xDegree)
		{
			m_xDegree = 25.f;
		}

		if (50.f < m_xDegree)
		{
			m_xDegree = 50.f;
		}
	}

	// 마우스를 좌우로 움직일 때
	if (mouseMove = Engine::GetDIMouseMove(Engine::DIMS_X))
	{
		m_yDegree += mouseSense * _float(mouseMove) / WINCX;
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
		Client::SafeRelease(instance);

	return instance;
}

void CDynamicCamera::Free()
{
	Engine::CCamera::Free();
}