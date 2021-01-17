#include "stdafx.h"
#include "ColliderManager.h"

USING(Client)
IMPLEMENT_SINGLETON(CColliderManager)

CColliderManager::CColliderManager()
{

}

CColliderManager::~CColliderManager()
{
	for (_int i = 0; i < OBJ_END; ++i)
	{
		m_colliderGroup[i].clear();
	}
}

void CColliderManager::CheckCollision(OBJTYPE srcGroup, OBJTYPE dstGroup)
{
	for (auto& src : m_colliderGroup[srcGroup])
	{
		auto srcCollVec = src->GetAttackColliderVec();
		for (auto& dst : m_colliderGroup[dstGroup])
		{
			auto dstCollVec = dst->GetHitColliderVec();

			for (auto& hitCol : *dstCollVec)
			{
				if (!hitCol->GetCanCollide()) continue;

				for (auto& atkCol : *srcCollVec)
				{
					if (!atkCol->GetCanCollide()) continue;

					if (SphereCollision(atkCol, hitCol))
					{
						if (src->AttackColliderOverlapped(dst))
						{
							dst->HitColliderOverlapped(src);
						}
						break;
					}
				}
			}
		}
	}
}

void CColliderManager::CheckEventCollision()
{
	Engine::CGameObject* player = GetPlayer();
	if (nullptr == player) return;

	Engine::CSphereCollider* playerColl = player->GetHitColliderVec()->front();

	for (auto& eventSpawner : m_colliderGroup[OBJ_EVENT])
	{
		auto eventColl = eventSpawner->GetHitColliderVec()->front();
		
		if (SphereCollision(playerColl, eventColl))
		{
			eventSpawner->HitColliderOverlapped(nullptr);
		}
	}
}

void CColliderManager::AddObject(OBJTYPE group, Engine::CGameObject * newObj)
{
	auto iter = FindObjectInGroup(group, newObj);

	if (m_colliderGroup[group].end() == iter)
	{
		m_colliderGroup[group].emplace_back(newObj);
	}
}

void CColliderManager::DeleteObject(OBJTYPE group, Engine::CGameObject * obj)
{
	auto iter = FindObjectInGroup(group, obj);

	if (m_colliderGroup[group].end() == iter) return;

	m_colliderGroup[group].erase(iter);
}

void CColliderManager::DeleteObject(Engine::CGameObject * obj)
{
	for (_int i = 0; i < OBJ_END; ++i)
	{
		auto iter = FindObjectInGroup(OBJTYPE(i), obj);

		if (m_colliderGroup[i].end() == iter) continue;

		m_colliderGroup[i].erase(iter);
	}
}

_bool CColliderManager::SphereCollision(Engine::CSphereCollider * lhs, Engine::CSphereCollider * rhs)
{
	_matrix lhsWorldMat = lhs->GetWorldMat();
	_matrix rhsWorldMat = rhs->GetWorldMat();

	_vec3 lhsPos, rhsPos;

	memcpy(&lhsPos, &lhsWorldMat.m[3], sizeof(_vec3));
	memcpy(&rhsPos, &rhsWorldMat.m[3], sizeof(_vec3));

	_float lhsRadius = lhs->GetRadius();
	_float rhsRadius = rhs->GetRadius();

	/*_vec3 radiusVec = { lhsRadius, lhsRadius, lhsRadius };
	_vec3 radiusVec2 = { rhsRadius, rhsRadius, rhsRadius };

	D3DXVec3TransformNormal(&radiusVec, &radiusVec, &lhsWorldMat);
	D3DXVec3TransformNormal(&radiusVec2, &radiusVec2, &rhsWorldMat);

	lhsRadius = (radiusVec.x + radiusVec.y + radiusVec.z) / 3.f;
	rhsRadius = (radiusVec2.x + radiusVec2.y + radiusVec2.z) / 3.f;*/

	_float dist = D3DXVec3Length(&(lhsPos - rhsPos));

	if (dist > lhsRadius + rhsRadius) return false;

	return true;
}

list<Engine::CGameObject*>::iterator CColliderManager::FindObjectInGroup(OBJTYPE group, Engine::CGameObject * target)
{
	auto iter = find_if(m_colliderGroup[group].begin(), m_colliderGroup[group].end(), [&](Engine::CGameObject* elem)
	{
		return target == elem;
	});

	return iter;
}

void CColliderManager::DeleteColliderGroup(OBJTYPE group)
{
	m_colliderGroup[group].clear();
}

Engine::CGameObject * CColliderManager::GetPlayer()
{
	if (0 < m_colliderGroup[OBJ_PLAYER].size())
	{
		return m_colliderGroup[OBJ_PLAYER].front();
	}

	return nullptr;
}
