#include "stdafx.h"
#include "FireBall.h"
#include "StaticMesh.h"
#include "Transform.h"

CFireBall::CFireBall(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _vec3& target) :
	Engine::CGameObject(device),
	m_pos(pos),
	m_target(target)
{

}

CFireBall::~CFireBall()
{
}

HRESULT CFireBall::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	FAILED_CHECK_RETURN(LoadCollider(), E_FAIL);

	CColliderManager::GetInstance()->AddObject(OBJ_ENEMY, this);

	m_transCom->SetPos(m_pos);
	m_transCom->SetScale(0.05f, 0.05f, 0.05f);
	
	LookAtTarget();

	m_speed = 20.f;
	m_lifeTime = 5.f;

	return S_OK;
}

_int CFireBall::Update(const _float& deltaTime)
{
	m_lifeTime -= deltaTime;
	if (0 >= m_lifeTime)
	{
		m_isDead = true;
	}

	m_transCom->MovePos(&(m_dir * m_speed * deltaTime));

	Engine::CGameObject::Update(deltaTime);

	m_rendererCom->AddObject(Engine::RENDER_ALPHA, this);

	for (auto& elem : m_attackCollider)
	{
		elem->Update(deltaTime);
		elem->UpdateByBone(m_transCom->GetWorldMatrix());
	}

	return 0;
}

void CFireBall::Render()
{
	m_transCom->SetTransform(m_device);

	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_headMeshCom->Render();
	m_tailMeshCom->Render();

	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	for (auto& elem : m_attackCollider)
	{
		elem->Render();
	}
}

_bool CFireBall::AttackColliderOverlapped(Engine::CGameObject * target)
{
	m_isDead = true;
	return true;
}

HRESULT CFireBall::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// Head Mesh
	component = m_headMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Mesh_Projectile_Head"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_DYNAMIC].emplace(L"Com_HeadMesh", component);

	// Tail Mesh
	component = m_tailMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Mesh_Projectile_Tail"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_DYNAMIC].emplace(L"Com_TailMesh", component);

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

HRESULT CFireBall::LoadCollider()
{
	Engine::CSphereCollider* newSphere = Engine::CSphereCollider::Create(m_device, { -15.f, 0.f, 0.f }, 15.f);
	NULL_CHECK_RETURN(newSphere, E_FAIL);
	newSphere->SetScale(0.05f);
	m_attackCollider.emplace_back(newSphere);

	return S_OK;
}

void CFireBall::LookAtTarget()
{
	m_dir = m_target - m_pos;
	D3DXVec3Normalize(&m_dir, &m_dir);

	_vec3 targetDir = m_target - m_pos;
	targetDir.y = 0.f;
	D3DXVec3Normalize(&targetDir, &targetDir);

	_vec3 curDir = { 0.f, 0.f, 1.f };

	_float dot = D3DXVec3Dot(&curDir, &targetDir);
	_float cosAngle = acosf(dot);

	_float angle = (curDir.x * targetDir.z - curDir.z * targetDir.x > 0.f) ? cosAngle : -cosAngle;
	_float degree = D3DXToDegree(angle);

	//m_transCom->SetRotation(Engine::ROT_X, D3DXToRadian(-90.f));
	m_transCom->SetRotation(Engine::ROT_Y, D3DXToRadian(-degree - 90.f));
}

CFireBall* CFireBall::Create(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _vec3& target)
{
	CFireBall* instance = new CFireBall(device, pos, target);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CFireBall::Free()
{
	CColliderManager::GetInstance()->DeleteObject(OBJ_ENEMY, this);
	CSpawnManager::GetInstance()->DeleteObject(SPAWNTYPE::FIREBALL, this);

	Engine::CGameObject::Free();
}