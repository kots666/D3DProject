#ifndef GraphicDevice_h__
#define GraphicDevice_h__

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDevice : public CBase
{
	DECLARE_SINGLETON(CGraphicDevice)

private:
	explicit CGraphicDevice();
	virtual ~CGraphicDevice();

public:
	LPDIRECT3DDEVICE9 GetDevice() { return m_device; }
	LPD3DXSPRITE GetSprite() { return m_sprite; }
	LPD3DXFONT GetFont() { return m_font; }
	LPD3DXLINE GetLine() { return m_line; }

public:
	HRESULT Ready(HWND hWnd, WINMODE mode, const _uint& sizeX, const _uint& sizeY, CGraphicDevice** graphicDevice);
	void RenderBegin(D3DXCOLOR color = { 0.2f, 0.2f, 0.2f, 1.f });
	void RenderEnd(HWND hWnd = nullptr);

private:
	LPDIRECT3D9 m_SDK;
	LPDIRECT3DDEVICE9 m_device;
	LPD3DXSPRITE m_sprite;
	LPD3DXFONT m_font;
	LPD3DXLINE m_line;

public:
	virtual void Free();
};

END

#endif