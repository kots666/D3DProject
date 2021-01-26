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
	_bool IsAnimationSetChange(const _uint& index, const _double& transitionTime, const _double& endTimeOffset, _double* blendTime);
	void PlayAnimation(const _float& deltaTime, const _float& playSpeed);

public:
	LPD3DXANIMATIONCONTROLLER GetAnimCtrl() { return m_animCtrl; }
	_bool IsAnimationSetEnd();
	void ResetAnimation();

public:
	_uint GetAnimIdx() const { return m_oldAnimIdx; }
	_float GetAccTime() const { return m_accTime; }

private:
	LPD3DXANIMATIONCONTROLLER m_animCtrl;
	_uint m_currentTrack;
	_uint m_newTrack;

	_float m_accTime;
	_uint m_oldAnimIdx;

	_double m_period;
	_double m_transitionTime;
	_double m_endTimeOffset;

public:
	static CAnimCtrl* Create(LPD3DXANIMATIONCONTROLLER animCtrl);
	static CAnimCtrl* Create(const CAnimCtrl& rhs);
	virtual void Free();

};

END

#endif // AnimCtrl_h__
