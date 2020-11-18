#include "TimerManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimerManager)

CTimerManager::CTimerManager()
{

}

CTimerManager::~CTimerManager()
{
	Free();
}

_float CTimerManager::GetDeltaTime(const _tchar* timerTag)
{
	CTimer*	timer = Find(timerTag);

	if (nullptr == timer)
		return 0.f;

	return timer->GetDeltaTime();
}

void CTimerManager::Update(const _tchar* timerTag)
{
	CTimer*	timer = Find(timerTag);

	if (nullptr == timer)
		return;

	timer->Update();
}

HRESULT CTimerManager::Ready(const _tchar* timerTag)
{
	CTimer*	timer = Find(timerTag);
	
	if (nullptr != timer)
		return E_FAIL;

	timer = CTimer::Create();
	NULL_CHECK_RETURN(timer, E_FAIL);

	m_timerMap.emplace(timerTag, timer);

	return S_OK;
}

CTimer* CTimerManager::Find(const _tchar* timerTag)
{
	auto iter = find_if(m_timerMap.begin(), m_timerMap.end(), CTagFinder(timerTag));

	if (m_timerMap.end() == iter)
		return nullptr;

	return iter->second;
}

void CTimerManager::Free()
{
	for_each(m_timerMap.begin(), m_timerMap.end(), CDeleteMap());
	m_timerMap.clear();
}