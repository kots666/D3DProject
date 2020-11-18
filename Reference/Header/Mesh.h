#ifndef Mesh_h__
#define Mesh_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CMesh: public CResources
{
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 device);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh();

protected:
	_bool m_isClone;

public:
	virtual CComponent* Clone() PURE;
	virtual void Free();

};

END
#endif // Mesh_h__
