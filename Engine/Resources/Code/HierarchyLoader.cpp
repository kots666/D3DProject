#include "HierarchyLoader.h"

USING(Engine)

CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 device, const _tchar* path) :
	m_device(device), m_path(path)
{
	SafeAddRef(m_device);
}

CHierarchyLoader::~CHierarchyLoader()
{

}

STDMETHODIMP CHierarchyLoader::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	D3DXFRAME_EX* EXFrame = new D3DXFRAME_EX;
	ZeroMemory(EXFrame, sizeof(D3DXFRAME_EX));

	AllocateName(&EXFrame->Name, Name);

	// ó������ �׵���ķ� �ʱ�ȭ�����ش�.
	EXFrame->combinedTransformationMatrix = new _matrix;
	*EXFrame->combinedTransformationMatrix = *D3DXMatrixIdentity(&EXFrame->TransformationMatrix);

	*ppNewFrame = EXFrame;

	return S_OK;
}

STDMETHODIMP CHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name, 
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials, 
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials, 
	CONST DWORD *pAdjacency, 
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	// ���ο� �޽������̳� ����
	D3DXMESHCONTAINER_EX* EXMeshContainer = new D3DXMESHCONTAINER_EX;
	ZeroMemory(EXMeshContainer, sizeof(D3DXMESHCONTAINER_EX));

	// 1. �̸��� �Ҵ����ش�.
	AllocateName(&EXMeshContainer->Name, Name);

	EXMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	// �޽��������� �޽��� ���´�.
	LPD3DXMESH mesh = pMeshData->pMesh;

	// ���� �޽��� ���� �������� ������ ��ȯ
	_ulong numFaces = mesh->GetNumFaces();

	// ���� �������� �����ϱ� ���� �迭�� �Ҵ�
	EXMeshContainer->pAdjacency = new _ulong[numFaces * 3];
	memcpy(EXMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * numFaces * 3);

	_ulong meshFVF = mesh->GetFVF();

	// �޽��� �븻�� ���� ��� ��ǻƮ �븻 ���� �� �޽������̳ʿ� ����
	if (!(meshFVF & D3DFVF_NORMAL))
	{
		mesh->CloneMeshFVF(mesh->GetOptions(), meshFVF | D3DFVF_NORMAL, m_device, &EXMeshContainer->MeshData.pMesh);
		D3DXComputeNormals(EXMeshContainer->MeshData.pMesh, EXMeshContainer->pAdjacency);
	}
	else
	{
		mesh->CloneMeshFVF(mesh->GetOptions(), meshFVF, m_device, &EXMeshContainer->MeshData.pMesh);
	}

	// �޽��� ���͸���� �ؽ��ĸ� �������� �۾�
	EXMeshContainer->NumMaterials = ((NumMaterials == 0) ? 1 : NumMaterials);

	EXMeshContainer->pMaterials = new D3DXMATERIAL[EXMeshContainer->NumMaterials];
	ZeroMemory(EXMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * EXMeshContainer->NumMaterials);

	EXMeshContainer->texture = new LPDIRECT3DTEXTURE9[EXMeshContainer->NumMaterials];
	ZeroMemory(EXMeshContainer->texture, sizeof(LPDIRECT3DTEXTURE9) * EXMeshContainer->NumMaterials);

	if (0 != NumMaterials)
	{
		memcpy(EXMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * EXMeshContainer->NumMaterials);

		// �޽��� ���͸����� �ε��ϴ� �۾�
		for (_ulong i = 0; i < EXMeshContainer->NumMaterials; ++i)
		{
			_tchar	szFullPath[256] = L"";
			_tchar	szFileName[256] = L"";

			lstrcpy(szFullPath, m_path);

			// �ؽ������ϸ�(�ƽ�Ű�ڵ�)�� ��Ƽ����Ʈ �̸����� �ٲٴ� ����
			MultiByteToWideChar(
				CP_ACP,
				0,
				EXMeshContainer->pMaterials[i].pTextureFilename,
				strlen(EXMeshContainer->pMaterials[i].pTextureFilename),
				szFileName,
				256);

			lstrcat(szFullPath, szFileName);

			if (FAILED(D3DXCreateTextureFromFile(m_device, szFullPath, &EXMeshContainer->texture[i])))
			{
				MSG_BOX("DynamicMesh`s Texture Create Failed");
				return E_FAIL;
			}
		}
	}
	// �޽��� ���͸����� 0�� ��� ������� ���ؼ� �ϴ� �ε� �ܰ迡�� ������ ���� �ʰ� �ϱ� ���� ����
	else
	{
		EXMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		EXMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		EXMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		EXMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		EXMeshContainer->pMaterials[0].MatD3D.Power = 0.f;

		EXMeshContainer->texture[0] = nullptr;
	}

	// ��Ű�� �ִϸ��̼��� ���� ��Ų������ ���ٸ� �ǹ̰� ���� ������ ����
	if (nullptr == pSkinInfo)
		return S_OK;

	EXMeshContainer->pSkinInfo = pSkinInfo;
	SafeAddRef(EXMeshContainer->pSkinInfo);

	// �޽������̳��� �޽��������� ���� ������ �޽�(�ִϸ��̼� �������� ���� �޽�)�� �״�� ����ش�.
	EXMeshContainer->MeshData.pMesh->CloneMeshFVF(
		EXMeshContainer->MeshData.pMesh->GetOptions(), 
		EXMeshContainer->MeshData.pMesh->GetFVF(), 
		m_device, 
		&EXMeshContainer->originMesh);

	// ���� ������ ������ �� ������ŭ �����Ҵ�
	EXMeshContainer->numBones = EXMeshContainer->pSkinInfo->GetNumBones();

	EXMeshContainer->frameOffsetMatrix = new _matrix[EXMeshContainer->numBones];
	ZeroMemory(EXMeshContainer->frameOffsetMatrix, sizeof(_matrix) * EXMeshContainer->numBones);

	EXMeshContainer->frameCombinedMatrix = new _matrix*[EXMeshContainer->numBones];
	ZeroMemory(EXMeshContainer->frameCombinedMatrix, sizeof(_matrix*) * EXMeshContainer->numBones);

	EXMeshContainer->renderingMatrix = new _matrix[EXMeshContainer->numBones];
	ZeroMemory(EXMeshContainer->renderingMatrix, sizeof(_matrix) * EXMeshContainer->numBones);

	for (_ulong i = 0; i < EXMeshContainer->numBones; ++i)
	{
		// �޽��� �׸��� ���ؼ� ����鿡�� �ε��� ���� �Ű��� �ִ�.
		// ��Ų�������� �ε����� ���ؼ� �� ��� ������ �����´�.
		EXMeshContainer->frameOffsetMatrix[i] = *EXMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
	}
	
	*ppNewMeshContainer = EXMeshContainer;

	return S_OK;
}

