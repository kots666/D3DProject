#include "NaviLine.h"

USING(Engine)

CNaviLine::CNaviLine()
{

}

CNaviLine::~CNaviLine()
{

}

HRESULT CNaviLine::Ready(const _vec2* pointA, const _vec2* pointB)
{
	m_point[POINT_START] = *pointA;
	m_point[POINT_FINISH] = *pointB;

	m_direction = m_point[POINT_FINISH] - m_point[POINT_START];

	m_normal = _vec2(m_direction.y * -1.f, m_direction.x);
	D3DXVec2Normalize(&m_normal, &m_normal);

	return S_OK;
}

CNaviLine::COMPARE CNaviLine::Compare(const _vec2* pos)
{
	_vec2 dst = *pos - m_point[POINT_START];

	_float dot = D3DXVec2Dot(D3DXVec2Normalize(&dst, &dst), &m_normal);

	if (0.f <= dot)
		return COMPARE_LEFT;
	else
		return COMPARE_RIGHT;
}

CNaviLine* CNaviLine::Create(const _vec2* pointA, const _vec2* pointB)
{
	CNaviLine* instance = new CNaviLine;

	if (FAILED(instance->Ready(pointA, pointB)))
		SafeRelease(instance);

	return instance;
}

void Engine::CNaviLine::Free()
{

}

