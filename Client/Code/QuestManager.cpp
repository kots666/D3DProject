#include "stdafx.h"
#include "QuestManager.h"

USING(Client)
IMPLEMENT_SINGLETON(CQuestManager)

CQuestManager::CQuestManager() :
	m_questStep(0),
	m_isProgress(false),
	m_isComplete(false)
{
	
}

CQuestManager::~CQuestManager()
{
}

void CQuestManager::QuestProgress()
{
	if (!m_isProgress)
	{
		m_isProgress = true;
		m_isComplete = false;
		// step에 해당하는 퀘스트 진행시킴.
	}
	else
	{
		if (m_isComplete)
		{
			// 퀘스트완료에 대한 진행
		}
		else
		{
			// 퀘스트 미완료에 대한 진행
		}
	}
}

void CQuestManager::QuestCheat(const _int & step, const _bool & progrss, const _bool & complete)
{
	m_questStep = step;
	m_isProgress = progrss;
	m_isComplete = complete;
}