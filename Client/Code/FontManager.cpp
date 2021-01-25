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
	for (_int i = 0; i < 10; ++i)
	{
		for (auto& elem : m_fontList[i])
		{
			Engine::SafeRelease(elem);
		}
		m_fontList[i].clear();
	}

	for (auto elem : m_fontNameList)
	{
		if (nullptr != elem)
		{
			delete[] elem;
			elem = nullptr;
		}
	}
	m_fontNameList.clear();
}

void CFontManager::ActiveNumber(const _int & inputNumber, const _vec3 & pos, const _float & lifeTime, const _float & xSize, const _float & ySize)
{
	_int num = inputNumber;
	list<_int> numList;

	while (true)
	{
		if (10 > num)
		{
			numList.emplace_front(num);
			break;
		}

		numList.emplace_front(num % 10);

		num /= 10;
	}

	_float sizeOffset = (1.f - xSize) * 0.5f;

	_vec3 offset = { (numList.size() * 0.5f) * -(xSize + sizeOffset), 0.f, 0.f };

	for (auto& num : numList)
	{
		for (auto elem : m_fontList[num])
		{
			if (!elem->GetActive())
			{
				elem->Active(pos, offset, xSize, ySize, lifeTime);
				break;
			}
		}
		offset.x += xSize + sizeOffset;
	}
}

void CFontManager::ReadyFontUI()
{
	for (_int i = 0; i < 10; ++i)
	{
		m_fontList[i].reserve(10);

		for (_int j = 0; j < 10; ++j)
		{
			_tchar name[20];
			wsprintf(name, L"Texture_Number", i);
			
			CFontUI* font = CFontUI::Create(Engine::CGraphicDevice::GetInstance()->GetDevice(), name, i, 1.f, 1.f);
			Engine::SafeAddRef(font);
			m_fontList[i].emplace_back(font);

			_tchar* fontName = new _tchar[20];
			wsprintf(fontName, L"Font_%d_%d", i, j);
			m_fontNameList.emplace_back(fontName);

			Engine::GetCurScene()->GetLayer(L"Environment")->AddGameObject(fontName, font);
		}
	}
}
