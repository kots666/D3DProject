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
	m_transCom->SetTransform(m_device);

	FAILED_CHECK_RETURN(SetUpMaterial());
	m_texCom->RenderTexture(0);
	m_bufferCom->Render();
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

	return S_OK;
}

HRESULT CTerrain::SetUpMaterial()
{
	D3DMATERIAL9 mtrlInfo;
	ZeroMemory(&mtrlInfo, sizeof(D3DMATERIAL9));

	mtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	mtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	mtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	mtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	mtrlInfo.Power = 0.f;

	m_device->SetMaterial(&mtrlInfo);

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