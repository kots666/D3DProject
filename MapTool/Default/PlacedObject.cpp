#include "stdafx.h"
#include "PlacedObject.h"
#include "StaticMesh.h"
#include "TerrainTex.h"
#include "Transform.h"

CPlacedObject::CPlacedObject(LPDIRECT3DDEVICE9 device,_tchar* key, _tchar * tag) :
	CGameObject(device),
	m_key(key),
	m_tag(tag)
{
}

CPlacedObject::~CPlacedObject()
{
	if (nullptr != m_key)
	{
		delete m_key;
		m_key = nullptr;
	}
}

HRESULT CPlacedObject::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_pos = { 0.f, 0.f, 0.f };
	m_scale = { 0.01f, 0.01f, 0.01f };
	m_rotation = { 0.f, 0.f, 0.f };

	return S_OK;
}

_int CPlacedObject::Update(const _float& deltaTime)
{
	m_transCom->SetPos(m_pos.x, m_pos.y, m_pos.z);
	m_transCom->SetScale(m_scale.x, m_scale.y, m_scale.z);
	m_transCom->SetRotation(m_rotation.x, m_rotation.y, m_rotation.z);

	Engine::CGameObject::Update(deltaTime);

	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CPlacedObject::Render()
{
	m_transCom->SetTransform(m_device);

	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_meshCom->Render();
	m_device->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CPlacedObject::AddComponent()
{
	Engine::CComponent* component = nullptr;
	
	// Mesh
	component = m_meshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, m_tag));
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

	return S_OK;
}

//void CPlacedObject::SetUpOnTerrain()
//{
//	_vec3 position;
//	m_transCom->GetInfo(Engine::INFO_POS, &position);
//
//	Engine::CTerrainTex* terrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::GetComponent(L"Environment", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
//	NULL_CHECK(terrainBufferCom);
//
//	_float height = m_calcCom->ComputeHeightOnTerrain(&position, terrainBufferCom->GetVtxPos(), VTXCNTX, VTXCNTZ, VTXITV);
//
//	m_transCom->SetPos(position.x, height, position.z);
//}

CPlacedObject * CPlacedObject::Create(LPDIRECT3DDEVICE9 device, _tchar * key, _tchar * tag)
{
	CPlacedObject*	instance = new CPlacedObject(device, key, tag);

	if (FAILED(instance->Ready()))
		SafeRelease(instance);

	return instance;
}

void CPlacedObject::Free()
{
	Engine::CGameObject::Free();
}