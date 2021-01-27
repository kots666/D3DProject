#ifndef SpawnManager_h__
#define SpawnManager_h__

#include "Define.h"
#include "GameObject.h"

enum class SPAWNTYPE
{
	GOBLIN_SWORD,
	GOBLIN_MAGICIAN,
	FIREBALL,
	DOG,
	MINOTAUROS,
	BOSS,
	TYPE_END
};

BEGIN(Client)

class CSpawnManager
{
	DECLARE_SINGLETON(CSpawnManager)

private:
	explicit CSpawnManager();
	virtual ~CSpawnManager();

public:
	HRESULT Ready(LPDIRECT3DDEVICE9 device);
	void CheckSpawnCondition();
	void CheckQuestComplete();

public:
	void Spawn(SPAWNTYPE type, const _vec3& pos, const _float& angle = 0.f, const _vec3* target = nullptr);
	_bool StartEvent();
	void DeleteObject(SPAWNTYPE type, Engine::CGameObject* target);

private:
	_bool IsDeadAllEnemies();

private:
	_int m_spawnCount[(_int)SPAWNTYPE::TYPE_END];
	list<_tchar*> m_nameList;
	list<Engine::CGameObject*> m_spawnedList[(_int)SPAWNTYPE::TYPE_END];

	_int m_eventCount;

	LPDIRECT3DDEVICE9 m_device;
};

END

#endif
