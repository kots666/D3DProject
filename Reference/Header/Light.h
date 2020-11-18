#ifndef Light_h__
#define Light_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLight : public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 device);
	virtual ~CLight();
	
public:
	HRESULT Ready(const D3DLIGHT9* lightInfo, const _uint& index);

private:
	LPDIRECT3DDEVICE9 m_device;
	D3DLIGHT9 m_lightInfo;
	_uint m_index;

public:
	static CLight* Create(LPDIRECT3DDEVICE9 device, const D3DLIGHT9* lightInfo, const _uint& index);

private:
	virtual void Free();

};

END
#endif // Light_h__
