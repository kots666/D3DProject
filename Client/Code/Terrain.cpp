#include "stdafx.h"
#include "Terrain.h"
#include "Texture.h"
#include "Transform.h"
#include "TerrainTex.h"
#include "RCTex.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 device)
	: Engine::CGameObject(device)
{

}

CTerrain::~CTerrain()
{

}

HRESULT CTerrain::Ready()
{
	FAILED_CHECK_RETURN(Engine::ReadyBuffer(m_device, Engine::RESOURCE_STAGE, L"Buffer_TerrainTex", Engine::BUFFER_TERRAINTEX, 129, 129), E_FAIL);
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	return S_OK;
}

_int CTerrain::Update(const _float& deltaTime)
{
	Engine::CGameObject::Update(deltaTime);

	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CTerrain::Render()
{
	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;

	Engine::SafeAddRef(effect);

	FAILED_CHECK_RETURN(SetUpConstantTable(effect), );

	_uint maxPass = 0;
	effect->Begin(&maxPass, 0);
	effect->BeginPass(0);

	m_bufferCom->Render();

	effect->EndPass();
	effect->End();

	Engine::SafeRelease(effect);
}

HRESULT CTerrain::AddComponent()
{
	Engine::CComponent* comp = nullptr;

	// buffer
	comp = m_bufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Buffer_TerrainTex"));
	NULL_CHECK_RETURN(comp, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Buffer", comp);

	// texture
	comp = m_texCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Texture_Terrain"));
	NULL_CHECK_RETURN(comp, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Texture", comp);

	// Renderer
	comp = m_rendererCom = Engine::GetRenderer();
	NULL_CHECK_RETURN(comp, E_FAIL);
	Engine::SafeAddRef(comp);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Renderer", comp);

	// Transform
	comp = m_transCom = dynamic_cast<Engine::CTransform*>(Engine::CloneComp(L"Proto_Transform"));
	NULL_CHECK_RETURN(comp, E_FAIL);
	m_compMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", comp);

	// Shader
	comp = m_shaderCom = dynamic_cast<Engine::CShader*>(Engine::CloneComp(L"Proto_Shader_Terrain"));
	NULL_CHECK_RETURN(comp, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Shader", comp);

	return S_OK;
}

HRESULT CTerrain::SetUpConstantTable(LPD3DXEFFECT & effect)
{
	_matrix worldMat, viewMat, projMat;

	m_transCom->GetWorldMatrix(&worldMat);
	m_device->GetTransform(D3DTS_VIEW, &viewMat);
	m_device->GetTransform(D3DTS_PROJECTION, &projMat);

	effect->SetMatrix("g_matWorld", &worldMat);
	effect->SetMatrix("g_matView", &viewMat);
	effect->SetMatrix("g_matProj", &projMat);

	effect->SetFloat("g_detail", 129.f);

	m_texCom->SetTexture(effect, "g_BaseTexture");

	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 device)
{
	CTerrain* instance = new CTerrain(device);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CTerrain::Free()
{
	Engine::CGameObject::Free();
}