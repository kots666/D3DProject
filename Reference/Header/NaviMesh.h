#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "Base.h"
#include "Mesh.h"
#include "NaviCell.h"

BEGIN(Engine)

class ENGINE_DLL CNaviMesh : public CMesh
{
private:
	explicit CNaviMesh(LPDIRECT3DDEVICE9 device);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh();

public:
	HRESULT Ready();
	void Render();

public:
	_vec3 MoveOnNaviMesh(const _vec3* targetPos, const _vec3* targetDir);
	HRESULT LinkCell();

private:
	void SetNaviIndex(const _ulong& index) { m_index = index; }

private:
	vector<CNaviCell*> m_vecCell;
	_ulong m_index;

public:
	static CNaviMesh* Create(LPDIRECT3DDEVICE9 device);
	virtual CComponent* Clone();
	virtual void Free();

};

END

#endif // NaviMesh_h__