STDMETHODIMP CHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	SafeDeleteArray(pFrameToFree->Name);
	D3DXFRAME_EX* EXFrame = (D3DXFRAME_EX*)pFrameToFree;
	SafeDelete(EXFrame->combinedTransformationMatrix);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	SafeDelete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP CHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_EX* EXMeshContainer = (D3DXMESHCONTAINER_EX*)pMeshContainerToFree;

	for (_ulong i = 0; i < EXMeshContainer->NumMaterials; ++i)
		SafeRelease(EXMeshContainer->texture[i]);

	SafeRelease(EXMeshContainer->originMesh);
	SafeRelease(EXMeshContainer->pSkinInfo);
	SafeRelease(EXMeshContainer->MeshData.pMesh);

	SafeDeleteArray(EXMeshContainer->Name);
	SafeDeleteArray(EXMeshContainer->texture);
	SafeDeleteArray(EXMeshContainer->frameOffsetMatrix);
	SafeDeleteArray(EXMeshContainer->frameCombinedMatrix);
	SafeDeleteArray(EXMeshContainer->renderingMatrix);
	SafeDeleteArray(EXMeshContainer->pAdjacency);
	SafeDeleteArray(EXMeshContainer->pMaterials);
	
	SafeDelete(EXMeshContainer);

	return S_OK;
}

void CHierarchyLoader::AllocateName(char** outName, const char* inFrameName)
{
	if (nullptr == inFrameName)
		return;

	_uint length = strlen(inFrameName);

	*outName = new char[length + 1];

	strcpy_s(*outName, length + 1, inFrameName);
}

CHierarchyLoader* CHierarchyLoader::Create(LPDIRECT3DDEVICE9 device, const _tchar* path)
{
	return new CHierarchyLoader(device, path);
}

_ulong CHierarchyLoader::Release()
{
	SafeRelease(m_device);

	delete this;

	return 0;
}

