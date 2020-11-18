#ifndef Component_h__
#define Component_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent(void);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent(void);

public:
	virtual _int Update(const _float& deltaTime);

	virtual CComponent* Clone()PURE;
	virtual void Free()PURE;
};

END

#endif