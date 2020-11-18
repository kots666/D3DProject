#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CResources
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 device);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();

public:
	virtual HRESULT	Ready();
	virtual void Render();

protected:
	LPDIRECT3DVERTEXBUFFER9	m_VB;
	LPDIRECT3DINDEXBUFFER9 m_IB;

	_ulong m_vtxCnt;
	_ulong m_triCnt;
	_ulong m_vtxSize;
	_ulong m_FVF;

	_ulong m_idxSize;
	D3DFORMAT m_format;

public:
	virtual void Free();
};

END

#endif
