#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class CTexture;

class ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 device);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh();

public:
	HRESULT Ready(const _tchar* filePath, const _tchar* fileName);
	void Render();
	void Render(LPD3DXEFFECT& effect);
	void EffectMeshRender();

public:
	void AddNormalTexture(CTexture* tex) { m_normalTex = tex; }

public:
	const _ulong& GetNumVtx() { return m_numVtx; }
	const _ulong& GetStride() { return m_stride; }
	const _vec3* GetVtxPos() { return m_vtxPos; }

private:
	LPD3DXMESH m_originMesh; // 최초 로드 시점에 생성하는 메쉬 컴객체
	LPD3DXMESH m_mesh; // 노말 정보를 삽입하여 변환시킨 메쉬 컴객체
	LPD3DXBUFFER m_adjacency; // 인접한 이웃의 정보를 접근하기 위해 첫번째 주소를 보관하려는 멤버 변수
	LPD3DXBUFFER m_subset; // 서브셋 개수 == 텍스쳐의 개수 == 재질의 개수
	D3DXMATERIAL* m_mtrl;
	_ulong m_subsetCnt;

	_ulong m_stride;
	_ulong m_numVtx;
	_vec3* m_vtxPos;

	LPDIRECT3DTEXTURE9*	m_textures;

	CTexture* m_normalTex;

public:
	static CStaticMesh*	Create(LPDIRECT3DDEVICE9 device, const _tchar* filePath, const _tchar* fileName);
	virtual CComponent* Clone() override;
	virtual void Free() override;

};

END
#endif // StaticMesh_h__
