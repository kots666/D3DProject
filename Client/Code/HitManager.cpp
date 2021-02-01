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
	for (auto& elem : m_hitEffectList)
	{
		Engine::SafeRelease(elem);
	}
	m_hitEffectList.clear();

	for (auto& elem : m_hitSlashList)
	{
		Engine::SafeRelease(elem);
	}
	m_hitSlashList.clear();

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

void CHitManager::SpawnHitEffect(const _vec3 & pos, const _float& xSize, const _float& ySize, const _float & lifeTime)
{
	for (auto& elem : m_hitEffectList)
	{
		if (!elem->GetActive())
		{
			elem->SetActive(pos, xSize, ySize, lifeTime);
			break;
		}
	}
}

void CHitManager::SpawnHitSlash(const _vec3 & pos, const _float & xSize, const _float & ySize, const _float & lifeTime)
{
	for (auto& elem : m_hitSlashList)
	{
		if (!elem->GetActive())
		{
			elem->SetActive(pos, xSize, ySize, lifeTime);
			break;
		}
	}
}

void CHitManager::ReadyEffect()
{
	_int readyCount = 20;

	m_hitEffectList.reserve(readyCount);
	m_hitSlashList.reserve(readyCount);

	_vec4 hitEffectColor = { 3.f, 3.f, 0.83f, 1.f };
	_vec4 hitSlashColor = { 3.f, 0.f, 0.f, 1.f };

	for (_int i = 0; i < readyCount; ++i)
	{
		CHitEffect* hitEffect = CHitEffect::Create(
			Engine::CGraphicDevice::GetInstance()->GetDevice(),
			hitEffectColor,
			L"Texture_HitEffect",
			3,
			3);
		Engine::SafeAddRef(hitEffect);

		m_hitEffectList.emplace_back(hitEffect);

		_tchar* effectName = new _tchar[20];
		wsprintf(effectName, L"HitEffect_%d", i);
		m_nameList.emplace_back(effectName);

		Engine::GetCurScene()->GetLayer(L"Environment")->AddGameObject(effectName, hitEffect);

		CHitEffect* hitSlash = CHitEffect::Create(
			Engine::CGraphicDevice::GetInstance()->GetDevice(),
			hitSlashColor,
			L"Texture_HitSlash",
			5,
			1,
			true);
		Engine::SafeAddRef(hitSlash);

		m_hitSlashList.emplace_back(hitSlash);

		effectName = new _tchar[20];
		wsprintf(effectName, L"HitSlash_%d", i);
		m_nameList.emplace_back(effectName);

		Engine::GetCurScene()->GetLayer(L"Environment")->AddGameObject(effectName, hitSlash);
	}
}
