#ifndef QuestManager_h__
#define QuestManager_h__

#include "Define.h"
#include "GameObject.h"

BEGIN(Client)

class CQuestManager
{
	DECLARE_SINGLETON(CQuestManager)

private:
	explicit CQuestManager();
	virtual ~CQuestManager();

public:


private:
	_int m_questStep;
};

END

#endif
