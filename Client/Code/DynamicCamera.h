#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"
#include "Define.h"

BEGIN(Client)

class CDynamicCamera : public Engine::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 device);
	virtual ~CDynamicCamera();

public:
	HRESULT Ready(const _vec3* eye, const _vec3* at, const _vec3* up,	
				const _float& fovY, const _float& aspect,
				const _float& nearZ, const _float& farZ);

	virtual _int Update(const _float& deltaTime) override;
	virtual _int LateUpdate(const _float& deltaTime) override;

public:
	void ShakeCamera(const _float& duration = 0.2f, const _float& magnitude = 0.2f, const _bool& isForced = false);
	void UpdateShake(const _float& deltaTime);

public:
	_float GetXDegree() { return m_xDegree; }
	_float GetYDegree() { return m_yDegree; }
	_vec3 GetDirection() { m_direction; }

private:
	void KeyInput(const _float& deltaTime);

	void MoveMouse();
	void FixMouse();

public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 device, 
									const _vec3* eye, 
									const _vec3* at, 
									const _vec3* up, 
									const _float& fovY = D3DXToRadian(60.f),
									const _float& aspect = _float(WINCX) / WINCY, 
									const _float& nearZ = 0.1f, 
									const _float& farZ = 1000.f);

private:
	_bool m_canClick = false;
	_bool m_isFix = true;

	_float m_xDegree = 30.f;
	_float m_yDegree = 0.f;

	_float m_shakeDuration = 0.f;
	_float m_shakeMagnitude = 0.f;
	_bool m_isShake = false;

	_vec3 m_direction;

private:
	virtual void Free() override;
};

END
#endif
