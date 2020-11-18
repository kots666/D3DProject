#ifndef TestGeometry_h__
#define TestGeometry_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTestGeometry : public CVIBuffer
{
protected:
	explicit CTestGeometry(LPDIRECT3DDEVICE9 device);
	explicit CTestGeometry(const CTestGeometry& rhs);
	virtual ~CTestGeometry();

public:
	virtual HRESULT	Ready();

public:
	static CTestGeometry* Create(LPDIRECT3DDEVICE9 device);
	CComponent* Clone();
	virtual void Free();
};
END

#endif
