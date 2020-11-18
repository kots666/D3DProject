#include "FontMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFontMgr)

Engine::CFontMgr::CFontMgr()
{

}

Engine::CFontMgr::~CFontMgr()
{
	Free();
}

HRESULT Engine::CFontMgr::Ready(LPDIRECT3DDEVICE9 device, const _tchar* fontTag, const _tchar* fontType, const _uint& width, const _uint& height, const _uint& weight)
{
	CFont* font = nullptr;

	font = Find(fontTag);
	if (nullptr != font) return E_FAIL;

	font = CFont::Create(device, fontType, width, height, weight);
	NULL_CHECK_RETURN(font, E_FAIL);

	m_fontMap.emplace(fontTag, font);

	return S_OK;
}

void Engine::CFontMgr::Render(const _tchar* fontTag, const _tchar* string, const _vec2* pos, D3DXCOLOR color)
{
	CFont* font = Find(fontTag);
	NULL_CHECK(font);

	font->Render(string, pos, color);
}

CFont* Engine::CFontMgr::Find(const _tchar* fontTag)
{
	auto iter = find_if(m_fontMap.begin(), m_fontMap.end(), CTagFinder(fontTag));

	if (m_fontMap.end() == iter) return nullptr;

	return iter->second;
}

void Engine::CFontMgr::Free()
{
	for_each(m_fontMap.begin(), m_fontMap.end(), CDeleteMap());
	m_fontMap.clear();
}

