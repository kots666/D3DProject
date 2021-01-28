#ifndef QuestManager_h__
#define QuestManager_h__

#include "Define.h"

BEGIN(Client)

class CTalkUI;
class CUI;

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
	void CreateIlustUI(const _int& index);
	void CreateQuestUI();

	void ChangeTalkUI(const _int& index);
	void ChangeIlustUI(const _int& index);
	void ChangeQuestUI(const _int& index);

	void ClearTalkUI();
	void ClearIlustUI();
	void ClearQuestUI();
	void ClearAll();

	void QuestCheat(const _int& step, const _bool& progrss, const _bool& complete);


public:
	_bool GetIsProgress() const { return m_isProgress; }
	_bool GetIsComplete() const { return m_isComplete; }
	_int GetStep() const { return m_questStep; }
	
public:
	void CompleteQuest();

private:
	CTalkUI* m_talkUI;
	CTalkUI* m_ilustUI;
	CTalkUI* m_questUI;

	list<_tchar*> m_nameList;

	_int m_questStep;
	_int m_talkStep;

	_bool m_isProgress;
	_bool m_isComplete;
	_bool m_isTalking;
};

END

#endif
