#ifndef RCTex_h__
#define RCTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRCTex : public CVIBuffer
{
protected:
	explicit CRCTex(LPDIRECT3DDEVICE9 device);
	explicit CRCTex(const CRCTex& rhs);
	virtual ~CRCTex();

public:
	virtual HRESULT	Ready();

public:
	static CRCTex* Create(LPDIRECT3DDEVICE9 device);
	CComponent* Clone();
	virtual void Free();
};
END

#endif
