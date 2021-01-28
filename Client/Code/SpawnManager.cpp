#include "stdafx.h"
#include "SpawnManager.h"
#include "GoblinSword.h"
#include "GoblinMagician.h"
#include "FireBall.h"
#include "Dog.h"
#include "Minotauros.h"
#include "Boss.h"

USING(Client)
IMPLEMENT_SINGLETON(CSpawnManager)

CSpawnManager::CSpawnManager() :
	m_device(nullptr),
	m_eventCount(0)
{
	ZeroMemory(&m_spawnCount, sizeof(m_spawnCount));
}

CSpawnManager::~CSpawnManager()
{
	SafeRelease(m_device);

	for (auto& string : m_nameList)
	{
		SafeDeleteArray(string);
	}
}

HRESULT CSpawnManager::Ready(LPDIRECT3DDEVICE9 device)
{
	if (nullptr == m_device)
	{
		m_device = device;
		Engine::SafeAddRef(m_device);
	}

	return S_OK;
}

void CSpawnManager::CheckSpawnCondition()
{
	if (2 < m_eventCount && m_eventCount < 4)
	{
		if (IsDeadAllEnemies())
			StartEvent();
	}
}

void CSpawnManager::CheckQuestComplete()
{
	if (CQuestManager::GetInstance()->GetIsProgress())
	{
		if (1 == m_eventCount && 0 == CQuestManager::GetInstance()->GetStep())
		{
			if(IsDeadAllEnemies())
				CQuestManager::GetInstance()->CompleteQuest();
		}
		else if (3 < m_eventCount && 1 == CQuestManager::GetInstance()->GetStep())
		{
			if (IsDeadAllEnemies())
				CQuestManager::GetInstance()->CompleteQuest();
		}
		
	}
}

void CSpawnManager::Spawn(SPAWNTYPE type, const _vec3 & pos, const _float& angle, const _vec3* target)
{
	Engine::CGameObject* gameObj = nullptr;

	Engine::CLayer* layer = Engine::GetCurScene()->GetLayer(L"GameLogic");

	if (nullptr == layer) return;

	_int typeNum = 0;

	_tchar* meshName = new _tchar[30];

	switch (type)
	{
	case SPAWNTYPE::GOBLIN_SWORD:
		gameObj = CGoblinSword::Create(m_device, pos, angle);
		typeNum = 0;
		wsprintf(meshName, L"Mesh_Goblin_Sword%d", m_spawnCount[typeNum]);
		break;
	case SPAWNTYPE::GOBLIN_MAGICIAN:
		gameObj = CGoblinMagician::Create(m_device, pos, angle);
		typeNum = 1;
		wsprintf(meshName, L"Mesh_Goblin_Magician%d", m_spawnCount[typeNum]);
		break;
	case SPAWNTYPE::FIREBALL:
		gameObj = CFireBall::Create(m_device, pos, *target);
		typeNum = 2;
		wsprintf(meshName, L"Mesh_FireBall%d", m_spawnCount[typeNum]);
		break;
	case SPAWNTYPE::DOG:
		gameObj = CDog::Create(m_device, pos, angle);
		typeNum = 3;
		wsprintf(meshName, L"Mesh_Dog%d", m_spawnCount[typeNum]);
		break;
	case SPAWNTYPE::MINOTAUROS:
		gameObj = CMinotauros::Create(m_device, pos, angle);
		typeNum = 4;
		wsprintf(meshName, L"Mesh_Minotauros%d", m_spawnCount[typeNum]);
		break;
	case SPAWNTYPE::BOSS:
		gameObj = CBoss::Create(m_device, pos, angle);
		typeNum = 5;
		wsprintf(meshName, L"Mesh_Boss%d", m_spawnCount[typeNum]);
		break;
	}

	if (nullptr == gameObj)
	{
		SafeDeleteArray(meshName);
		return;
	}

	layer->AddGameObject(meshName, gameObj);
	m_spawnedList[typeNum].emplace_back(gameObj);
	m_nameList.emplace_back(meshName);

	++m_spawnCount[typeNum];
}

_bool CSpawnManager::StartEvent()
{
	if (!CQuestManager::GetInstance()->GetIsProgress())
		return false;

	if (0 == CQuestManager::GetInstance()->GetStep())
	{
		if (0 == m_eventCount)
		{
			Spawn(SPAWNTYPE::DOG, { 4.f, 0.f, 16.f }, 150.f);
			Spawn(SPAWNTYPE::DOG, { 12.f, 0.f, 23.f }, -150.f);

			++m_eventCount;

			return true;
		}
		else return false;
	}

	else
	{
		switch (m_eventCount)
		{
		case 1:
			Spawn(SPAWNTYPE::DOG, { 23.f, 0.f, 57.f }, -90.f);
			Spawn(SPAWNTYPE::DOG, { 14.f, 0.f, 36.f }, -150.f);
			Spawn(SPAWNTYPE::MINOTAUROS, { 35.f, 0.f, 64.f }, -90.f);
			break;

		case 2:
			Spawn(SPAWNTYPE::GOBLIN_MAGICIAN, { 67.f, 0.f, 62.f }, -75.f);
			Spawn(SPAWNTYPE::GOBLIN_MAGICIAN, { 70.f, 0.f, 73.f }, -90.f);
			Spawn(SPAWNTYPE::GOBLIN_MAGICIAN, { 60.f, 0.f, 83.f }, -130.f);
			break;

		case 3:
			Spawn(SPAWNTYPE::BOSS, { 65.f, 0.f, 77.f }, -90.f);
			break;
		}

		++m_eventCount;

		return true;
	}

	return true;
}

void CSpawnManager::DeleteObject(SPAWNTYPE type, Engine::CGameObject* target)
{
	auto iter = find_if(m_spawnedList[(_int)type].begin(), m_spawnedList[(_int)type].end(), [&](Engine::CGameObject* elem)
	{
		return elem == target;
	});

	if (m_spawnedList[(_int)type].end() == iter) return;

	m_spawnedList[(_int)type].erase(iter);
}

_bool CSpawnManager::IsDeadAllEnemies()
{
	for (_int i = 0; i < (_int)SPAWNTYPE::TYPE_END; ++i)
	{
		if (0 < m_spawnedList[i].size())
			return false;
	}

	return true;
}
