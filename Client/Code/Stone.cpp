#include "stdafx.h"
#include "Stone.h"
#include "TerrainTex.h"
#include "Transform.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "Collider.h"

CStone::CStone(LPDIRECT3DDEVICE9 device) :
	Engine::CGameObject(device)
{

}

CStone::~CStone()
{
}

HRESULT CStone::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_meshCom->SetAnimation(10, 0.15f, 0.1f, false);
	m_transCom->SetScale(0.01f, 0.01f, 0.01f);
	m_transCom->SetPos(10.f, 0.f, 5.f);
	m_transCom->SetRotation(Engine::ROT_Y, D3DXToRadian(45.f));

	return S_OK;
}

_int CStone::Update(const _float& deltaTime)
{
	SetUpOnTerrain();

	Engine::CGameObject::Update(deltaTime);

	//m_isColl = CollideToObject(L"GameLogic", L"Player");

	_vec3 pos;
	m_transCom->GetInfo(Engine::INFO_POS, &pos);

	//m_isDraw = m_optimizationCom->IsInFrustumForObject(&pos, 0.f);

	m_meshCom->PlayAnimation(deltaTime);
	m_meshCom->UpdateFrameMatrices(deltaTime);
	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CStone::Render()
{
	//if (!m_isDraw) return;

	m_transCom->SetTransform(m_device);

	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_meshCom->Render();
	m_device->SetRenderState(D3DRS_LIGHTING, TRUE);

	_matrix matWorld;
	//m_transCom->GetWorldMatrix(&matWorld);
	//m_transCom->GetNRotWorldMatrix(&matWorld);

	
	//m_colliderCom->Render(Engine::COLLTYPE(m_isColl), &matWorld);
}

HRESULT CStone::AddComponent()
{
	Engine::CComponent* component = nullptr;
	
	// Mesh
	component = m_meshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Mesh_Player"));
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

	//// Collider
	//component = m_colliderCom = Engine::CCollider::Create(m_device, m_meshCom->GetVtxPos(), m_meshCom->GetNumVtx(), m_meshCom->GetStride());
	//NULL_CHECK_RETURN(component, E_FAIL);
	//m_compMap[Engine::ID_STATIC].emplace(L"Com_Collider", component);

	/*component = m_optimizationCom = dynamic_cast<Engine::COptimization*>(Engine::CloneComp(L"Proto_Optimization"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Optimization", component);*/

	return S_OK;
}

void CStone::SetUpOnTerrain()
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
	if (nullptr == playerColliderCom) return false;

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