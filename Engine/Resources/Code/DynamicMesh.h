#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"
#include "HierarchyLoader.h"
#include "AnimCtrl.h"

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

public:
	void UpdateFrameMatrices(const _float& deltaTime, const _matrix* parentMatrix = nullptr);
	void SetAnimationSet(const _uint& index, const _bool& isRoot = true);
	void PlayAnimation(const _float& deltaTime);
	
	void CalcMovePos(const char* name, _vec3& outPos, const _matrix* parentMat = nullptr);

public:
	// Getter
	const D3DXFRAME_EX* GetFrameByName(const char* frameName);
	_bool IsAnimationSetEnd();

public:
	// Setter
	void SetIsRootMotion(const _bool& isRoot) { m_isRootMotion = isRoot; }
	void SetBoneName(const char* name) { m_boneName = name; }
	void AddToAccMovePos(const _vec3& movePos) { m_accMovePos += movePos; }
	
private:
	_bool CanCalcBoneMove(const D3DXFRAME_EX* EXframe, const char* name, const _matrix* parentMatrix, _matrix* combineMatrix, _vec3* out);
	void UpdateFrameMatrices(D3DXFRAME_EX* EXframe, const _matrix* parentMatrix);
	void SetInitFrameMatrices(D3DXFRAME_EX* EXframe, const _matrix* parentMatrix);
	void SetUpFrameMatricesPointer(D3DXFRAME_EX* EXframe);

private:
	D3DXFRAME* m_rootFrame;
	CHierarchyLoader* m_loader;
	CAnimCtrl* m_animCtrl;
	list<D3DXMESHCONTAINER_EX*> m_meshContainerList;

	const char* m_boneName;
	_bool m_isRootMotion;
	_bool m_isBlendTime;
	_double m_blendTime;
	_vec3 m_accMovePos;
	_vec3 m_prevPos;

public:
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 device, const _tchar* filePath, const _tchar* fileName);
	virtual CComponent* Clone() override;
	virtual void Free();
};

END

#endif


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