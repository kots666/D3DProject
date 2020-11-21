#include "stdafx.h"
#include "Stone.h"
#include "TerrainTex.h"
#include "Transform.h"
#include "StaticMesh.h"
#include "Collider.h"

CStone::CStone(LPDIRECT3DDEVICE9 device) :
	Engine::CGameObject(device)
{

}

CStone::~CStone()
{
}

HRESULT Client::CStone::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_transCom->SetPos(10.f, 0.f, 5.f);
	m_transCom->Rotation(Engine::ROT_Y, D3DXToRadian(45.f));

	return S_OK;
}

Client::_int Client::CStone::Update(const _float& deltaTime)
{
	SetUpOnTerrain();

	Engine::CGameObject::Update(deltaTime);

	//m_isColl = CollideToObject(L"GameLogic", L"Player");

	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	return 0;
}

void Client::CStone::Render()
{
	m_transCom->SetTransform(m_device);

	m_meshCom->Render();

	_matrix matWorld;
	//m_transCom->GetWorldMatrix(&matWorld);
	//m_transCom->GetNRotWorldMatrix(&matWorld);

	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_colliderCom->Render(Engine::COLLTYPE(m_isColl), &matWorld);
}

HRESULT Client::CStone::AddComponent()
{
	Engine::CComponent* component = nullptr;
	
	// Mesh
	component = m_meshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Mesh_Stone"));
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

	// Calculator
	component = m_calcCom = dynamic_cast<Engine::CCalculator*>(Engine::CloneComp(L"Proto_Calculator"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Calculator", component);

	// Collider
	component = m_colliderCom = Engine::CCollider::Create(m_device, m_meshCom->GetVtxPos(), m_meshCom->GetNumVtx(), m_meshCom->GetStride());
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Collider", component);

	return S_OK;
}

CStone* CStone::Create(LPDIRECT3DDEVICE9 device)
{
	CStone*	instance = new CStone(device);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CStone::Free()
{
	Engine::CGameObject::Free();
}

void Client::CStone::SetUpOnTerrain()
{
	_vec3 position;
	m_transCom->GetInfo(Engine::INFO_POS, &position);

	Engine::CTerrainTex* terrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::GetComponent(L"Environment", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
	NULL_CHECK(terrainBufferCom);

	_float height = m_calcCom->ComputeHeightOnTerrain(&position, terrainBufferCom->GetVtxPos(), VTXCNTX, VTXCNTZ, VTXITV);

	m_transCom->SetPos(position.x, height, position.z);
}

_bool CStone::CollideToObject(const _tchar * layerTag, const _tchar * objTag)
{
	Engine::CCollider* playerColliderCom = dynamic_cast<Engine::CCollider*>(Engine::GetComponent(layerTag, objTag, L"Com_Collider", Engine::ID_STATIC));
	NULL_CHECK_RETURN(playerColliderCom, false);

	return m_calcCom->CollisionAABB(
		playerColliderCom->GetMinPos(),
		playerColliderCom->GetMaxPos(),
		playerColliderCom->GetCollMatrix(),
		m_colliderCom->GetMinPos(),
		m_colliderCom->GetMaxPos(),
		m_colliderCom->GetCollMatrix());

	/*return m_calcCom->CollisionOBB(
		playerColliderCom->GetMinPos(),
		playerColliderCom->GetMaxPos(),
		playerColliderCom->GetCollMatrix(),
		m_colliderCom->GetMinPos(),
		m_colliderCom->GetMaxPos(),
		m_colliderCom->GetCollMatrix());*/
}
