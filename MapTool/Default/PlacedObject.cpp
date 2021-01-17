#include "stdafx.h"
#include "PlacedObject.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "TerrainTex.h"
#include "Transform.h"

CPlacedObject::CPlacedObject(LPDIRECT3DDEVICE9 device,_tchar* key, _tchar * tag, _bool isDynamic) :
	CGameObject(device),
	m_key(key),
	m_tag(tag),
	m_isDynamic(isDynamic)
{
}

CPlacedObject::~CPlacedObject()
{
	if (nullptr != m_key)
	{
		delete m_key;
		m_key = nullptr;
	}

	for (auto& sphere : m_sphereColliderList)
	{
		sphere->Release();
		sphere = nullptr;
	}

	for (auto& name : m_sphereColliderNameList)
	{
		delete name;
		name = nullptr;
	}
}

HRESULT CPlacedObject::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_pos = { 0.f, 0.f, 0.f };
	m_scale = { 0.01f, 0.01f, 0.01f };
	m_rotation = { 0.f, 0.f, 0.f };

	if (m_isDynamic)
	{
		m_dynamicMeshCom->SetAnimation(0, 0.25f, 0.1f, false);
	}

	return S_OK;
}

_int CPlacedObject::Update(const _float& deltaTime)
{
	m_transCom->SetPos(m_pos.x, m_pos.y, m_pos.z);
	m_transCom->SetScale(m_scale.x, m_scale.y, m_scale.z);
	m_transCom->SetRotation(m_rotation.x, m_rotation.y, m_rotation.z);

	Engine::CGameObject::Update(deltaTime);

	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	for (auto& sphere : m_sphereColliderList)
	{
		sphere->UpdateByBone(m_transCom->GetWorldMatrix());
	}

	if (m_isDynamic)
	{
		m_dynamicMeshCom->PlayAnimation(deltaTime);
		m_dynamicMeshCom->UpdateFrameMatrices(deltaTime);
	}

	return 0;
}

void CPlacedObject::Render()
{
	m_transCom->SetTransform(m_device);

	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (m_isDynamic)
		m_dynamicMeshCom->Render();
	else
		m_meshCom->Render();

	for (auto& sphere : m_sphereColliderList)
	{
		sphere->Render();
	}

	m_device->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CPlacedObject::AddSphereCollider(_tchar* name, _matrix* boneMat, const _vec3& offset, const _float& radius)
{
	Engine::CSphereCollider* sphereColl = nullptr;

	Engine::CComponent* component = nullptr;

	component = sphereColl = Engine::CSphereCollider::Create(m_device, name, boneMat, offset, radius);
	if (nullptr == component)
		return;

	m_compMap[Engine::ID_STATIC].emplace(name, component);
	m_sphereColliderList.emplace_back(sphereColl);
	m_sphereColliderNameList.emplace_back(name);
}

void CPlacedObject::AddSphereCollider(Engine::CSphereCollider * newComp, _tchar* name)
{
	m_compMap[Engine::ID_STATIC].emplace(name, newComp);
	m_sphereColliderList.emplace_back(newComp);
}

void CPlacedObject::DeleteSphereCollider(Engine::CSphereCollider * comp, _tchar * name)
{
	auto collIter = find_if(m_sphereColliderList.begin(), m_sphereColliderList.end(), [&](CSphereCollider* coll)
	{
		return comp == coll;
	});

	if (m_sphereColliderList.end() != collIter)
	{
		m_sphereColliderList.erase(collIter);
	}

	auto nameIter = find_if(m_sphereColliderNameList.begin(), m_sphereColliderNameList.end(), [&](_tchar* collName)
	{
		return !lstrcmp(name, collName);
	});

	if (m_sphereColliderNameList.end() != nameIter)
	{
		delete *nameIter;
		m_sphereColliderNameList.erase(nameIter);
	}
}

const D3DXFRAME_EX * CPlacedObject::FindFrame(const char * name)
{
	return m_dynamicMeshCom->GetCloneFrameByName(name);
}

const D3DXFRAME_EX * CPlacedObject::GetCloneFrame()
{
	if (m_isDynamic)
	{
		return m_dynamicMeshCom->GetCloneFrame();
	}
	return nullptr;
}

HRESULT CPlacedObject::AddComponent()
{
	Engine::CComponent* component = nullptr;
	
	// Mesh
	if (m_isDynamic)
	{
		component = m_dynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, m_tag));
		NULL_CHECK_RETURN(component, E_FAIL);
		m_compMap[Engine::ID_STATIC].emplace(L"Com_Mesh", component);
	}
	else
	{
		component = m_meshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, m_tag));
		NULL_CHECK_RETURN(component, E_FAIL);
		m_compMap[Engine::ID_STATIC].emplace(L"Com_Mesh", component);
	}

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

CPlacedObject * CPlacedObject::Create(LPDIRECT3DDEVICE9 device, _tchar * key, _tchar * tag, _bool isDynamic)
{
	CPlacedObject*	instance = new CPlacedObject(device, key, tag, isDynamic);

	if (FAILED(instance->Ready()))
		SafeRelease(instance);

	return instance;
}

void CPlacedObject::Free()
{
	Engine::CGameObject::Free();
}