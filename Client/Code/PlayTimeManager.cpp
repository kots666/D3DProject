#include "stdafx.h"
#include "PlayTimeManager.h"

USING(Client)
IMPLEMENT_SINGLETON(CPlayTimeManager)

CPlayTimeManager::CPlayTimeManager() :
	m_isCurStop(false),
	m_isCurSlow(false),
	m_restStopTime(0.f),
	m_restSlowTime(0.f)
{
}

CPlayTimeManager::~CPlayTimeManager()
{
}

void CPlayTimeManager::Update(const _float & deltaTime)
{
	if (m_isCurStop)
	{
		m_restStopTime -= deltaTime;

		if (0.f > m_restStopTime)
		{
			m_isCurStop = false;
			m_restStopTime = 0.f;
		}
	}

	if (m_isCurSlow)
	{
		m_restSlowTime -= deltaTime;

		if (0.f > m_restSlowTime)
		{
			m_isCurSlow = false;
			m_restSlowTime = 0.f;

			SetHitStop();
		}
	}
}

void CPlayTimeManager::SetHitStop(const _float & stopTime, const _bool & isForced)
{
	if (isForced)
	{
		m_isCurStop = true;
		m_restStopTime = stopTime;
	}
	else if (!m_isCurStop)
	{
		m_isCurStop = true;
		m_restStopTime = stopTime;
	}
}

void CPlayTimeManager::SetHitSlow(const _float & slowTime)
{
	m_isCurSlow = true;
	m_restSlowTime = slowTime;
}
