#include "DynamicMesh.h"

USING(Engine)

CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 device) :
	CMesh(device), m_loader(nullptr),
	m_rootFrame(nullptr), m_animCtrl(nullptr)
{

}

CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs) :
	CMesh(rhs), m_rootFrame(rhs.m_rootFrame),
	m_loader(rhs.m_loader), m_meshContainerList(rhs.m_meshContainerList)
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

	UpdateFrameMatrices((D3DXFRAME_EX*)m_rootFrame, D3DXMatrixIdentity(&matTemp));
	//UpdateFrameMatrices((D3DXFRAME_EX*)m_rootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f)));
	SetUpFrameMatricesPointer((D3DXFRAME_EX*)m_rootFrame);

	return S_OK;
}

void CDynamicMesh::Render()
{
	// ���� �� �ش� �ִϸ��̼� ������ ���� �´� ������ ���� �����ϱ� ���� �ܰ�
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
		// ����Ʈ ���� ��Ű���� �����ϴ� �Լ�(��Ű�� �� �ƴ϶� �ִϸ��̼� ���� ��, ������� ���� �������� ���浵 ���ÿ� ����)
		// 1���� : ���� �������� ��ȯ ����
		// 2���� : ������� ������ ���� �����(���� �ȳ־��൵ �ȴ�)
		// 3���� : ������ �ʴ� ���� �޽��� ���� ����
		// 4���� : ��ȯ�� ������ ��Ƶΰ� �ִ� �޽��� ���� ����

		for (_ulong i = 0; i < EXMeshContainer->NumMaterials; ++i)
		{
			m_device->SetTexture(0, EXMeshContainer->texture[i]);
			EXMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		EXMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
		EXMeshContainer->originMesh->UnlockVertexBuffer();
	}
}

const D3DXFRAME_EX * CDynamicMesh::GetFrameByName(const char * frameName)
{
	return (D3DXFRAME_EX*)D3DXFrameFind(m_rootFrame, frameName);
}

_bool CDynamicMesh::IsAnimationSetEnd()
{
	return m_animCtrl->IsAnimationSetEnd();
}

void CDynamicMesh::SetAnimationSet(const _uint & index)
{
	m_animCtrl->SetAnimationSet(index);
}

void CDynamicMesh::PlayAnimation(const _float & deltaTime)
{
	m_animCtrl->PlayAnimation(deltaTime);

	_matrix matTmp;
	//UpdateFrameMatrices((D3DXFRAME_EX*)m_rootFrame, D3DXMatrixRotationY(&matTmp, D3DXToRadian(180.f)));
	UpdateFrameMatrices((D3DXFRAME_EX*)m_rootFrame, D3DXMatrixIdentity(&matTmp));
}

void CDynamicMesh::UpdateFrameMatrices(D3DXFRAME_EX* EXFrame, const _matrix* parentMatrix)
{
	if (nullptr == EXFrame)
		return;

	EXFrame->combinedTransformationMatrix = EXFrame->TransformationMatrix * (*parentMatrix);

	if (nullptr != EXFrame->pFrameSibling)
		UpdateFrameMatrices((D3DXFRAME_EX*)EXFrame->pFrameSibling, parentMatrix);

	if (nullptr != EXFrame->pFrameFirstChild)
		UpdateFrameMatrices((D3DXFRAME_EX*)EXFrame->pFrameFirstChild, &EXFrame->combinedTransformationMatrix);

}

void CDynamicMesh::SetUpFrameMatricesPointer(D3DXFRAME_EX * EXFrame)
{
	// �� ���� ��ȸ�ϸ鼭 combinedMatrix�� meshContainer�� �����ϴ� �۾�
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
	m_meshContainerList.clear();
}

