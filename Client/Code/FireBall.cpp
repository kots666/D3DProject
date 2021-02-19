#include "stdafx.h"
#include "FireBall.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "Texture.h"

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
	m_transCom->SetScale(0.001f, 0.001f, 0.001f);
	m_transCom->SetRotation(Engine::ROT_Y, D3DXToRadian(90.f));
	
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

	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	for (auto& elem : m_attackCollider)
	{
		elem->Update(deltaTime);
		elem->UpdateByBone(m_transCom->GetWorldMatrix());
	}

	return 0;
}

void CFireBall::Render()
{
	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;

	FAILED_CHECK_RETURN(SetUpConstantTable(effect), );

	Engine::SafeAddRef(effect);

	effect->Begin(nullptr, 0);

	m_meshCom->Render(effect);
	
	effect->End();

	Engine::SafeRelease(effect);
}

_bool CFireBall::AttackColliderOverlapped(Engine::CGameObject * target)
{
	_vec3 pos;
	m_transCom->GetInfo(Engine::INFO_POS, &pos);

	CHitManager::GetInstance()->SpawnExplosiveEffect(pos);
	m_isDead = true;
	return true;
}

HRESULT CFireBall::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// Head Mesh
	component = m_meshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Mesh_ThrowRock"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Mesh", component);

	// Transform
	component = m_transCom = dynamic_cast<Engine::CTransform*>(Engine::CloneComp(L"Proto_Transform"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", component);

	// Renderer
	component = m_rendererCom = Engine::GetRenderer();
	NULL_CHECK_RETURN(component, E_FAIL);
	Engine::SafeAddRef(component);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Renderer", component);

	// Shader
	component = m_shaderCom = dynamic_cast<Engine::CShader*>(Engine::CloneComp(L"Proto_Shader_Mesh"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Shader", component);

	// Normal Texture
	component = m_normalTexCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_NORMAL, L"Mesh_ThrowRock"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_NormalTexture", component);

	m_meshCom->AddNormalTexture(m_normalTexCom);

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

HRESULT CFireBall::SetUpConstantTable(LPD3DXEFFECT & effect)
{
	_matrix matWorld, matView, matProj;

	m_transCom->GetWorldMatrix(&matWorld);
	m_device->GetTransform(D3DTS_VIEW, &matView);
	m_device->GetTransform(D3DTS_PROJECTION, &matProj);

	effect->SetMatrix("g_matWorld", &matWorld);
	effect->SetMatrix("g_matView", &matView);
	effect->SetMatrix("g_matProj", &matProj);

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

	m_transCom->SetRotation(Engine::ROT_Y, D3DXToRadian(-degree));
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