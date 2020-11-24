#ifndef Optimization_h__
#define Optimization_h__

#include "Component.h"
#include "Frustum.h"

BEGIN(Engine)

class ENGINE_DLL COptimization : public CComponent
{
private:
	explicit COptimization(LPDIRECT3DDEVICE9 device);
	explicit COptimization(const COptimization& rhs);
	virtual ~COptimization();

public:
	HRESULT Ready();
	_bool IsInFrustumForObject(const _vec3* worldPos, const _float& radius);

private:
	CFrustum* m_frustum;
	LPDIRECT3DDEVICE9 m_device;

public:
	static COptimization* Create(LPDIRECT3DDEVICE9 device);
	virtual CComponent* Clone();
	virtual void Free();
};

END
#endif // Optimization_h__
