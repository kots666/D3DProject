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
	void SpawnHitEffect(const _vec3& pos, const _float& xSize = 2.f, const _float& ySize = 2.f, const _float& lifeTime = 0.2f);
	void SpawnHitSlash(const _vec3& pos, const _float& xSize = 0.8f, const _float& ySize = 4.f, const _float& lifeTime = 0.2f);

private:
	void ReadyEffect();

private:
	vector<CHitEffect*> m_hitEffectList;
	vector<CHitEffect*> m_hitSlashList;
	list<_tchar*> m_nameList;
};

END

#endif
