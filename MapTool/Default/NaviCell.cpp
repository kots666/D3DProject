#include "stdafx.h"
#include "NaviCell.h"
#include "TriCol.h"

CNaviCell::CNaviCell(LPDIRECT3DDEVICE9 device, _vec3 pos1, _vec3 pos2, _vec3 pos3) :
	CGameObject(device), m_line(nullptr)
{
	m_pos[0] = pos1;
	m_pos[1] = pos2;
	m_pos[2] = pos3;

	m_line = Engine::CGraphicDevice::GetInstance()->GetLine();
	SafeAddRef(m_line);
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

	_vec3 vertices[] =
	{
	m_pos[0],
	m_pos[1],
	m_pos[2],
	m_pos[0]
	};

	_matrix viewMat, projMat;
	m_device->GetTransform(D3DTS_VIEW, &viewMat);
	m_device->GetTransform(D3DTS_PROJECTION, &projMat);

	viewMat *= projMat;

	m_line->SetWidth(5.f);
	m_line->Begin();
	m_line->DrawTransform(vertices, 4, &viewMat, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	m_line->End();

	m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

_vec3 * CNaviCell::GetPos(const _int & vertexIndex)
{
	if (0 > vertexIndex || vertexIndex > 2) return nullptr;

	return &m_pos[vertexIndex];
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
	SafeRelease(m_line);

	CGameObject::Free();
}
