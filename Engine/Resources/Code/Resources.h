#ifndef Resources_h__
#define Resources_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CResources : public CComponent 
{
protected:
	explicit CResources(LPDIRECT3DDEVICE9 device);
	explicit CResources(const CResources& rhs);
	virtual ~CResources();

protected:
	LPDIRECT3DDEVICE9 m_device;

public:
	virtual void Free();
};

END

#endif
