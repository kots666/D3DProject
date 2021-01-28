#include "stdafx.h"
#include "QuestManager.h"
#include "TalkUI.h"

USING(Client)
IMPLEMENT_SINGLETON(CQuestManager)

CQuestManager::CQuestManager() :
	m_questStep(0),
	m_talkStep(0),
	m_isProgress(false),
	m_isComplete(false),
	m_isTalking(false)
{
	
}

CQuestManager::~CQuestManager()
{
	ClearTalk();
}

void CQuestManager::QuestProgress()
{
	// ���ϴ� ���� ��
	if (m_isTalking)
	{
		ClearTalk();
	}
	
	else
	{
		if (!m_isProgress)
		{
			m_isProgress = true;
			m_isComplete = false;
			m_isTalking = true;
			// step�� �ش��ϴ� ����Ʈ �����Ŵ.

			CreateTalkUI(m_talkStep++);
		}
		else
		{
			if (m_isComplete)
			{
				// ����Ʈ�Ϸῡ ���� ����

				++m_questStep;
				m_isComplete = false;
				m_isProgress = false;
			}
			else
			{
				// ����Ʈ �̿Ϸῡ ���� ����
			}
		}
	}
}

void CQuestManager::CreateTalkUI(const _int& index)
{
	Engine::CLayer* layer = Engine::GetCurScene()->GetLayer(L"UI");
	if (nullptr == layer) return;

	CTalkUI* talk = CTalkUI::Create(
		Engine::CGraphicDevice::GetInstance()->GetDevice(),
		index,
		0,
		WINCY - 200,
		WINCX,
		200
	);

	_tchar* name = new _tchar[10];

	wsprintf(name, L"TALK_%d", index);

	layer->AddGameObject(name, talk);

	m_nameList.emplace_back(name);
	m_talkList.emplace_back(talk);
}

void CQuestManager::QuestCheat(const _int & step, const _bool & progrss, const _bool & complete)
{
	m_questStep = step;
	m_isProgress = progrss;
	m_isComplete = complete;
}

void CQuestManager::ClearTalk()
{
	for (auto& elem : m_talkList)
	{
		elem->SetIsDead(true);
	}

	for (auto& elem : m_nameList)
	{
		delete[] elem;
		elem = nullptr;
	}
}
