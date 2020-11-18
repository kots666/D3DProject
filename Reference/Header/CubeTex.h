#ifndef CubeTex_h__
#define CubeTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex : public CVIBuffer
{
protected:
	explicit CCubeTex(LPDIRECT3DDEVICE9 device);
	explicit CCubeTex(const CCubeTex& rhs);
	virtual ~CCubeTex();

public:
	virtual HRESULT	Ready();

public:
	static CCubeTex* Create(LPDIRECT3DDEVICE9 device);
	virtual CComponent* Clone();
	virtual void Free();
};
END

#endif
