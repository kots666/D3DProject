#ifndef Cell_h__
#define Cell_h__

#include "Base.h"
#include "NaviLine.h"

BEGIN(Engine)

class ENGINE_DLL CNaviCell : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE  { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };
	enum MOVESTATE { MOVESTATE_MOVE, MOVESTATE_STOP };

private:
	explicit CNaviCell(LPDIRECT3DDEVICE9 device);
	virtual ~CNaviCell();

public:
	HRESULT Ready(const _ulong& index, const _vec3* pointA, const _vec3* pointB, const _vec3* pointC);
	void Render();

public:
	_bool ComparePoint(const _vec3* firstPoint, const _vec3* secondPoint, CNaviCell* cell);
	MOVESTATE CompareCell(const _vec3* endPos, _ulong* cellIndex, _vec3* normalVec);

public:
	// Getter
	const _vec3* GetPoint(POINT type) const { return &m_point[type]; }
	CNaviCell* GetNeighbor(NEIGHBOR type) const { return m_neighbor[type]; }
	const _ulong* GetIndex() { return &m_index; }

public:
	// Setter
	void SetNeighbor(NEIGHBOR type, CNaviCell* neighbor) { m_neighbor[type] = neighbor; }

private:
	_vec3 m_point[POINT_END];
	CNaviCell* m_neighbor[NEIGHBOR_END];
	CNaviLine* m_line[LINE_END];
	LPDIRECT3DDEVICE9 m_device;
	LPD3DXLINE m_lineCom;
	_ulong m_index = 0;

public:
	static CNaviCell* Create(LPDIRECT3DDEVICE9 device, const _ulong& index, const _vec3* pointA, const _vec3* pointB, const _vec3* pointC);
	virtual void Free();

};

END

#endif
