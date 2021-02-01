#ifndef CrossTex_h__
#define CrossTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCrossTex : public CVIBuffer
{
protected:
	explicit CCrossTex(LPDIRECT3DDEVICE9 device);
	explicit CCrossTex(const CCrossTex& rhs);
	virtual ~CCrossTex();

public:
	virtual HRESULT	Ready();

public:
	static CCrossTex* Create(LPDIRECT3DDEVICE9 device);
	CComponent* Clone();
	virtual void Free();
};
END

#endif
