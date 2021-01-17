#include "RenderTargetMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderTargetMgr)

Engine::CRenderTargetMgr::CRenderTargetMgr()
{

}

Engine::CRenderTargetMgr::~CRenderTargetMgr()
{
	Free();
}

HRESULT Engine::CRenderTargetMgr::ReadyRenderTarget(LPDIRECT3DDEVICE9 device, const _tchar* targetTag, const _uint& width, const _uint& height, D3DFORMAT format, D3DXCOLOR color)
{
	CRenderTarget* renderTarget = FindRenderTarget(targetTag);

	if (nullptr != renderTarget)
		return E_FAIL;

	renderTarget = CRenderTarget::Create(device, width, height, format, color);
	
	if (nullptr == renderTarget)
		return E_FAIL;

	m_RenderTargetMap.emplace(targetTag, renderTarget);

	return S_OK;
}

HRESULT CRenderTargetMgr::ReadyMRT(const _tchar * MRTTag, const _tchar * targetTag)
{
	CRenderTarget* renderTarget = FindRenderTarget(targetTag);
	if (nullptr == renderTarget)
		return E_FAIL;

	list<CRenderTarget*>* findMRTList = FindMRT(MRTTag);
	if (nullptr == findMRTList)
	{
		list<CRenderTarget*> MRTList;
		MRTList.push_back(renderTarget);

		m_MRTMap.emplace(MRTTag, MRTList);
	}
	else
	{
		findMRTList->push_back(renderTarget);
	}
	
	return S_OK;
}

HRESULT CRenderTargetMgr::BeginMRT(const _tchar * MRTTag)
{
	list<CRenderTarget*>* MRTList = FindMRT(MRTTag);
	if (nullptr == MRTList)
		return E_FAIL;

	for (auto& iter : *MRTList)
		iter->Clear();

	_uint index = 0;

	for (auto& iter : *MRTList)
	{
		iter->SetUpOnDevice(index);
		index++;
	}

	return S_OK;
}

HRESULT CRenderTargetMgr::EndMRT(const _tchar * MRTTag)
{
	list<CRenderTarget*>* MRTList = FindMRT(MRTTag);
	if (nullptr == MRTList)
		return E_FAIL;

	_uint index = 0;

	for (auto& iter : *MRTList)
	{
		iter->ReleaseOnDevice(index);
		index++;
	}

	return S_OK;
}

HRESULT CRenderTargetMgr::ReadyDebugBuffer(const _tchar * targetTag, const _float & x, const _float & y, const _float & sizeX, const _float & sizeY)
{
	CRenderTarget* renderTarget = FindRenderTarget(targetTag);
	if (nullptr == renderTarget)
		return E_FAIL;

	FAILED_CHECK_RETURN(renderTarget->ReadyDebugBuffer(x, y, sizeX, sizeY), E_FAIL);

	return S_OK;
}

void CRenderTargetMgr::RenderDebugBuffer(const _tchar * MRTTag)
{
	list<CRenderTarget*>* MRTList = FindMRT(MRTTag);

	if (nullptr == MRTList)
		return;

	for (auto& iter : *MRTList)
		iter->RenderDebugBuffer();
}

void Engine::CRenderTargetMgr::ThrowRenderTargetTexture(LPD3DXEFFECT & effect, const _tchar * targetTag, const char * constantName)
{
	CRenderTarget* renderTarget = FindRenderTarget(targetTag);
	NULL_CHECK(renderTarget);

	renderTarget->ThrowRenderTargetTexture(effect, constantName);
}

CRenderTarget* Engine::CRenderTargetMgr::FindRenderTarget(const _tchar* targetTag)
{
	auto iter = find_if(m_RenderTargetMap.begin(), m_RenderTargetMap.end(), CTagFinder(targetTag));

	if (iter == m_RenderTargetMap.end())
		return nullptr;

	return iter->second;
}

list<CRenderTarget*>* CRenderTargetMgr::FindMRT(const _tchar * MRTTag)
{
	auto iter = find_if(m_MRTMap.begin(), m_MRTMap.end(), CTagFinder(MRTTag));

	if (iter == m_MRTMap.end())
		return nullptr;

	return &iter->second;
}

void Engine::CRenderTargetMgr::Free(void)
{
	for_each(m_RenderTargetMap.begin(), m_RenderTargetMap.end(), CDeleteMap());
	m_RenderTargetMap.clear();

	for (auto& iter : m_MRTMap)
		iter.second.clear();

	m_MRTMap.clear();
}

