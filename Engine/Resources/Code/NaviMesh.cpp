#include "NaviMesh.h"

USING(Engine)

CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 device) :
	CMesh(device),
	m_index(0)
{
	
}

CNaviMesh::CNaviMesh(const CNaviMesh& rhs) :
	CMesh(rhs),
	m_vecCell(rhs.m_vecCell),
	m_index(rhs.m_index)
{
	for (auto& iter : m_vecCell)
		SafeAddRef(iter);
}

CNaviMesh::~CNaviMesh()
{

}

HRESULT CNaviMesh::Ready()
{
	m_vecCell.reserve(4);

	CNaviCell* cell = nullptr;

	// 0번 
	cell = CNaviCell::Create(m_device, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f), &_vec3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN(cell, E_FAIL);
	m_vecCell.push_back(cell);

	// 1번 
	cell = CNaviCell::Create(m_device, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f));
	NULL_CHECK_RETURN(cell, E_FAIL);
	m_vecCell.push_back(cell);

	// 2번 
	cell = CNaviCell::Create(m_device, m_vecCell.size(), &_vec3(0.f, 0.f, 4.f), &_vec3(2.f, 0.f, 2.f), &_vec3(0.f, 0.f, 2.f));
	NULL_CHECK_RETURN(cell, E_FAIL);
	m_vecCell.push_back(cell);

	// 3번 
	cell = CNaviCell::Create(m_device, m_vecCell.size(), &_vec3(2.f, 0.f, 2.f), &_vec3(4.f, 0.f, 0.f), &_vec3(2.f, 0.f, 0.f));
	NULL_CHECK_RETURN(cell, E_FAIL);
	m_vecCell.push_back(cell);

	FAILED_CHECK_RETURN(LinkCell(), E_FAIL);

	return S_OK;
}

void CNaviMesh::Render()
{
	for (auto& iter : m_vecCell)
		iter->Render();
}

_vec3 CNaviMesh::MoveOnNaviMesh(const _vec3 * targetPos, const _vec3 * targetDir)
{
	_vec3 endPos = *targetPos + *targetDir;

	if (CNaviCell::MOVESTATE_MOVE == m_vecCell[m_index]->CompareCell(&endPos, &m_index))
		return endPos;

	else if (CNaviCell::MOVESTATE_STOP == m_vecCell[m_index]->CompareCell(&endPos, &m_index))
		return *targetPos;
}

HRESULT CNaviMesh::LinkCell()
{
	for (_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		for (_ulong j = 0; j < m_vecCell.size(); ++j)
		{
			if(i == j)
				continue;

			if (nullptr == m_vecCell[i]->GetNeighbor(CNaviCell::NEIGHBOR_AB) &&
				true == m_vecCell[j]->ComparePoint(m_vecCell[i]->GetPoint(CNaviCell::POINT_A),
													m_vecCell[i]->GetPoint(CNaviCell::POINT_B),	
													m_vecCell[i]))
			{
				m_vecCell[i]->SetNeighbor(CNaviCell::NEIGHBOR_AB, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->GetNeighbor(CNaviCell::NEIGHBOR_BC) &&
				true == m_vecCell[j]->ComparePoint(m_vecCell[i]->GetPoint(CNaviCell::POINT_B),
													m_vecCell[i]->GetPoint(CNaviCell::POINT_C),
													m_vecCell[i]))
			{
				m_vecCell[i]->SetNeighbor(CNaviCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->GetNeighbor(CNaviCell::NEIGHBOR_CA) &&
				true == m_vecCell[j]->ComparePoint(m_vecCell[i]->GetPoint(CNaviCell::POINT_C),
													m_vecCell[i]->GetPoint(CNaviCell::POINT_A),
													m_vecCell[i]))
			{
				m_vecCell[i]->SetNeighbor(CNaviCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}
		}
	}
	
	return S_OK;
}

CNaviMesh* CNaviMesh::Create(LPDIRECT3DDEVICE9 device)
{
	CNaviMesh* instance = new CNaviMesh(device);

	if (FAILED(instance->Ready()))
		SafeRelease(instance);

	return instance;
}

CComponent* CNaviMesh::Clone()
{
	return new CNaviMesh(*this);
}

void CNaviMesh::Free()
{
	for_each(m_vecCell.begin(), m_vecCell.end(), CDeleteObj());
	m_vecCell.clear();

	CMesh::Free();
}