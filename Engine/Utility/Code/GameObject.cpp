#include "GameObject.h"
#include "SphereCollider.h"

USING(Engine)

CGameObject::CGameObject(LPDIRECT3DDEVICE9 device) :
	m_device(device),
	m_isDead(false),
	m_speed(5.f),
	m_hp(50),
	m_maxHp(50),
	m_atk(10)
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

_int CGameObject::LateUpdate(const _float & deltaTime)
{
	return 0;
}

_bool CGameObject::IsInCollideList(CGameObject * obj)
{
	auto iter = find_if(m_collideList.begin(), m_collideList.end(), [&](CGameObject* elem)
	{
		return obj == elem;
	});

	if (m_collideList.end() == iter) return false;

	return true;
}

void CGameObject::ClearCollideList()
{
	m_collideList.clear();
}

void CGameObject::EnableAttackCollider()
{
	for (auto& elem : m_attackCollider)
		elem->SetCanCollide(true);
}

void CGameObject::EnableHitCollider()
{
	for (auto& elem : m_hitCollider)
		elem->SetCanCollide(true);
}

void CGameObject::DisableAttackCollider()
{
	for (auto& elem : m_attackCollider)
		elem->SetCanCollide(false);

	ClearCollideList();
}

void CGameObject::DisableHitCollider()
{
	for (auto& elem : m_hitCollider)
		elem->SetCanCollide(false);
}

CComponent * CGameObject::GetComponent(const _tchar* key, COMPONENTID id)
{
	auto iter = find_if(m_compMap[id].begin(), m_compMap[id].end(), CTagFinder(key));
	
	if (m_compMap[id].end() == iter) return nullptr;

	return iter->second;
}

void CGameObject::ComputeViewZ(const _vec3 * pos)
{
	_matrix camWorld;
	m_device->GetTransform(D3DTS_VIEW, &camWorld);
	D3DXMatrixInverse(&camWorld, nullptr, &camWorld);

	_vec3 camPos;
	memcpy(&camPos, &camWorld.m[3][0], sizeof(_vec3));

	m_viewZ = D3DXVec3Length(&(camPos - *pos));
}

void CGameObject::Free()
{
	for (_int i = 0; i < ID_END; ++i)
	{
		for_each(m_compMap[i].begin(), m_compMap[i].end(), CDeleteMap());
		m_compMap[i].clear();
	}

	SafeRelease(m_device);

	for (auto& elem : m_hitCollider)
	{
		SafeRelease(elem);
	}
	m_hitCollider.clear();
	m_hitCollider.shrink_to_fit();

	for (auto& elem : m_attackCollider)
	{
		SafeRelease(elem);
	}
	m_attackCollider.clear();
	m_attackCollider.shrink_to_fit();
}
