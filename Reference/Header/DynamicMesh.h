#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"
#include "HierarchyLoader.h"
#include "AnimCtrl.h"

BEGIN(Engine)

class CTexture;

class ENGINE_DLL CDynamicMesh : public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 device);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh();

public:
	HRESULT Ready(const _tchar* filePath, const _tchar* fileName);
	void Render();
	void Render(LPD3DXEFFECT& effect, _int passIndex);

public:
	void UpdateFrameMatrices(const _float& deltaTime, const _matrix* parentMatrix = nullptr);
	void SetAnimation(const _uint& index, const _float& transitionTime, const _float& endTimeOffset, const _bool& isRoot);
	void PlayAnimation(const _float& deltaTime, const _float& playSpeed = 1.f);
	void ResetAnimation();

	_bool CanCalcMovePos(const char* name, _vec3& outPos, const _matrix* parentMat = nullptr);

public:
	// Getter
	const D3DXFRAME_EX* GetRootFrame() { return (D3DXFRAME_EX*)m_rootFrame; }
	const D3DXFRAME_EX* GetCloneFrame() { return (D3DXFRAME_EX*)m_cloneFrame; }
	const D3DXFRAME_EX* GetFrameByName(const char* frameName);
	const D3DXFRAME_EX* GetCloneFrameByName(const char* frameName);
	_bool IsAnimationSetEnd();

public:
	// Setter
	void SetIsRootMotion(const _bool& isRoot) { m_isRootMotion = isRoot; }
	void SetBoneName(const char* name) { m_boneName = name; }
	void AddToAccMovePos(const _vec3& movePos) { m_accMovePos += movePos; }

	void AddNormalTexture(CTexture* tex) { m_normalTexList.emplace_back(tex); }
private:
	_bool CanCalcBoneMove(const D3DXFRAME_EX* EXFrame, const D3DXFRAME_EX* originFrame, const char* name, _matrix parentMatrix, _matrix combineMatrix, _vec3* out);
	void UpdateFrameMatrices(D3DXFRAME_EX* EXFrame, D3DXFRAME_EX* originFrame, const _matrix* parentMatrix);
	void SetUpFrameMatricesPointer(D3DXFRAME_EX* EXFrame);

private:
	void CloneFrame(D3DXFRAME** cloneFrame, const D3DXFRAME* originFrame);
	void CloneMeshContainer(list<D3DXMESHCONTAINER_EX*>& meshList, const list<D3DXMESHCONTAINER_EX*>& originMeshList);
	void DestroyCloneFrame(D3DXFRAME_EX* EXFrame);

private:
	D3DXFRAME* m_rootFrame;
	D3DXFRAME* m_cloneFrame;
	CHierarchyLoader* m_loader;
	CAnimCtrl* m_animCtrl;
	list<D3DXMESHCONTAINER_EX*> m_meshContainerList;
	vector<CTexture*> m_normalTexList;

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