#ifndef HPUIManager_h__
#define HPUIManager_h__

#include "Define.h"
#include "GameObject.h"

BEGIN(Client)

class CHPUIManager
{
	DECLARE_SINGLETON(CHPUIManager)

private:
	explicit CHPUIManager();
	virtual ~CHPUIManager();

public:
	void Create(LPDIRECT3DDEVICE9 device, Engine::CGameObject* target, const _float& yOffset = 2.f, const _float& sizeX = 1.5f, const _float& sizeY = 0.15f, const _tchar* texName = L"Texture_HPBar2");

private:
	_int m_hpCnt;
	list<_tchar*> m_nameList;
};

END

#endif
