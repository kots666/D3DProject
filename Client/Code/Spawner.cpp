#include "stdafx.h"
#include "Spawner.h"
#include "DynamicMesh.h"
#include "NaviMesh.h"
#include "Transform.h"
#include "Calculator.h"
#include "Collider.h"
#include "HPUI.h"

CSpawner::CSpawner(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& radius) :
	Engine::CGameObject(device),
	m_pos(pos),
	m_radius(radius)
{

}

CSpawner::~CSpawner()
{
}

HRESULT CSpawner::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	FAILED_CHECK_RETURN(LoadCollider(), E_FAIL);

	m_transCom->SetPos(m_pos);
	m_transCom->SetScale(0.01f, 0.01f, 0.01f);

	CColliderManager::GetInstance()->AddObject(OBJ_EVENT, this);

	return S_OK;
}

_int CSpawner::Update(const _float& deltaTime)
{
	Engine::CGameObject::Update(deltaTime);

	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	for (auto& elem : m_hitCollider)
	{
		elem->Update(deltaTime);
		elem->UpdateByBone(m_transCom->GetWorldMatrix());
	}

	return 0;
}

void CSpawner::Render()
{
#ifdef _DEBUG
	for (auto& elem : m_hitCollider)
	{
		elem->Render();
	}
//
//	for (auto& elem : m_attackCollider)
//	{
//		elem->Render();
//	}
#endif
}

void CSpawner::HitColliderOverlapped(Engine::CGameObject * causer)
{
	if (CSpawnManager::GetInstance()->StartEvent())
		m_isDead = true;
}

HRESULT CSpawner::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// Transform
	component = m_transCom = dynamic_cast<Engine::CTransform*>(Engine::CloneComp(L"Proto_Transform"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", component);

	// Renderer
	component = m_rendererCom = Engine::GetRenderer();
	NULL_CHECK_RETURN(component, E_FAIL);
	Engine::SafeAddRef(component);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Renderer", component);

	return S_OK;
}

HRESULT CSpawner::LoadCollider()
{
	Engine::CSphereCollider* newSphere = Engine::CSphereCollider::Create(m_device, { 0.f, 0.f, 0.f }, m_radius);
	NULL_CHECK_RETURN(newSphere, E_FAIL);
	m_hitCollider.emplace_back(newSphere);

	return S_OK;
}

CSpawner* CSpawner::Create(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& radius)
{
	CSpawner* instance = new CSpawner(device, pos, radius);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CSpawner::Free()
{
	CColliderManager::GetInstance()->DeleteObject(OBJ_EVENT, this);

	Engine::CGameObject::Free();
}