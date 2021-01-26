#include "stdafx.h"
#include "NPC.h"
#include "DynamicMesh.h"
#include "Transform.h"
#include "Texture.h"

CNPC::CNPC(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle) :
	Engine::CGameObject(device),
	m_startPosition(pos),
	m_angle(angle)
{

}

CNPC::~CNPC()
{
}

HRESULT CNPC::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	FAILED_CHECK_RETURN(LoadCollider(), E_FAIL);

	D3DXMatrixRotationY(&m_reviseMat, D3DXToRadian(-90.f));

	m_transCom->SetPos(m_startPosition);
	m_transCom->SetScale(0.01f, 0.01f, 0.01f);
	m_transCom->SetRotation(Engine::ROT_Y, D3DXToRadian(m_angle));

	m_meshCom->SetBoneName("Bip001");

	m_meshCom->SetAnimation(0, 0.1f, 0.5f, false);

	CColliderManager::GetInstance()->AddObject(OBJ_EVENT, this);

	return S_OK;
}

_int CNPC::Update(const _float& deltaTime)
{
	m_meshCom->PlayAnimation(deltaTime);
	m_meshCom->UpdateFrameMatrices(deltaTime, &m_reviseMat);

	Engine::CGameObject::Update(deltaTime);

	for (auto& elem : m_hitCollider)
	{
		elem->Update(deltaTime);
		elem->UpdateByBone(m_transCom->GetWorldMatrix());
	}

	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CNPC::Render()
{
	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;

	_int passIndex[3] = { 1, 3, 1 };

	Engine::SafeAddRef(effect);

	effect->Begin(nullptr, 0);

	FAILED_CHECK_RETURN(SetUpConstantTable(effect), );
	m_meshCom->Render(effect, passIndex);

	effect->End();

	Engine::SafeRelease(effect);

	for (auto& elem : m_hitCollider)
	{
		elem->Render();
	}
}

void CNPC::HitColliderOverlapped(Engine::CGameObject * causer)
{
	if (Engine::GetDIKeyDownState('F'))
	{
		CQuestManager::GetInstance()->QuestProgress();
	}
}

HRESULT CNPC::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// buffer
	component = m_meshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Mesh_NPC"));
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

	// body - hair - cloth
	// body
	component = m_normalCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_NORMAL, L"Texture_NPC_Body"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_BodyNormal", component);
	m_meshCom->AddNormalTexture(m_normalCom);

	// hair
	component = m_normalCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_NORMAL, L"Texture_NPC_Hair"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_HairNormal", component);
	m_meshCom->AddNormalTexture(m_normalCom);

	// cloth
	component = m_normalCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_NORMAL, L"Texture_NPC_Cloth"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_ClothNormal", component);
	m_meshCom->AddNormalTexture(m_normalCom);

	return S_OK;
}

HRESULT CNPC::LoadCollider()
{
	Engine::CSphereCollider* newSphere = Engine::CSphereCollider::Create(m_device, { 0.f, 100.f, 0.f }, 150.f);
	NULL_CHECK_RETURN(newSphere, E_FAIL);
	m_hitCollider.emplace_back(newSphere);

	return S_OK;
}

HRESULT CNPC::SetUpConstantTable(LPD3DXEFFECT & effect)
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

void CNPC::LookAtTarget(const _vec3& targetPos)
{
	_vec3 myPos;
	m_transCom->GetInfo(Engine::INFO_POS, &myPos);

	_vec3 targetDir = targetPos - myPos;
	targetDir.y = 0.f;
	D3DXVec3Normalize(&targetDir, &targetDir);

	_vec3 curDir = { 0.f, 0.f, 1.f };

	_float dot = D3DXVec3Dot(&curDir, &targetDir);
	_float cosAngle = acosf(dot);

	_float angle = (curDir.x * targetDir.z - curDir.z * targetDir.x > 0.f) ? cosAngle : -cosAngle;
	_float degree = D3DXToDegree(angle);

	m_transCom->SetRotation(Engine::ROT_Y, D3DXToRadian(-degree));
}

CNPC* CNPC::Create(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle)
{
	CNPC* instance = new CNPC(device, pos, angle);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CNPC::Free()
{
	CColliderManager::GetInstance()->DeleteObject(OBJ_EVENT, this);

	Engine::CGameObject::Free();
}