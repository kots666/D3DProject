#include "stdafx.h"
#include "HPUIManager.h"
#include "HPUI.h"

USING(Client)
IMPLEMENT_SINGLETON(CHPUIManager)

CHPUIManager::CHPUIManager()
{
}

CHPUIManager::~CHPUIManager()
{
	for (auto& elem : m_nameList)
	{
		delete[] elem;
		elem = nullptr;
	}
	m_nameList.clear();
}

void CHPUIManager::Create(LPDIRECT3DDEVICE9 device, Engine::CGameObject * target, const _float & sizeX, const _float & sizeY, const _tchar * texName)
{
	Engine::CGameObject* newHPUI = CHPUI::Create(device, target, texName, 0, 0, sizeX, sizeY, false);
	if (nullptr == newHPUI) return;

	_tchar* newName = new _tchar[20];

	wsprintf(newName, L"HP_UI_%d", m_hpCnt);
	m_nameList.emplace_back(newName);

	Engine::GetCurScene()->GetLayer(L"Environment")->AddGameObject(newName, newHPUI);
}