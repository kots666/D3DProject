#ifndef Base_h__
#define Base_h__

class _declspec(dllexport) CBase
{
protected:
	inline explicit CBase();
	inline virtual ~CBase();

public:
	inline unsigned long AddRef();
	inline unsigned long Release();

private:
	unsigned long m_refCnt;

public:
	inline virtual void Free() = 0;
};

#include "Base.inl"

#endif