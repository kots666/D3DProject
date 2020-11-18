#include "Scene.h"

USING(Engine)

CScene::CScene()
{
}


CScene::~CScene()
{
}

HRESULT CScene::Ready()
{
	return S_OK;
}

CLayer * CScene::GetLayer(const _tchar * layerName)
{
	auto iter = m_layerMap.find(layerName);

	if (m_layerMap.end() == iter) return nullptr;

	return iter->second;
}

CScene * CScene::Create()
{
	CScene* instance = new CScene;

	if (FAILED(instance->Ready())) SafeRelease(instance);

	return instance;
}

void CScene::Free()
{
}
