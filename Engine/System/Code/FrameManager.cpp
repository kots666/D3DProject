#include "FrameManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrameManager)

CFrameManager::CFrameManager()
{

}

CFrameManager::~CFrameManager()
{
	Free();
}

_bool CFrameManager::IsPermitCall(const _tchar * frameTag, const _float & deltaTime)
{
	CFrame* frame = FindFrame(frameTag);
	if (nullptr == frame) return false;

	return frame->IsPermitCall(deltaTime);
}

HRESULT CFrameManager::Ready(const _tchar * frameTag, const _float & callLimit)
{
	CFrame*	frame = FindFrame(frameTag);

	if (nullptr != frame) return E_FAIL;

	frame = CFrame::Create(callLimit);
	if (nullptr == frame) return E_FAIL;

	m_frameMap.emplace(frameTag, frame);

	return S_OK;
}

CFrame* CFrameManager::FindFrame(const _tchar * frameTag)
{
	auto iter = find_if(m_frameMap.begin(), m_frameMap.end(), CTagFinder(frameTag));

	if (m_frameMap.end() == iter) return nullptr;

	return iter->second;
}

void CFrameManager::Free()
{
	for_each(m_frameMap.begin(), m_frameMap.end(), CDeleteMap());
	m_frameMap.clear();
}
