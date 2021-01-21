#include "stdafx.h"
#include "FontManager.h"
#include "FontUI.h"

USING(Client)
IMPLEMENT_SINGLETON(CFontManager)

CFontManager::CFontManager()
{
	ReadyFontUI();
}

CFontManager::~CFontManager()
{
}

void CFontManager::ActiveNumber(const _int & numIndex, const _vec3 & pos, const _float & lifeTime, const _float & xSize, const _float & ySize)
{
}

void CFontManager::Create(LPDIRECT3DDEVICE9 device, Engine::CGameObject * target, const _float & sizeX, const _float & sizeY, const _tchar * texName)
{
}

void CFontManager::ReadyFontUI()
{
	for (_int i = 0; i < 10; ++i)
	{
		m_fontList[i].reserve(10);

		for (_int j = 0; j < 10; ++j)
		{
			_tchar name[20];
			wsprintf(name, L"Texture_Number_%d", i);
			
			Engine::CGameObject* font = CFontUI::Create(Engine::CGraphicDevice::GetInstance()->GetDevice(), name, 1.f, 1.f);

			_tchar* fontName = new _tchar[20];
			wsprintf(fontName, L"Font_%d_%d", i, j);
			m_fontNameList.emplace_back(fontName);

			Engine::GetCurScene()->GetLayer(L"Environment")->AddGameObject(fontName, font);
		}
	}
}
