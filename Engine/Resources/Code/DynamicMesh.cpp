#include "DynamicMesh.h"
#include <iostream>

using namespace std;

USING(Engine)

CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 device) :
	CMesh(device), m_loader(nullptr),
	m_rootFrame(nullptr),
	m_animCtrl(nullptr),
	m_boneName(nullptr), m_isRootMotion(false),
	m_accMovePos({ 0.f, 0.f, 0.f }), m_prevPos({ 0.f, 0.f, 0.f })
{
}

CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs) :
	CMesh(rhs),
	m_rootFrame(rhs.m_rootFrame),
	m_loader(rhs.m_loader),
	m_meshContainerList(rhs.m_meshContainerList),
	m_boneName(rhs.m_boneName), m_isRootMotion(rhs.m_isRootMotion),
	m_accMovePos(rhs.m_accMovePos), m_prevPos(rhs.m_prevPos)
{
	m_animCtrl = CAnimCtrl::Create(*rhs.m_animCtrl);
}

CDynamicMesh::~CDynamicMesh()
{
}

HRESULT CDynamicMesh::Ready(const _tchar* filePath, const _tchar* fileName)
{
	_tchar fullPath[MAX_PATH] = L"";

	lstrcpy(fullPath, filePath);
	lstrcat(fullPath, fileName);

	m_loader = CHierarchyLoader::Create(m_device, filePath);
	NULL_CHECK_RETURN(m_loader, E_FAIL);

	LPD3DXANIMATIONCONTROLLER animCtrl = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(fullPath,
		D3DXMESH_MANAGED,
		m_device,
		m_loader,	// HierarchyLoader
		NULL, 
		&m_rootFrame,
		&animCtrl))) // AniCtrl
		return E_FAIL;

	m_animCtrl = CAnimCtrl::Create(animCtrl);
	NULL_CHECK_RETURN(m_animCtrl, E_FAIL);

	SafeRelease(animCtrl);

	_matrix matTemp;

	UpdateFrameMatrices((D3DXFRAME_EX*)m_rootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f)));
	SetUpFrameMatricesPointer((D3DXFRAME_EX*)m_rootFrame);

	return S_OK;
}

void CDynamicMesh::Render()
{
	_bool isToDefault = m_animCtrl->IsAnimationSetChange(m_animIndex, &m_blendTime);
	m_isRootMotion = m_isRoot;

	if (isToDefault)
	{
		m_isBlendTime = true;
		m_accMovePos = m_prevPos = { 0.f, 0.f, 0.f };
	}

	m_animCtrl->PlayAnimation(m_deltaTime);
	UpdateFrameMatrices((D3DXFRAME_EX*)m_rootFrame, &m_parentMat);

	// 렌더 시 해당 애니메이션 프레임 값에 맞는 포지션 값을 적용하기 위한 단계
	for (auto& iter : m_meshContainerList)
	{
		D3DXMESHCONTAINER_EX* EXMeshContainer = iter;

		for (_ulong i = 0; i < EXMeshContainer->numBones; ++i)
		{
			EXMeshContainer->renderingMatrix[i] = EXMeshContainer->frameOffsetMatrix[i]
				* (*EXMeshContainer->frameCombinedMatrix[i]);
		}

		void* srcVertices = nullptr;
		void* dstVertices = nullptr;

		EXMeshContainer->originMesh->LockVertexBuffer(0, &srcVertices);
		EXMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &dstVertices);

		EXMeshContainer->pSkinInfo->UpdateSkinnedMesh(
			EXMeshContainer->renderingMatrix,
			nullptr,
			srcVertices,
			dstVertices);
		// 소프트 웨어 스키닝을 수행하는 함수(스키닝 뿐 아니라 애니메이션 변경 시, 뼈대들의 정점 정보들의 변경도 동시에 수행)
		// 1인자 : 뼈의 최종적인 변환 상태
		// 2인자 : 원래대로 돌리기 위한 역행렬(굳이 안넣어줘도 된다)
		// 3인자 : 변하지 않는 원본 메쉬의 정점 정보
		// 4인자 : 변환된 정보를 담아두고 있는 메쉬의 정점 정보

		for (_ulong i = 0; i < EXMeshContainer->NumMaterials; ++i)
		{
			m_device->SetTexture(0, EXMeshContainer->texture[i]);
			EXMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		EXMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
		EXMeshContainer->originMesh->UnlockVertexBuffer();
	}
}

