#include "Frame.h"
//#include <iostream>

USING(Engine)

CFrame::CFrame() :
	m_frame(0), m_callLimit(0.f), m_accDeltaTime(0.f)
{
}

CFrame::~CFrame()
{
	Free();
}

_bool CFrame::IsPermitCall(const _float & deltaTime)
{
	++m_frame;
	m_accDeltaTime += deltaTime;

	if (m_accDeltaTime >= m_callLimit)
	{
		//std::cout << m_frame << std::endl;
		m_frame = 0;
		m_accDeltaTime = 0.f;
		return true;
	}

	return false;
}

HRESULT CFrame::Ready(const _float & callLimit)
{
	m_callLimit = 1.f / callLimit;

	return S_OK;
}

CFrame * CFrame::Create(const _float & callLimit)
{
	CFrame* instance = new CFrame;

	if (FAILED(instance->Ready(callLimit)))
	{
		SafeRelease(instance);
	}

	return instance;
}

void CFrame::Free()
{
}
