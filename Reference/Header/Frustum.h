#ifndef Frustum_h__
#define Frustum_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFrustum : public CBase
{
private:
	explicit CFrustum(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFrustum(const CFrustum& rhs);
	virtual ~CFrustum();

public:
	LPDIRECT3DDEVICE9 GetDevice() { return m_device; }

public:
	HRESULT Ready();
	_bool IsInFrustumForObject(const _vec3* worldPos, const _float& radius);
	_bool IsInFrustum(const _vec3* pos);

private:
	LPDIRECT3DDEVICE9 m_device;
	_vec3 m_point[8];
	D3DXPLANE m_plane[6];

public:
	static CFrustum* Create(LPDIRECT3DDEVICE9 device);
	virtual void Free();

};

END

#endif // Frustum_h__
