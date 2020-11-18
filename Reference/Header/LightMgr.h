#ifndef LightMgr_h__
#define LightMgr_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Light.h"

BEGIN(Engine)

class ENGINE_DLL CLightMgr : public CBase
{
	DECLARE_SINGLETON(CLightMgr)
private:
	explicit CLightMgr();
	virtual ~CLightMgr();

public:
	HRESULT ReadyLight(LPDIRECT3DDEVICE9 device, const D3DLIGHT9* lightInfo, const _uint& index);

private:
	list<CLight*> m_lightList;

private:
	virtual void Free();
};

END
#endif // LightMgr_h__
