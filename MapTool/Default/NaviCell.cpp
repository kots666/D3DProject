#include "stdafx.h"
#include "NaviCell.h"
#include "TriCol.h"

CNaviCell::CNaviCell(LPDIRECT3DDEVICE9 device, _vec3 pos1, _vec3 pos2, _vec3 pos3) :
	CGameObject(device)
{
	m_pos[0] = pos1;
	m_pos[1] = pos2;
	m_pos[2] = pos3;
}

CNaviCell::~CNaviCell()
{
}

HRESULT CNaviCell::Ready()
{
	if (FAILED(AddComponent()))
		return E_FAIL;
	return S_OK;
}

_int CNaviCell::Update(const _float & deltaTime)
{
	_vec3 tmpPos = { 0.f, 0.f, 0.f };

	for (int i = 0; i < 3; ++i)
		tmpPos += m_pos[i];

	tmpPos /= 3.f;

	m_transCom->SetPos(tmpPos.x, tmpPos.y, tmpPos.z);
	CGameObject::Update(deltaTime);

	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CNaviCell::Render()
{
	m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_bufferCom->Render();

	m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT CNaviCell::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// Mesh
	component = m_bufferCom = dynamic_cast<Engine::CTriCol*>(Engine::CloneResource(Engine::RESOURCE_STATIC, L"Buffer_TriCol"));
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

	// Collider
	/*component = m_colliderCom = Engine::CCollider::Create(m_device, m_meshCom->GetVtxPos(), m_meshCom->GetNumVtx(), m_meshCom->GetStride());
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Collider", component);*/

	return S_OK;
}

CNaviCell * CNaviCell::Create(LPDIRECT3DDEVICE9 device, _vec3 pos1, _vec3 pos2, _vec3 pos3)
{
	CNaviCell* instance = new CNaviCell(device, pos1, pos2, pos3);
	if (FAILED(instance->Ready()))
		SafeRelease(instance);
	return instance;
}

void CNaviCell::Free()
{
	CGameObject::Free();
}
