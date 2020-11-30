#ifndef AnimCtrl_h__
#define AnimCtrl_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAnimCtrl : public CBase
{
private:
	explicit CAnimCtrl(LPD3DXANIMATIONCONTROLLER animCtrl);
	explicit CAnimCtrl(const CAnimCtrl& rhs);
	virtual ~CAnimCtrl();

public:
	HRESULT Ready();
	_bool IsAnimationSetChange(const _uint& index, _double* blendTime);
	void PlayAnimation(const _float& deltaTime);

public:
	LPD3DXANIMATIONCONTROLLER GetAnimCtrl() { return m_animCtrl; }
	_bool IsAnimationSetEnd();

private:
	LPD3DXANIMATIONCONTROLLER m_animCtrl;
	_uint m_currentTrack;
	_uint m_newTrack;

	_float m_accTime;
	_uint m_oldAnimIdx;

	_double m_period;
	_double m_curTransitionTime;

public:
	static CAnimCtrl* Create(LPD3DXANIMATIONCONTROLLER animCtrl);
	static CAnimCtrl* Create(const CAnimCtrl& rhs);
	virtual void Free();

};

END

#endif // AnimCtrl_h__
