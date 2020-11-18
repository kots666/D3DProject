#ifndef Timer_h__
#define Timer_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CTimer : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer();

public:
	_float GetDeltaTime() { return m_deltaTime; }

public:
	HRESULT Ready();
	void Update();

private:
	LARGE_INTEGER m_frameTime;
	LARGE_INTEGER m_fixTime;
	LARGE_INTEGER m_lastTime;
	LARGE_INTEGER m_cpuTick;

private:
	_float m_deltaTime;

public:
	static CTimer* Create();
	virtual void Free();
};

END

#endif
