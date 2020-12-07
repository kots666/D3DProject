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

_int CLayer::Update(const _float & deltaTime)
{
	_int ret = 0;

	for (auto& iter : m_objMap)
	{
		if (nullptr != iter.second)
		{
			ret = iter.second->Update(deltaTime);

			if (ret & 0x80000000)
			{
				MSG_BOX("GameObject Problem");
				return ret;
			}
		}
	}

	return ret;
}

_int CLayer::LateUpdate(const _float & deltaTime)
{
	_int ret = 0;

	for (auto iter = m_objMap.begin(); iter != m_objMap.end();)
	{
		if (iter->second->GetIsDead())
		{
			SafeRelease(iter->second);
			iter = m_objMap.erase(iter);
		}
		else
			++iter;
	}

	return ret;
}

HRESULT CLayer::AddGameObject(const _tchar * key, CGameObject * obj)
{
	if (nullptr == obj) return E_FAIL;

	m_objMap.emplace(key, obj);

	return S_OK;
}

CGameObject * CLayer::GetGameObject(const _tchar * key)
{
	auto iter = find_if(m_objMap.begin(), m_objMap.end(), CTagFinder(key));

	if (m_objMap.end() == iter) return nullptr;

	return iter->second;
}

CComponent * CLayer::GetComponent(const _tchar * objKey, const _tchar * compKey, COMPONENTID id)
{
	auto iter = find_if(m_objMap.begin(), m_objMap.end(), CTagFinder(objKey));

	if (m_objMap.end() == iter) return nullptr;

	return iter->second->GetComponent(compKey, id);
}

CLayer * CLayer::Create()
{
	CLayer* instance = new CLayer;

	if (FAILED(instance->Ready())) SafeRelease(instance);

	return instance;
}

void CLayer::Free()
{
	for_each(m_objMap.begin(), m_objMap.end(), CDeleteMap());
	m_objMap.clear();
}
