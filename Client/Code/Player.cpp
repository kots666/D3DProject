#include "stdafx.h"
#include "Player.h"
#include "DynamicMesh.h"
#include "Transform.h"
#include "Calculator.h"
#include "Collider.h"
#include "TerrainTex.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 device) :
	Engine::CGameObject(device),
	m_dir(0.f, 0.f, 0.f),
	m_yRotAngle(0.f)
{

}

CPlayer::~CPlayer()
{

}

HRESULT CPlayer::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_transCom->SetScale(0.01f, 0.01f, 0.01f);

	// Goku
	//m_meshCom->SetAnimationSet(0);

	// Player
	m_meshCom->SetAnimationSet(0);
	m_meshCom->SetIsRootMotion(true);
	m_meshCom->SetBoneName("Bip001");

	return S_OK;
}

_int CPlayer::Update(const _float& deltaTime)
{
	SetUpOnTerrain();
	KeyInput(deltaTime);
	UpdateAnimMatrices();

	Engine::CGameObject::Update(deltaTime);

	_vec3 movePos;
	_vec3 scale;
	m_meshCom->CalcMovePos("Bip001", movePos);

	D3DXVec3TransformNormal(&movePos, &movePos, &m_yScaleRotMat);

	//movePos.x = 0;
	movePos.y = 0;

	m_transCom->MovePos(&movePos);
	m_transCom->SetMovePosAtWorldMatrix(&movePos);

	_vec3 pos;
	m_transCom->GetInfo(Engine::INFO_POS, &pos);
	cout << "X : " << pos.x << ", Y : " << pos.y << ", Z : " << pos.z << endl;

	m_meshCom->UpdateFrameMatrices(deltaTime, &m_yRotMat);
	m_meshCom->PlayAnimation(deltaTime);
	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CPlayer::Render()
{
	m_transCom->SetTransform(m_device);
	m_meshCom->Render();

	/*
	_matrix matWorld;
	m_transCom->GetWorldMatrix(&matWorld);

	m_colliderCom->Render(Engine::COLLTYPE::COL_TRUE, &matWorld);
	*/
}

HRESULT CPlayer::AddComponent()
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
	Engine::SafeAddRef(component);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Renderer", component);

	// Calculator
	component = m_calcCom = dynamic_cast<Engine::CCalculator*>(Engine::CloneComp(L"Proto_Calculator"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Calculator", component);

	// Collider
	//component = m_colliderCom = Engine::CCollider::Create(m_device, m_meshCom->GetVtxPos(), m_meshCom->GetNumVtx(), m_meshCom->GetStride());
	//NULL_CHECK_RETURN(component, E_FAIL);
	//m_compMap[Engine::ID_STATIC].emplace(L"Com_Collider", component);

	return S_OK;
}

void CPlayer::KeyInput(const _float& deltaTime)
{
	m_transCom->GetInfo(Engine::INFO_LOOK, &m_dir);

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		// Goku Attack
		//m_meshCom->SetAnimationSet(1);

		// Player
		m_meshCom->SetAnimationSet(0);
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		D3DXVec3Normalize(&m_dir, &m_dir);
		m_transCom->MovePos(&(m_dir * -m_speed * deltaTime));
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_transCom->SetRotation(Engine::ROT_Y, D3DXToRadian(90.f * deltaTime));

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_transCom->SetRotation(Engine::ROT_Y, D3DXToRadian(-90.f * deltaTime));

	if (Engine::GetDIMouseState(Engine::DIM_LB) & 0x80)
	{
		_vec3 pickPos = PickUpOnTerrain();
		m_transCom->MoveToPickPos(&pickPos, m_speed, deltaTime);
	}

	if (Engine::GetDIMouseState(Engine::DIM_RB) & 0x80)
	{
		// Player
		m_meshCom->SetAnimationSet(2, true);
	}

	if (m_meshCom->IsAnimationSetEnd())
	{
		// Player
		m_meshCom->SetAnimationSet(0);
	}
}

void CPlayer::SetUpOnTerrain()
{
	_vec3 position;
	m_transCom->GetInfo(Engine::INFO_POS, &position);

	Engine::CTerrainTex* terrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::GetComponent(L"Environment", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
	NULL_CHECK(terrainBufferCom);

	_float height = m_calcCom->ComputeHeightOnTerrain(&position, terrainBufferCom->GetVtxPos(), VTXCNTX, VTXCNTZ, VTXITV);

	m_transCom->SetPos(position.x, height, position.z);
}

_vec3 CPlayer::PickUpOnTerrain()
{
	Engine::CTerrainTex* terrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::GetComponent(L"Environment", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
	if (nullptr == terrainBufferCom)
		return _vec3(0.f, 0.f, 0.f);

	Engine::CTransform* terrainTransformCom = dynamic_cast<Engine::CTransform*>(Engine::GetComponent(L"Environment", L"Terrain", L"Com_Transform", Engine::ID_DYNAMIC));
	if (nullptr == terrainTransformCom)
		return _vec3(0.f, 0.f, 0.f);

	return m_calcCom->PickingOnTerrain(g_hWnd, terrainBufferCom, terrainTransformCom);
}

void CPlayer::UpdateAnimMatrices()
{
	D3DXQUATERNION qt;
	_vec3 up = { 0.f, 1.f, 0.f };
	D3DXQuaternionRotationAxis(&qt, &up, D3DXToRadian(m_yRotAngle - 90.f));
	D3DXMatrixRotationQuaternion(&m_yRotMat, &qt);

	D3DXMatrixScaling(&m_yScaleRotMat, 0.01f, 0.01f, 0.01f);

	m_yScaleRotMat *= m_yRotMat;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 device)
{
	CPlayer* instance = new CPlayer(device);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CPlayer::Free()
{
	Engine::CGameObject::Free();
}