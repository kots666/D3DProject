#ifndef Line_h__
#define Line_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CNaviLine : public CBase
{
public:
	enum POINT { POINT_START, POINT_FINISH, POINT_END };
	enum COMPARE { COMPARE_LEFT, COMPARE_RIGHT };

private:
	explicit CNaviLine();
	virtual ~CNaviLine();

public:
	HRESULT Ready(const _vec2* pointA, const _vec2* pointB);

public:
	COMPARE Compare(const _vec2* pos);

	_vec3 GetNormal() { return _vec3(m_normal.x, 0.f, m_normal.y); }

private:
	_vec2 m_point[POINT_END];
	_vec2 m_direction;
	_vec2 m_normal;

public:
	static CNaviLine* Create(const _vec2* pointA, const _vec2* pointB);
	virtual void Free();
};

END

#endif