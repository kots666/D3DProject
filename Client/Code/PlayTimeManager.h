#ifndef PlayTimeManager_h__
#define PlayTimeManager_h__

#include "Define.h"
#include "GameObject.h"

BEGIN(Client)

class CPlayTimeManager
{
	DECLARE_SINGLETON(CPlayTimeManager)

private:
	explicit CPlayTimeManager();
	virtual ~CPlayTimeManager();

public:
	void Update(const _float& deltaTime);
	void SetHitStop(const _float& stopTime = 0.05f, const _bool& isForced = false);
	void SetHitSlow(const _float& slowTime = 0.05f);

	_bool GetIsStopNow() const { return m_isCurStop; }
	_bool GetIsSlowNow() const { return m_isCurSlow; }
	_float GetStopTime() const { return m_restStopTime; }
	_float GetSlowTime() const { return m_restSlowTime; }

private:
	_bool m_isCurStop;
	_bool m_isCurSlow;
	_float m_restStopTime;
	_float m_restSlowTime;
};

END

#endif
