#include "stdafx.h"
#include "BackMesh.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "Texture.h"

CBackMesh::CBackMesh(LPDIRECT3DDEVICE9 device, _tchar * key, _tchar * name, const _vec3 & pos, const _vec3 & scale, const _vec3 & rot) :
	CGameObject(device),
	m_key(key),
	m_name(name),
	m_pos(pos),
	m_scale(scale),
	m_rotation(rot)
{
}

CBackMesh::~CBackMesh()
{
	if (nullptr != m_name)
	{
		delete[] m_name;
		m_name = nullptr;
	}
}

HRESULT CBackMesh::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_transCom->SetScale(m_scale.x, m_scale.y, m_scale.z);
	m_transCom->SetPos(m_pos);
	m_transCom->SetRotation(m_rotation.x, m_rotation.y, m_rotation.z);

	return S_OK;
}

_int CBackMesh::Update(const _float& deltaTime)
{
	Engine::CGameObject::Update(deltaTime);

	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CBackMesh::Render()
{
	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;
	Engine::SafeAddRef(effect);

	_uint maxPass = 0;


	effect->Begin(&maxPass, 0);

	FAILED_CHECK_RETURN(SetUpConstantTable(effect), );

	if (nullptr == m_normalTexCom)
		effect->BeginPass(0);
	else
		effect->BeginPass(1);

	m_meshCom->Render(effect);

	effect->EndPass();
	effect->End();

	Engine::SafeRelease(effect);
}

HRESULT CBackMesh::AddComponent()
{
	Engine::CComponent* component = nullptr;
	
	// Mesh
	component = m_meshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, m_key));
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
	component = m_normalTexCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_NORMAL, m_key));
	if (nullptr == component) return S_OK;
	m_compMap[Engine::ID_STATIC].emplace(L"Com_NormalTexture", component);

	return S_OK;
}

HRESULT CBackMesh::SetUpConstantTable(LPD3DXEFFECT & effect)
{
	_matrix matWorld, matView, matProj;

	m_transCom->GetWorldMatrix(&matWorld);
	m_device->GetTransform(D3DTS_VIEW, &matView);
	m_device->GetTransform(D3DTS_PROJECTION, &matProj);

	effect->SetMatrix("g_matWorld", &matWorld);
	effect->SetMatrix("g_matView", &matView);
	effect->SetMatrix("g_matProj", &matProj);

	if (nullptr != m_normalTexCom)
	{
		m_normalTexCom->SetTexture(effect, "g_NormalTexture");
	}

	return S_OK;
}

CBackMesh * CBackMesh::Create(LPDIRECT3DDEVICE9 device, _tchar * key, _tchar * name, const _vec3 & pos, const _vec3 & scale, const _vec3 & rot)
{
	CBackMesh* instance = new CBackMesh(device, key, name, pos, scale, rot);

	if (FAILED(instance->Ready()))
		Engine::SafeRelease(instance);

	return instance;
}

void CBackMesh::Free()
{
	Engine::CGameObject::Free();
}