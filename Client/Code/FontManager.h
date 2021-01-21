#ifndef FontManager_h__
#define FontManager_h__

#include "Define.h"
#include "GameObject.h"

BEGIN(Client)

class CFontUI;

class CFontManager
{
	DECLARE_SINGLETON(CFontManager)

private:
	explicit CFontManager();
	virtual ~CFontManager();

public:
	void ActiveNumber(const _int& numIndex, const _vec3& pos, const _float& lifeTime = 2.f, const _float& xSize = 1.f, const _float& ySize = 1.f);
	void Create(LPDIRECT3DDEVICE9 device, Engine::CGameObject* target, const _float& sizeX = 1.5f, const _float& sizeY = 0.15f, const _tchar* texName = L"Texture_HPBar2");

private:
	void ReadyFontUI();

private:
	vector<CFontUI*> m_fontList[10];
	list<_tchar*> m_fontNameList;
};

END

#endif
