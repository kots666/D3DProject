#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"
#include "HierarchyLoader.h"

BEGIN(Engine)

class ENGINE_DLL CDynamicMesh : public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 device);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh();

public:
	HRESULT Ready(const _tchar* filePath, const _tchar* fileName);
	void Render();

private:
	void UpdateFrameMatrices(D3DXFRAME_EX* frame, const _matrix* parentMatrix);
	void SetUpFrameMatricesPointer(D3DXFRAME_EX* frame);

private:
	D3DXFRAME* m_rootFrame;
	CHierarchyLoader* m_loader;
	list<D3DXMESHCONTAINER_EX*> m_meshContainerList;

public:
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 device, const _tchar* filePath, const _tchar* fileName);
	virtual CComponent* Clone() override;
	virtual void Free();
};

END
#endif // DynamicMesh_h__


//typedef struct _D3DXFRAME
//{
//	LPSTR                   Name;
//	D3DXMATRIX              TransformationMatrix;
//
//	LPD3DXMESHCONTAINER     pMeshContainer;
//
//	struct _D3DXFRAME       *pFrameSibling;
//	struct _D3DXFRAME       *pFrameFirstChild;
//
//} D3DXFRAME, *LPD3DXFRAME;
//
//
/*
typedef struct _D3DXMESHCONTAINER
{
	LPSTR                   Name;
	D3DXMESHDATA            MeshData;
	LPD3DXMATERIAL          pMaterials;
	LPD3DXEFFECTINSTANCE    pEffects;
	DWORD                   NumMaterials;
	DWORD                  *pAdjacency;
	LPD3DXSKININFO          pSkinInfo;

	struct _D3DXMESHCONTAINER *pNextMeshContainer;

} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;*/