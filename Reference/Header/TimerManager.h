#ifndef TimerManager_h__
#define TimerManager_h__

#include "Timer.h"

BEGIN(Engine)

class ENGINE_DLL CTimerManager : public CBase
{
	DECLARE_SINGLETON(CTimerManager)

private:
	explicit CTimerManager();
	virtual ~CTimerManager();

public:
	_float GetDeltaTime(const _tchar* timerTag);
	void Update(const _tchar* timerTag);

public:
	HRESULT Ready(const _tchar* timerTag);

private:
	CTimer* Find(const _tchar* timerTag);

private:
	map<const _tchar*, CTimer*> m_timerMap;

public:
	virtual void Free();

};

END
#endif
