#include "stdafx.h"
#include "HitManager.h"
#include "HitEffect.h"

USING(Client)
IMPLEMENT_SINGLETON(CHitManager)

CHitManager::CHitManager()
{
	ReadyEffect();
}

CHitManager::~CHitManager()
{
	for (_int i = 0; i < 10; ++i)
	{
		for (auto& elem : m_hitEffectList)
		{
			Engine::SafeRelease(elem);
		}
		m_hitEffectList.clear();
	}

	for (auto elem : m_nameList)
	{
		if (nullptr != elem)
		{
			delete[] elem;
			elem = nullptr;
		}
	}
	m_nameList.clear();
}

void CHitManager::Spawn(const _vec3 & pos, const _float & lifeTime)
{
	for (auto& elem : m_hitEffectList)
	{
		if (!elem->GetActive())
		{
			elem->SetActive(pos, lifeTime);
			break;
		}
	}
}

void CHitManager::ReadyEffect()
{
	_int readyCount = 10;

	m_hitEffectList.reserve(readyCount);

	for (_int i = 0; i < readyCount; ++i)
	{
		CHitEffect* hitEffect = CHitEffect::Create(Engine::CGraphicDevice::GetInstance()->GetDevice(), { 0.f, 0.f, 0.f });
		Engine::SafeAddRef(hitEffect);

		m_hitEffectList.emplace_back(hitEffect);

		_tchar* effectName = new _tchar[20];
		wsprintf(effectName, L"HitEffect_%d", i);
		m_nameList.emplace_back(effectName);

		Engine::GetCurScene()->GetLayer(L"Environment")->AddGameObject(effectName, hitEffect);
	}
}
