#include "stdafx.h"
#include "QuestManager.h"
#include "TalkUI.h"
#include "UI.h"

USING(Client)
IMPLEMENT_SINGLETON(CQuestManager)

CQuestManager::CQuestManager() :
	m_talkUI(nullptr),
	m_ilustUI(nullptr),
	m_questUI(nullptr),
	m_questStep(0),
	m_talkStep(0),
	m_isProgress(false),
	m_isComplete(false),
	m_isTalking(false)
{
	
}

CQuestManager::~CQuestManager()
{
	ClearAll();
}

void CQuestManager::QuestProgress()
{
	// 말하는 중일 때
	if (m_isTalking)
	{
		if (3 > m_talkStep)
		{
			if (1 == m_talkStep)
				ChangeIlustUI(1);
			ChangeTalkUI(m_talkStep++);
		}

		else
		{
			ClearTalkUI();
			ClearIlustUI();
			CreateQuestUI();
			m_isTalking = false;
		}
	}
	
	else
	{
		if (!m_isProgress)
		{
			m_isProgress = true;
			m_isComplete = false;

			if (0 == m_talkStep)
			{
				m_isTalking = true;
				CreateTalkUI(m_talkStep);
				CreateIlustUI(m_talkStep);
				++m_talkStep;
			}
			else
				ChangeQuestUI(3);
		}
		else
		{
			if (m_isComplete)
			{
				// 퀘스트완료에 대한 진행

				++m_questStep;
				m_isComplete = false;
				m_isProgress = false;

				ChangeQuestUI(4);
			}
			else
			{
				// 퀘스트 미완료에 대한 진행
			}
		}
	}
}

void CQuestManager::CreateTalkUI(const _int& index)
{
	ClearTalkUI();

	Engine::CLayer* layer = Engine::GetCurScene()->GetLayer(L"UI");
	if (nullptr == layer) return;

	m_talkUI = CTalkUI::Create(
		Engine::CGraphicDevice::GetInstance()->GetDevice(),
		index,
		0,
		WINCY - 200,
		WINCX,
		200
	);

	_tchar* name = new _tchar[10];

	wsprintf(name, L"TALK_%d", index);

	layer->AddGameObject(name, m_talkUI);

	m_nameList.emplace_back(name);
}

void CQuestManager::CreateIlustUI(const _int & index)
{
	ClearIlustUI();

	Engine::CLayer* layer = Engine::GetCurScene()->GetLayer(L"UI");
	if (nullptr == layer) return;

	m_ilustUI = CTalkUI::Create(
		Engine::CGraphicDevice::GetInstance()->GetDevice(),
		0,
		0,
		WINCY - 400,
		200,
		200,
		true
	);

	_tchar* name = new _tchar[10];

	wsprintf(name, L"Ilust_%d", index);

	layer->AddGameObject(name, m_ilustUI);

	m_nameList.emplace_back(name);
}

void CQuestManager::CreateQuestUI()
{
	ClearQuestUI();

	Engine::CLayer* layer = Engine::GetCurScene()->GetLayer(L"UI");
	if (nullptr == layer) return;

	m_questUI = CTalkUI::Create(
		Engine::CGraphicDevice::GetInstance()->GetDevice(),
		3,
		WINCX - 280,
		50,
		280,
		55
	);

	_tchar* name = new _tchar[10];

	wsprintf(name, L"QuestUI_%d", 0);

	layer->AddGameObject(name, m_questUI);

	m_nameList.emplace_back(name);
}

void CQuestManager::ChangeTalkUI(const _int & index)
{
	if (nullptr == m_talkUI) return;

	m_talkUI->SetIndex(index);
}

void CQuestManager::ChangeIlustUI(const _int & index)
{
	if (nullptr == m_ilustUI) return;

	m_ilustUI->SetIndex(index);
}

void CQuestManager::ChangeQuestUI(const _int& index)
{
	if (nullptr == m_questUI) return;

	m_questUI->SetIndex(index);
}

void CQuestManager::QuestCheat(const _int & step, const _bool & progrss, const _bool & complete)
{
	m_questStep = step;
	m_isProgress = progrss;
	m_isComplete = complete;
}

void CQuestManager::CompleteQuest()
{
	m_isComplete = true;

	ChangeQuestUI(4);
}

void CQuestManager::ClearTalkUI()
{
	if (nullptr != m_talkUI)
	{
		m_talkUI->SetIsDead(true);
		m_talkUI = nullptr;
	}
}

void CQuestManager::ClearIlustUI()
{
	if (nullptr != m_ilustUI)
	{
		m_ilustUI->SetIsDead(true);
		m_ilustUI = nullptr;
	}
}

void CQuestManager::ClearQuestUI()
{
	if (nullptr != m_questUI)
	{
		m_questUI->SetIsDead(true);
		m_questUI = nullptr;
	}
}

void CQuestManager::ClearAll()
{
	ClearTalkUI();
	ClearIlustUI();
	ClearQuestUI();

	for (auto& elem : m_nameList)
	{
		delete[] elem;
		elem = nullptr;
	}
	m_nameList.clear();
}
