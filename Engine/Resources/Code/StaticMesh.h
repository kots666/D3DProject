#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 device);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh();

public:
	HRESULT		Ready(const _tchar* filePath, const _tchar* fileName);
	void		Render();

public:
	const _ulong& GetNumVtx() { return m_numVtx; }
	const _ulong& GetStride() { return m_stride; }
	const _vec3* GetVtxPos() { return m_vtxPos; }

private:
	LPD3DXMESH m_originMesh; // ���� �ε� ������ �����ϴ� �޽� �İ�ü
	LPD3DXMESH m_mesh; // �븻 ������ �����Ͽ� ��ȯ��Ų �޽� �İ�ü
	LPD3DXBUFFER m_adjacency; // ������ �̿��� ������ �����ϱ� ���� ù��° �ּҸ� �����Ϸ��� ��� ����
	LPD3DXBUFFER m_subset; // ����� ���� == �ؽ����� ���� == ������ ����
	D3DXMATERIAL* m_mtrl;
	_ulong m_subsetCnt;

	_ulong m_stride;
	_ulong m_numVtx;
	_vec3* m_vtxPos;

	LPDIRECT3DTEXTURE9*	m_textures;

public:
	static CStaticMesh*	Create(LPDIRECT3DDEVICE9 device, const _tchar* filePath, const _tchar* fileName);
	virtual CComponent* Clone() override;
	virtual void Free() override;

};

END
#endif // StaticMesh_h__
