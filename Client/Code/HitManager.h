#ifndef HitManager_h__
#define HitManager_h__

#include "Define.h"
#include "GameObject.h"

BEGIN(Client)

class CHitEffect;

class CHitManager
{
	DECLARE_SINGLETON(CHitManager)

private:
	explicit CHitManager();
	virtual ~CHitManager();

public:
	void Spawn(const _vec3& pos, const _float& lifeTime = 1.f);

private:
	void ReadyEffect();

private:
	vector<CHitEffect*> m_hitEffectList;
	list<_tchar*> m_nameList;
};

END

#endif