void CDynamicMesh::UpdateFrameMatrices(const _float& deltaTime, const _matrix * parentMatrix)
{
	if (m_isBlendTime)
	{
		m_blendTime -= deltaTime;

		if (0.0 >= m_blendTime)
		{
			m_isBlendTime = false;
		}
	}

	if (nullptr == parentMatrix)
	{
		_matrix tmp;
		parentMatrix = D3DXMatrixIdentity(&tmp);
	}

	m_parentMat = *parentMatrix;

	//UpdateFrameMatrices((D3DXFRAME_EX*)m_rootFrame, parentMatrix);
}

void CDynamicMesh::SetAnimationSet(const _uint & index, const _bool & isRoot)
{
	m_animIndex = index;
	m_isRoot = isRoot;
}

void CDynamicMesh::PlayAnimation(const _float & deltaTime)
{
	//m_animCtrl->PlayAnimation(deltaTime);
	m_deltaTime = deltaTime;
}

void CDynamicMesh::CalcMovePos(const char * name, _vec3& outPos, const _matrix* parentMat)
{
	if (m_isRootMotion && !m_isBlendTime)
	{
		_matrix initMat;
		D3DXMatrixIdentity(&initMat);

		if (nullptr == parentMat)
		{
			parentMat = &initMat;
		}

		if (CanCalcBoneMove((D3DXFRAME_EX*)m_rootFrame, name, parentMat, &initMat, &outPos))
		{
			m_prevPos = m_accMovePos;
			m_accMovePos = outPos;

			outPos = m_accMovePos - m_prevPos;

			return;
		}
	}

	outPos = { 0.f, 0.f, 0.f };

	return;
}

const D3DXFRAME_EX * CDynamicMesh::GetFrameByName(const char * frameName)
{
	return (D3DXFRAME_EX*)D3DXFrameFind(m_rootFrame, frameName);
}

_bool CDynamicMesh::IsAnimationSetEnd()
{
	return m_animCtrl->IsAnimationSetEnd();
}

_bool CDynamicMesh::CanCalcBoneMove(const D3DXFRAME_EX * EXframe, const char * name, const _matrix * parentMatrix, _matrix * combineMatrix, _vec3 * out)
{
	if (nullptr == EXframe)
		return false;

	*combineMatrix = EXframe->TransformationMatrix * *(parentMatrix);

	if (nullptr != EXframe->Name)
	{
		if (0 == strcmp(EXframe->Name, name))
		{
			*out = _vec3(combineMatrix->m[3][0], combineMatrix->m[3][1], combineMatrix->m[3][2]);
			return true;
		}
	}

	if (nullptr != EXframe->pFrameSibling)
		CanCalcBoneMove((D3DXFRAME_EX*)EXframe->pFrameSibling, name, parentMatrix, combineMatrix, out);

	if (nullptr != EXframe->pFrameFirstChild)
		CanCalcBoneMove((D3DXFRAME_EX*)EXframe->pFrameFirstChild, name, combineMatrix, combineMatrix, out);
}

void CDynamicMesh::UpdateFrameMatrices(D3DXFRAME_EX* EXFrame, const _matrix* parentMatrix)
{
	if (nullptr == EXFrame)
		return;

	if (m_isRootMotion && !m_isBlendTime && nullptr != EXFrame->Name && nullptr != m_boneName)
	{
		if (0 == strcmp(EXFrame->Name, m_boneName))
		{
			EXFrame->TransformationMatrix.m[3][0] = 0;
			EXFrame->TransformationMatrix.m[3][2] = 0;

			EXFrame->combinedTransformationMatrix = EXFrame->TransformationMatrix * (*parentMatrix);
		}
		else
		{
			EXFrame->combinedTransformationMatrix = EXFrame->TransformationMatrix * (*parentMatrix);
		}
	}
	else
	{
		EXFrame->combinedTransformationMatrix = EXFrame->TransformationMatrix * (*parentMatrix);
	}

	if (nullptr != EXFrame->pFrameSibling)
		UpdateFrameMatrices((D3DXFRAME_EX*)EXFrame->pFrameSibling, parentMatrix);

	if (nullptr != EXFrame->pFrameFirstChild)
		UpdateFrameMatrices((D3DXFRAME_EX*)EXFrame->pFrameFirstChild, &EXFrame->combinedTransformationMatrix);

}

