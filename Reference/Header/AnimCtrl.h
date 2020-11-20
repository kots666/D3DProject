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

private:
	LPD3DXANIMATIONCONTROLLER m_animCtrl;
	_uint m_currentTrack;

public:
	static CAnimCtrl* Create(LPD3DXANIMATIONCONTROLLER animCtrl);
	static CAnimCtrl* Create(const CAnimCtrl& rhs);
	virtual void Free();

};

END

#endif // AnimCtrl_h__
