#include "Layer.h"

USING(Engine)

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

HRESULT CLayer::Ready()
{
	return S_OK;
}

CGameObj * CLayer::GetObj(const _tchar * objName)
{
	auto iter = m_objMap.find(objName);

	if (m_objMap.end() == iter) return nullptr;

	return iter->second;
}

CLayer * CLayer::Create()
{
	CLayer* instance = new CLayer;

	if (FAILED(instance->Ready())) SafeRelease(instance);

	return instance;
}

void CLayer::Free()
{
}
