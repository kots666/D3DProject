#ifndef Font_h__
#define Font_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFont : public CBase
{
private:
	explicit CFont(LPDIRECT3DDEVICE9 device);
	virtual ~CFont();

public:
	HRESULT Ready(const _tchar* fontType, 
				  const _uint& width, 
				  const _uint& height,
				  const _uint& weight);

	void Render(const _tchar* string, const _vec2* pos, D3DXCOLOR color);

private:
	LPDIRECT3DDEVICE9 m_device;
	LPD3DXFONT m_font;
	LPD3DXSPRITE m_sprite;

public:
	static CFont* Create(LPDIRECT3DDEVICE9 device,
						 const _tchar* fontType,
						 const _uint& width,
						 const _uint& height,
						 const _uint& weight);

private:
	virtual void Free();

};

END

#endif
