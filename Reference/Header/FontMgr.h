#ifndef FontMgr_h__
#define FontMgr_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Font.h"

BEGIN(Engine)

class ENGINE_DLL CFontMgr : public CBase
{
	DECLARE_SINGLETON(CFontMgr)

private:
	explicit CFontMgr();
	virtual ~CFontMgr();

public:
	HRESULT Ready(LPDIRECT3DDEVICE9 device,
						const _tchar* fontTag,
						const _tchar* fontType,
						const _uint& width,
						const _uint& height,
						const _uint& weight);

	void Render(const _tchar* fontTag, 
						const _tchar* string,
						const _vec2* pos, 
						D3DXCOLOR color);

private:
	CFont* Find(const _tchar* fontTag);

private:
	map<const _tchar*, CFont*> m_fontMap;

public:
	virtual void Free();
};

END
#endif
