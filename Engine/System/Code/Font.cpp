#include "Font.h"

USING(Engine)

CFont::CFont(LPDIRECT3DDEVICE9 device) :
	m_device(device),
	m_font(nullptr),
	m_sprite(nullptr)
{
	m_device->AddRef();
}

CFont::~CFont()
{

}

HRESULT CFont::Ready(const _tchar* fontType, const _uint& width, const _uint& height, const _uint& weight)
{
	D3DXFONT_DESC Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	Font_Desc.CharSet = HANGUL_CHARSET;
	lstrcpy(Font_Desc.FaceName, fontType);
	Font_Desc.Width  = width;
	Font_Desc.Height = height;
	Font_Desc.Weight = weight;

	if (FAILED(D3DXCreateFontIndirect(m_device, &Font_Desc, &m_font)))
	{
		MSG_BOX("Font Create Failed");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_device, &m_sprite)))
	{
		MSG_BOX("Sprite Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CFont::Render(const _tchar* string, const _vec2* pos, D3DXCOLOR color)
{
	RECT rc{ _long(pos->x), _long(pos->y) };

	m_sprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_font->DrawTextW(m_sprite, string, lstrlen(string), &rc, DT_NOCLIP, color);

	m_sprite->End();
}

CFont* CFont::Create(LPDIRECT3DDEVICE9 device, const _tchar* fontType, const _uint& width, const _uint& height, const _uint& weight)
{
	CFont* instance = new CFont(device);

	if (FAILED(instance->Ready(fontType, width, height, weight)))
	{
		MSG_BOX("Font Create Failed");
		SafeRelease(instance);
	}

	return instance;
}

void CFont::Free()
{
	SafeRelease(m_font);
	SafeRelease(m_sprite);
	SafeRelease(m_device);
}

