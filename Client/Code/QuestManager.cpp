#include "stdafx.h"
#include "QuestManager.h"

USING(Client)
IMPLEMENT_SINGLETON(CQuestManager)

CQuestManager::CQuestManager() :
	m_questStep(0)
{
	
}

CQuestManager::~CQuestManager()
{
}