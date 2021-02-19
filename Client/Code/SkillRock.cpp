#include "stdafx.h"
#include "SkillRock.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "Texture.h"

CSkillRock::CSkillRock(LPDIRECT3DDEVICE9 device, _tchar * name, const _vec3 & pos) :
	CGameObject(device),
	m_name(name),
	m_pos(pos)
{
}

CSkillRock::~CSkillRock()
{
	if (nullptr != m_name)
	{
		delete[] m_name;
		m_name = nullptr;
	}
}

HRESULT CSkillRock::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_transCom->SetPos(m_pos);

	return S_OK;
}

_int CSkillRock::Update(const _float& deltaTime)
{
	Engine::CGameObject::Update(deltaTime);

	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CSkillRock::Render()
{
	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;
	Engine::SafeAddRef(effect);

	_uint maxPass = 0;

	effect->Begin(&maxPass, 0);

	FAILED_CHECK_RETURN(SetUpConstantTable(effect), );
	m_meshCom->Render(effect);

	effect->End();

	Engine::SafeRelease(effect);

	for (auto& elem : m_attackCollider)
	{
		elem->Render();
	}
}

HRESULT CSkillRock::AddComponent()
{
	Engine::CComponent* component = nullptr;
	
	// Mesh
	component = m_meshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Mesh_SkillRock"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Mesh", component);

	// Transform
	component = m_transCom = dynamic_cast<Engine::CTransform*>(Engine::CloneComp(L"Proto_Transform"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", component);

	// Renderer
	component = m_rendererCom = Engine::GetRenderer();
	NULL_CHECK_RETURN(component, E_FAIL);
	SafeAddRef(component);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Renderer", component);

	// Shader
	component = m_shaderCom = dynamic_cast<Engine::CShader*>(Engine::CloneComp(L"Proto_Shader_Mesh"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Shader", component);

	// Texture
	component = m_normalTexCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_NORMAL, L"Mesh_SkillRock"));
	if (nullptr == component) return S_OK;
	m_compMap[Engine::ID_STATIC].emplace(L"Com_NormalTexture", component);

	m_meshCom->AddNormalTexture(m_normalTexCom);

	return S_OK;
}

HRESULT CSkillRock::SetUpConstantTable(LPD3DXEFFECT & effect)
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

CSkillRock * CSkillRock::Create(LPDIRECT3DDEVICE9 device, _tchar * name, const _vec3 & pos)
{
	CSkillRock* instance = new CSkillRock(device, name, pos);

	if (FAILED(instance->Ready()))
		Engine::SafeRelease(instance);

	return instance;
}

void CSkillRock::Free()
{
	Engine::CGameObject::Free();
}