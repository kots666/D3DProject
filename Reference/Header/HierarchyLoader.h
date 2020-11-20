#ifndef HierarchyLoader_h__
#define HierarchyLoader_h__

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CHierarchyLoader : public ID3DXAllocateHierarchy
{
private:
	explicit CHierarchyLoader(LPDIRECT3DDEVICE9 device, const _tchar* path);
	virtual ~CHierarchyLoader();

public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);

	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name,
									CONST D3DXMESHDATA *pMeshData,
									CONST D3DXMATERIAL *pMaterials,
									CONST D3DXEFFECTINSTANCE *pEffectInstances,
									DWORD NumMaterials,
									CONST DWORD *pAdjacency,
									LPD3DXSKININFO pSkinInfo,
									LPD3DXMESHCONTAINER *ppNewMeshContainer);
		
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);

private:
	void AllocateName(char** outName, const char* inFrameName);

private:
	LPDIRECT3DDEVICE9 m_device;
	const _tchar* m_path;

public:
	static CHierarchyLoader* Create(LPDIRECT3DDEVICE9 device, const _tchar* path);
	_ulong Release();

};

END
#endif // HierarchyLoader_h__
