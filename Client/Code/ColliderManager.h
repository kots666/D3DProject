#ifndef ColliderManager_h__
#define ColliderManager_h__

#include "Define.h"
#include "GameObject.h"

BEGIN(Client)

class CColliderManager
{
	DECLARE_SINGLETON(CColliderManager)

private:
	explicit CColliderManager();
	virtual ~CColliderManager();

public:
	void CheckCollision(OBJTYPE srcGroup, OBJTYPE dstGroup);
	void CheckEventCollision();

public:
	void AddObject(OBJTYPE group, Engine::CGameObject* newObj);
	void DeleteObject(OBJTYPE group, Engine::CGameObject* obj);
	void DeleteObject(Engine::CGameObject* obj);
	_bool SphereCollision(Engine::CSphereCollider* lhs, Engine::CSphereCollider* rhs);

public:
	list<Engine::CGameObject*>::iterator FindObjectInGroup(OBJTYPE group, Engine::CGameObject* target);
	void DeleteColliderGroup(OBJTYPE group);

private:
	Engine::CGameObject* GetPlayer();

private:
	list<Engine::CGameObject*> m_colliderGroup[OBJ_END];
};

END

#endif
