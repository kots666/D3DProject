#ifndef Frame_h__
#define Frame_h__

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CFrame : public CBase
{
private:
	explicit CFrame();
	virtual ~CFrame();

public:
	_bool IsPermitCall(const _float& deltaTime);

public:
	HRESULT Ready(const _float& callLimit);

private:
	_int m_frame;
	_float m_callLimit;
	_float m_accDeltaTime;

public:
	static CFrame* Create(const _float& callLimit);
	virtual void Free();
};

END

#endif