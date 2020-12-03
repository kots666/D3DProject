#include "stdafx.h"
#include "Sword.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "Renderer.h"
#include "Calculator.h"
#include "Collider.h"
#include "DynamicMesh.h"

CSword::CSword(LPDIRECT3DDEVICE9 device) :
	CGameObject(device)
{
}

CSword::~CSword()
{
}

HRESULT CSword::AddComponent()
{
	Engine::CComponent* component = nullptr;
	
	// Mesh
	component = m_meshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Mesh_Sword"));
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


CSword* CSword::Create(LPDIRECT3DDEVICE9 device)
{
	CSword*	instance = new CSword(device);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CSword::Free()
{
	Engine::CGameObject::Free();
}


HRESULT CSword::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_transCom->SetRotation(Engine::ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CSword::Update(const _float& deltaTime)
{
	if (nullptr == m_parentBoneMatrix)
	{
		Engine::CDynamicMesh* playerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::GetComponent(L"GameLogic", L"Player", L"Com_Mesh", Engine::ID_STATIC));
		NULL_CHECK_RETURN(playerMeshCom, 0);

		const Engine::D3DXFRAME_EX* EXFrame = playerMeshCom->GetFrameByName("R_Hand");

		m_parentBoneMatrix = EXFrame->combinedTransformationMatrix;

		Engine::CTransform* playerTransCom = dynamic_cast<Engine::CTransform*>(Engine::GetComponent(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(playerTransCom, 0);
		m_parentWorldMatrix = playerTransCom->GetWorldMatrix();
	}

	Engine::CGameObject::Update(deltaTime);
		
	m_transCom->SetParentMatrix(&(*m_parentBoneMatrix * *m_parentWorldMatrix));
	

	//m_isColl = Collision_ToObject(L"GameLogic", L"Player");

	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	return 0;
}
void CSword::Render()
{
	m_transCom->SetTransform(m_device);

	m_meshCom->Render();

	_matrix matWorld;

	m_transCom->GetWorldMatrix(&matWorld);
	//m_transCom->GetNRotWorldMatrix(&matWorld);

	m_colliderCom->Render(Engine::COLLTYPE(m_isColl), &matWorld);
}


_bool CSword::CollisionToObject(const _tchar * layerTag, const _tchar * objTag)
{
	Engine::CCollider* playerColliderCom = dynamic_cast<Engine::CCollider*>(Engine::GetComponent(layerTag, objTag, L"Com_Collider", Engine::ID_STATIC));
	if (nullptr == playerColliderCom) return false;
	
	/*return m_calcCom->Collision_AABB(playerColliderCom->Get_Min(),
											playerColliderCom->Get_Max(),
											playerColliderCom->Get_CollMatrix(),
											m_colliderCom->Get_Min(), 
											m_colliderCom->Get_Max(),
											m_colliderCom->Get_CollMatrix());*/

	return m_calcCom->CollisionOBB(playerColliderCom->GetMinPos(),
		playerColliderCom->GetMaxPos(),
		playerColliderCom->GetCollMatrix(),
		m_colliderCom->GetMinPos(),
		m_colliderCom->GetMaxPos(),
		m_colliderCom->GetCollMatrix());
}
