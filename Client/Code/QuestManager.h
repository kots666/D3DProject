#ifndef QuestManager_h__
#define QuestManager_h__

#include "Define.h"

BEGIN(Client)

class CQuestManager
{
	DECLARE_SINGLETON(CQuestManager)

private:
	explicit CQuestManager();
	virtual ~CQuestManager();

public:
	void IncreaseQuestStep() { ++m_questStep; }
	void QuestProgress();
	void CreateTalkUI(const _int& index);

	void QuestCheat(const _int& step, const _bool& progrss, const _bool& complete);

	void ClearTalk();

public:
	_bool GetIsProgress() const { return m_isProgress; }
	_bool GetIsComplete() const { return m_isComplete; }
	_int GetStep() const { return m_questStep; }
	
public:
	void CompleteQuest() { m_isComplete = true; }

private:
	list<Engine::CGameObject*> m_talkList;
	list<_tchar*> m_nameList;

	_int m_questStep;
	_int m_talkStep;

	_bool m_isProgress;
	_bool m_isComplete;
	_bool m_isTalking;
};

END

#endif
