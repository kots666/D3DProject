#include "Timer.h"

USING(Engine)

CTimer::CTimer() :
	m_deltaTime(0.f)
{

}

CTimer::~CTimer()
{

}

HRESULT CTimer::Ready()
{
	QueryPerformanceCounter(&m_frameTime);	// 1012
	QueryPerformanceCounter(&m_lastTime);	// 1048
	QueryPerformanceCounter(&m_fixTime);
	QueryPerformanceFrequency(&m_cpuTick);

	return S_OK;
}

void CTimer::Update()
{
	QueryPerformanceCounter(&m_frameTime);

	if (m_frameTime.QuadPart - m_fixTime.QuadPart >= m_cpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_cpuTick);
		m_fixTime = m_frameTime;
	}

	m_deltaTime = (_float(m_frameTime.QuadPart) - _float(m_lastTime.QuadPart)) / (_float)m_cpuTick.QuadPart;

	m_lastTime = m_frameTime;
}

CTimer* CTimer::Create()
{
	CTimer*	instance = new CTimer;

	if (FAILED(instance->Ready()))
		SafeRelease(instance);

	return instance;
}

void CTimer::Free()
{

}

