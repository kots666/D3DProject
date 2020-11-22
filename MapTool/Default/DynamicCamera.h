#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"

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
									const _float& nearZ = 1.f, 
									const _float& farZ = 1000.f);

private:
	_bool m_canClick = false;
	_bool m_isFix = true;

private:
	virtual void Free() override;
};

#endif