void CDynamicMesh::SetUpFrameMatricesPointer(D3DXFRAME_EX * EXFrame)
{
	// 각 본을 순회하면서 combinedMatrix를 meshContainer에 저장하는 작업
	if (nullptr != EXFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_EX* EXMeshContainer = (D3DXMESHCONTAINER_EX*)EXFrame->pMeshContainer;

		for (_ulong i = 0; i < EXMeshContainer->numBones; ++i)
		{
			const char* boneName = EXMeshContainer->pSkinInfo->GetBoneName(i);
			D3DXFRAME_EX* foundFrame = (D3DXFRAME_EX*)D3DXFrameFind(m_rootFrame, boneName);

			EXMeshContainer->frameCombinedMatrix[i] = &foundFrame->combinedTransformationMatrix;
		}
		
		m_meshContainerList.push_back(EXMeshContainer);
	}

	if (nullptr != EXFrame->pFrameSibling)
		SetUpFrameMatricesPointer((D3DXFRAME_EX*)EXFrame->pFrameSibling);

	if (nullptr != EXFrame->pFrameFirstChild)
		SetUpFrameMatricesPointer((D3DXFRAME_EX*)EXFrame->pFrameFirstChild);
}

//void CDynamicMesh::CloneFrame(D3DXFRAME ** dstFrame, const D3DXFRAME * srcFrame)
//{
//	if (nullptr == srcFrame) return;
//
//	if (nullptr == *dstFrame)
//		*dstFrame = new D3DXFRAME_EX(*(D3DXFRAME_EX*)srcFrame);
//
//	if (nullptr != srcFrame->pFrameSibling)
//	{
//		(*dstFrame)->pFrameSibling = new D3DXFRAME_EX(*(D3DXFRAME_EX*)srcFrame->pFrameSibling);
//		CloneFrame(&(*dstFrame)->pFrameSibling, srcFrame->pFrameSibling);
//	}
//
//	if (nullptr != srcFrame->pFrameFirstChild)
//	{
//		(*dstFrame)->pFrameFirstChild = new D3DXFRAME_EX(*(D3DXFRAME_EX*)srcFrame->pFrameFirstChild);
//		CloneFrame(&(*dstFrame)->pFrameFirstChild, srcFrame->pFrameFirstChild);
//	}
//}
//
//void CDynamicMesh::CloneMeshContainer(list<D3DXMESHCONTAINER_EX*>& dstList, const list<D3DXMESHCONTAINER_EX*>& srcList)
//{
//	D3DXMESHCONTAINER_EX* newMeshContainer = nullptr;
//
//	for (auto& meshContainer : srcList)
//	{
//		newMeshContainer = new D3DXMESHCONTAINER_EX(*meshContainer);
//
//		newMeshContainer->frameCombinedMatrix = new _matrix*[meshContainer->numBones];
//
//		for (DWORD i = 0; i < meshContainer->numBones; ++i)
//		{
//			const char* boneName = meshContainer->pSkinInfo->GetBoneName(i);
//			D3DXFRAME_EX* frame = (D3DXFRAME_EX*)D3DXFrameFind(m_cloneRootFrame, boneName);
//			newMeshContainer->frameCombinedMatrix[i] = &frame->combinedTransformationMatrix;
//		}
//
//		dstList.push_back(newMeshContainer);
//	}
//}

//void CDynamicMesh::DestroyCloneFrame(D3DXFRAME_EX * EXFrame)
//{
//	if (nullptr == EXFrame) return;
//	
//	if (nullptr != EXFrame->pFrameSibling)
//		DestroyCloneFrame((D3DXFRAME_EX*)EXFrame->pFrameSibling);
//	if (nullptr != EXFrame->pFrameFirstChild)
//		DestroyCloneFrame((D3DXFRAME_EX*)EXFrame->pFrameFirstChild);
//
//	SafeDelete(EXFrame);
//	EXFrame = nullptr;
//}

CDynamicMesh* CDynamicMesh::Create(LPDIRECT3DDEVICE9 device, const _tchar* filePath, const _tchar* fileName)
{
	CDynamicMesh* instance = new CDynamicMesh(device);

	if (FAILED(instance->Ready(filePath, fileName)))
		SafeRelease(instance);

	return instance;
}

CComponent* CDynamicMesh::Clone()
{
	return new CDynamicMesh(*this);
}

void CDynamicMesh::Free()
{
	CMesh::Free();

	SafeRelease(m_animCtrl);

	if (!m_isClone)
	{
		m_loader->DestroyFrame(m_rootFrame);
		SafeRelease(m_loader);
	}
	//else
	//{
	//	DestroyCloneFrame((D3DXFRAME_EX*)m_cloneRootFrame);

	//	for (auto& meshCon : m_meshContainerList)
	//	{
	//		SafeDeleteArray(meshCon->frameCombinedMatrix);
	//		SafeDelete(meshCon);
	//	}
	//}
	m_meshContainerList.clear();
}

