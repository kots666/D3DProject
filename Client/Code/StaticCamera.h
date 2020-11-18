#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "Camera.h"
#include "Define.h"

BEGIN(Client)

class CPlayer;

class CStaticCamera : public Engine::CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 device);
	virtual ~CStaticCamera();

public:
	HRESULT Ready(const _vec3* eye, const _vec3* at, const _vec3* up,	
				const _float& fovY, const _float& aspect,
				const _float& nearZ, const _float& farZ);

	virtual _int Update(const _float& deltaTime) override;

	HRESULT SetTarget(CGameObject* target);

public:
	static CStaticCamera* Create(LPDIRECT3DDEVICE9 device, 
									const _vec3* eye, 
									const _vec3* at, 
									const _vec3* up, 
									const _float& fovY = D3DXToRadian(60.f),
									const _float& aspect = _float(WINCX) / WINCY, 
									const _float& nearZ = 1.f, 
									const _float& farZ = 1000.f);

private:
	CPlayer* m_target = nullptr;

private:
	virtual void Free() override;
};

END
#endif
