#include "NaviCell.h"	

USING(Engine)

CNaviCell::CNaviCell(LPDIRECT3DDEVICE9 device) :
	m_lineCom(nullptr),
	m_device(device)
{

	SafeAddRef(m_device);

	ZeroMemory(m_line, sizeof(CNaviLine*) * LINE_END);
	ZeroMemory(m_neighbor, sizeof(CNaviCell*) * NEIGHBOR_END);
}

CNaviCell::~CNaviCell()
{

}

HRESULT CNaviCell::Ready(const _ulong& index, const _vec3* pointA, const _vec3* pointB, const _vec3* pointC)
{
	m_index = index;

	m_point[POINT_A] = *pointA;
	m_point[POINT_B] = *pointB;
	m_point[POINT_C] = *pointC;

	m_line[LINE_AB] = CNaviLine::Create(&_vec2(m_point[POINT_A].x, m_point[POINT_A].z),
									 &_vec2(m_point[POINT_B].x, m_point[POINT_B].z));

	m_line[LINE_BC] = CNaviLine::Create(&_vec2(m_point[POINT_B].x, m_point[POINT_B].z),
									 &_vec2(m_point[POINT_C].x, m_point[POINT_C].z));

	m_line[LINE_CA] = CNaviLine::Create(&_vec2(m_point[POINT_C].x, m_point[POINT_C].z),
									 &_vec2(m_point[POINT_A].x, m_point[POINT_A].z));

#ifdef _DEBUG
	if (FAILED(D3DXCreateLine(m_device, &m_lineCom)))
		return S_OK;
#endif
	return S_OK;
}

void CNaviCell::Render()
{
#ifdef _DEBUG
	_vec3 point[4];

	point[0] = m_point[POINT_A];
	point[1] = m_point[POINT_B];
	point[2] = m_point[POINT_C];
	point[3] = m_point[POINT_A];

	_matrix matView, matProj;

	m_device->GetTransform(D3DTS_VIEW, &matView);
	m_device->GetTransform(D3DTS_PROJECTION, &matProj);

	for (_ulong i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&point[i], &point[i], &matView);
		
		if (point[i].z <= 0.1f)
			point[i].z = 0.1f;

		D3DXVec3TransformCoord(&point[i], &point[i], &matProj);
	}
	
	m_lineCom->SetWidth(3.f);	// 라인의 굵기 결정

	m_device->EndScene();
	m_device->BeginScene();

	m_lineCom->Begin();
	
	_matrix matTemp;

	m_lineCom->DrawTransform(point, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	m_lineCom->End();
#endif
}

_bool CNaviCell::ComparePoint(const _vec3* firstPoint, const _vec3* secondPoint, CNaviCell* cell)
{
	if (*firstPoint == m_point[POINT_A])
	{
		if (*secondPoint == m_point[POINT_B])
		{
			m_neighbor[NEIGHBOR_AB] = cell;
			return true;
		}

		if (*secondPoint == m_point[POINT_C])
		{
			m_neighbor[NEIGHBOR_CA] = cell;
			return true;
		}
	}

	if (*firstPoint == m_point[POINT_B])
	{
		if (*secondPoint == m_point[POINT_A])
		{
			m_neighbor[NEIGHBOR_AB] = cell;
			return true;
		}

		if (*secondPoint == m_point[POINT_C])
		{
			m_neighbor[NEIGHBOR_BC] = cell;
			return true;
		}
	}

	if (*firstPoint == m_point[POINT_C])
	{
		if (*secondPoint == m_point[POINT_B])
		{
			m_neighbor[NEIGHBOR_BC] = cell;
			return true;
		}

		if (*secondPoint == m_point[POINT_A])
		{
			m_neighbor[NEIGHBOR_CA] = cell;
			return true;
		}
	}

	return false;
}

CNaviCell::MOVESTATE CNaviCell::CompareCell(const _vec3 * endPos, _ulong * cellIndex, _vec3* normalVec)
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		if (CNaviLine::COMPARE_LEFT == m_line[i]->Compare(&_vec2(endPos->x, endPos->z)))
		{
			if (nullptr == m_neighbor[i])
			{
				*normalVec = m_line[i]->GetNormal();
				return CNaviCell::MOVESTATE_STOP;
			}
			else
			{
				*cellIndex = *m_neighbor[i]->GetIndex();
				return CNaviCell::MOVESTATE_MOVE;
			}
		}
	}
	return CNaviCell::MOVESTATE_MOVE;
}

CNaviCell* CNaviCell::Create(LPDIRECT3DDEVICE9 device, const _ulong& index, const _vec3* pointA, const _vec3* pointB, const _vec3* pointC)
{
	CNaviCell* instance = new CNaviCell(device);

	if (FAILED(instance->Ready(index, pointA, pointB, pointC)))
		SafeRelease(instance);

	return instance;
}

void CNaviCell::Free()
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		SafeRelease(m_line[i]);
	}
	SafeRelease(m_lineCom);
	SafeRelease(m_device);
}