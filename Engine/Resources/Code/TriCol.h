#ifndef TriCol_h__
#define TriCol_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriCol : public CVIBuffer
{
protected:
	explicit CTriCol(LPDIRECT3DDEVICE9 device);
	explicit CTriCol(const CTriCol& rhs);
	virtual ~CTriCol();

public:
	virtual HRESULT	Ready();

public:
	static CTriCol*	Create(LPDIRECT3DDEVICE9 device);
	CComponent* Clone();
	virtual void Free();
};
END

#endif
