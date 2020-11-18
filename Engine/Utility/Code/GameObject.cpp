#include "GameObject.h"

USING(Engine)

CGameObject::CGameObject(LPDIRECT3DDEVICE9 device) :
	m_device(device)
{
	SafeAddRef(m_device);
}

//CGameObject::CGameObject(const CGameObject & rhs) :
//	m_device(rhs.m_device)
//{
//	SafeAddRef(m_device);
//
//	for (_int i = 0; i < ID_END; ++i)
//	{
//		for (auto& iter : rhs.m_compMap[i])
//		{
//			m_compMap[i].emplace(iter.first, iter.second);
//		}
//	}
//}

CGameObject::~CGameObject()
{
}

_int CGameObject::Update(const _float & deltaTime)
{
	_int ret = 0;

	for (auto& iter : m_compMap[ID_DYNAMIC])
	{
		ret = iter.second->Update(deltaTime);

		if (ret & 0x80000000)
		{
			MSG_BOX("Component Problem");
			return -1;
		}
	}

	return ret;
}

CComponent * CGameObject::GetComponent(const _tchar* key, COMPONENTID id)
{
	auto iter = find_if(m_compMap[id].begin(), m_compMap[id].end(), CTagFinder(key));
	
	if (m_compMap[id].end() == iter) return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	for (_int i = 0; i < ID_END; ++i)
	{
		for_each(m_compMap[i].begin(), m_compMap[i].end(), CDeleteMap());
		m_compMap[i].clear();
	}

	SafeRelease(m_device);
}
