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

	// 처음에는 항등행렬로 초기화시켜준다.
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
	// 새로운 메쉬컨테이너 생성
	D3DXMESHCONTAINER_EX* EXMeshContainer = new D3DXMESHCONTAINER_EX;
	ZeroMemory(EXMeshContainer, sizeof(D3DXMESHCONTAINER_EX));

	// 1. 이름을 할당해준다.
	AllocateName(&EXMeshContainer->Name, Name);

	EXMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	// 메쉬데이터의 메쉬를 얻어온다.
	LPD3DXMESH mesh = pMeshData->pMesh;

	// 현재 메쉬가 지닌 폴리곤의 개수를 반환
	_ulong numFaces = mesh->GetNumFaces();

	// 인접 폴리곤을 저장하기 위한 배열을 할당
	EXMeshContainer->pAdjacency = new _ulong[numFaces * 3];
	memcpy(EXMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * numFaces * 3);

	_ulong meshFVF = mesh->GetFVF();

	// 메쉬에 노말이 없는 경우 컴퓨트 노말 삽입 후 메쉬컨테이너에 복사
	if (!(meshFVF & D3DFVF_NORMAL))
	{
		mesh->CloneMeshFVF(mesh->GetOptions(), meshFVF | D3DFVF_NORMAL, m_device, &EXMeshContainer->MeshData.pMesh);
		D3DXComputeNormals(EXMeshContainer->MeshData.pMesh, EXMeshContainer->pAdjacency);
	}
	else
	{
		mesh->CloneMeshFVF(mesh->GetOptions(), meshFVF, m_device, &EXMeshContainer->MeshData.pMesh);
	}

	// 메쉬의 매터리얼과 텍스쳐를 가져오는 작업
	EXMeshContainer->NumMaterials = ((NumMaterials == 0) ? 1 : NumMaterials);

	EXMeshContainer->pMaterials = new D3DXMATERIAL[EXMeshContainer->NumMaterials];
	ZeroMemory(EXMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * EXMeshContainer->NumMaterials);

	EXMeshContainer->texture = new LPDIRECT3DTEXTURE9[EXMeshContainer->NumMaterials];
	ZeroMemory(EXMeshContainer->texture, sizeof(LPDIRECT3DTEXTURE9) * EXMeshContainer->NumMaterials);

	if (0 != NumMaterials)
	{
		memcpy(EXMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * EXMeshContainer->NumMaterials);

		// 메쉬의 매터리얼을 로드하는 작업
		for (_ulong i = 0; i < EXMeshContainer->NumMaterials; ++i)
		{
			_tchar	szFullPath[256] = L"";
			_tchar	szFileName[256] = L"";

			lstrcpy(szFullPath, m_path);

			// 텍스쳐파일명(아스키코드)를 멀티바이트 이름으로 바꾸는 과정
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
	// 메쉬의 매터리얼이 0인 경우 디버깅을 위해서 일단 로드 단계에서 오류가 나지 않게 하기 위한 과정
	else
	{
		EXMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		EXMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		EXMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		EXMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		EXMeshContainer->pMaterials[0].MatD3D.Power = 0.f;

		EXMeshContainer->texture[0] = nullptr;
	}

	// 스키닝 애니메이션을 위한 스킨인포가 없다면 의미가 없기 때문에 리턴
	if (nullptr == pSkinInfo)
		return S_OK;

	EXMeshContainer->pSkinInfo = pSkinInfo;
	SafeAddRef(EXMeshContainer->pSkinInfo);

	// 메쉬컨테이너의 메쉬데이터의 값을 오리진 메쉬(애니메이션 프레임을 위한 메쉬)에 그대로 담아준다.
	EXMeshContainer->MeshData.pMesh->CloneMeshFVF(
		EXMeshContainer->MeshData.pMesh->GetOptions(), 
		EXMeshContainer->MeshData.pMesh->GetFVF(), 
		m_device, 
		&EXMeshContainer->originMesh);

	// 뼈의 갯수를 얻어오고 그 갯수만큼 동적할당
	EXMeshContainer->numBones = EXMeshContainer->pSkinInfo->GetNumBones();

	EXMeshContainer->frameOffsetMatrix = new _matrix[EXMeshContainer->numBones];
	ZeroMemory(EXMeshContainer->frameOffsetMatrix, sizeof(_matrix) * EXMeshContainer->numBones);

	EXMeshContainer->frameCombinedMatrix = new _matrix*[EXMeshContainer->numBones];
	ZeroMemory(EXMeshContainer->frameCombinedMatrix, sizeof(_matrix*) * EXMeshContainer->numBones);

	EXMeshContainer->renderingMatrix = new _matrix[EXMeshContainer->numBones];
	ZeroMemory(EXMeshContainer->renderingMatrix, sizeof(_matrix) * EXMeshContainer->numBones);

	for (_ulong i = 0; i < EXMeshContainer->numBones; ++i)
	{
		// 메쉬를 그리기 위해서 뼈대들에는 인덱스 값이 매겨져 있다.
		// 스킨인포에서 인덱스를 통해서 각 행렬 정보를 가져온다.
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

