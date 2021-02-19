#include "stdafx.h"
#include "HitManager.h"
#include "HitEffect.h"
#include "CrossEffect.h"

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

	for (auto& elem : m_flashHitEffectList)
	{
		Engine::SafeRelease(elem);
	}
	m_flashHitEffectList.clear();

	for (auto& elem : m_explosiveEffectList)
	{
		Engine::SafeRelease(elem);
	}
	m_explosiveEffectList.clear();

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

void CHitManager::SpawnFlashHitEffect(const _vec3 & pos, const _float & xSize, const _float & ySize, const _float & lifeTime)
{
	for (auto& elem : m_flashHitEffectList)
	{
		if (!elem->GetActive())
		{
			elem->SetActive(pos, xSize, ySize, lifeTime);
			break;
		}
	}
}

void CHitManager::SpawnExplosiveEffect(const _vec3 & pos, const _float & xSize, const _float & ySize, const _float & lifeTime)
{
	for (auto& elem : m_explosiveEffectList)
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
	m_flashHitEffectList.reserve(readyCount);
	m_explosiveEffectList.reserve(readyCount);

	_vec4 hitEffectColor = { 3.f, 3.f, 0.83f, 0.8f };
	_vec4 hitSlashColor = { 10000.f, 10000.f, 10000.f, 1.f };
	_vec4 playerHitEffectColor = { 3.f, 0.f, 0.f, 1.f };
	_vec4 explosiveEffectColor = { 1.f, 0.9f, 0.6f, 1.f };

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
			1,
			8,
			true,
			1);
		Engine::SafeAddRef(hitSlash);

		m_hitSlashList.emplace_back(hitSlash);

		effectName = new _tchar[20];
		wsprintf(effectName, L"HitSlash_%d", i);
		m_nameList.emplace_back(effectName);

		Engine::GetCurScene()->GetLayer(L"Environment")->AddGameObject(effectName, hitSlash);

		CHitEffect* playerHitEffect = CHitEffect::Create(
			Engine::CGraphicDevice::GetInstance()->GetDevice(),
			playerHitEffectColor,
			L"Texture_HitEffect",
			3,
			3);
		Engine::SafeAddRef(playerHitEffect);

		m_flashHitEffectList.emplace_back(playerHitEffect);

		effectName = new _tchar[20];
		wsprintf(effectName, L"PlayerHitEffect_%d", i);
		m_nameList.emplace_back(effectName);

		Engine::GetCurScene()->GetLayer(L"Environment")->AddGameObject(effectName, playerHitEffect);

		CHitEffect* explosiveEffect = CHitEffect::Create(
			Engine::CGraphicDevice::GetInstance()->GetDevice(),
			explosiveEffectColor,
			L"Texture_Explosive",
			8,
			8,
			false,
			1);
		Engine::SafeAddRef(explosiveEffect);

		m_explosiveEffectList.emplace_back(explosiveEffect);

		effectName = new _tchar[20];
		wsprintf(effectName, L"ExplosiveEffect_%d", i);
		m_nameList.emplace_back(effectName);

		Engine::GetCurScene()->GetLayer(L"Environment")->AddGameObject(effectName, explosiveEffect);
	}
}
