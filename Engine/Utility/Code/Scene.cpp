#include "Scene.h"

USING(Engine)

CScene::CScene(LPDIRECT3DDEVICE9 device) :
	m_device(device)
{
	SafeAddRef(m_device);
}


CScene::~CScene()
{
}

HRESULT CScene::Ready()
{
	return S_OK;
}

_int CScene::Update(const _float & deltaTime)
{
	_int ret = 0;

	for (auto& iter : m_layerMap)
	{
		ret = iter.second->Update(deltaTime);

		if (ret & 0x80000000)
		{
			MSG_BOX("Layer Problem");
			return ret;
		}
	}

	return ret;
}

void CScene::Render()
{
}

HRESULT CScene::AddLayer(const _tchar * key, CLayer * newLayer)
{
	auto iter = find_if(m_layerMap.begin(), m_layerMap.end(), CTagFinder(key));

	if (m_layerMap.end() != iter) return E_FAIL;

	m_layerMap.emplace(key, newLayer);

	return S_OK;
}

CLayer * CScene::GetLayer(const _tchar * key)
{
	auto iter = find_if(m_layerMap.begin(), m_layerMap.end(), CTagFinder(key));

	if (m_layerMap.end() == iter) return nullptr;

	return iter->second;
}

CComponent * CScene::GetComponent(const _tchar * layerKey, const _tchar * objKey, const _tchar * compKey, COMPONENTID id)
{
	auto iter = find_if(m_layerMap.begin(), m_layerMap.end(), CTagFinder(layerKey));

	if (m_layerMap.end() == iter) return nullptr;

	return iter->second->GetComponent(objKey, compKey, id);
}

void CScene::Free()
{
	for_each(m_layerMap.begin(), m_layerMap.end(), CDeleteMap());
	m_layerMap.clear();

	SafeRelease(m_device);
}
