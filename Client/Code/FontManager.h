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
	void ActiveNumber(const _int& inputNumber, const _vec3& pos, const _vec4& color = { 1.f, 1.f, 1.f, 1.f }, const _float& lifeTime = 2.f, const _float& xSize = 0.3f, const _float& ySize = 0.3f);

private:
	void ReadyFontUI();

private:
	vector<CFontUI*> m_fontList[10];
	list<_tchar*> m_fontNameList;
};

END

#endif
