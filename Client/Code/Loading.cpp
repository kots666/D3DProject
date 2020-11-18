#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 device) :
	m_device(device),
	m_isFinish(false)
{
	ZeroMemory(m_loadingStr, sizeof(_tchar) * 256);
	m_device->AddRef();
}

CLoading::~CLoading()
{
}

_uint	CALLBACK CLoading::ThreadMain(void* arg)
{
	CLoading* loading = (CLoading*)arg;

	_uint flag = 0;

	EnterCriticalSection(loading->GetCRT());

	switch (loading->GetLoadingID())
	{
	case LOADING_STAGE:
		flag = loading->LoadingForStage();		
		break;

	case LOADING_BOSS:
		break;
	}
	
	LeaveCriticalSection(loading->GetCRT());
	_endthreadex(0);

	return flag;
}

HRESULT CLoading::Ready(LOADINGID loadID)
{
	InitializeCriticalSection(&m_crt);

	m_thread = (HANDLE)_beginthreadex(NULL, 0, ThreadMain, this, 0, NULL);

	m_loadingID = loadID;

	return S_OK;
}

_uint CLoading::LoadingForStage()
{
	lstrcpy(m_loadingStr, L"Buffer Loading.............................");

	int i = 0;

	FAILED_CHECK_RETURN(Engine::ReadyBuffer(m_device, Engine::RESOURCE_STAGE, L"Buffer_CubeTex", Engine::BUFFER_CUBETEX, VTXCNTX, VTXCNTZ, VTXITV), E_FAIL);

	// buffer
	/*FAILED_CHECK_RETURN(Engine::ReadyBuffer(m_device,
												Engine::RESOURCE_STATIC,
												L"Buffer_TerrainTex",
												Engine::BUFFER_TERRAINTEX,
												VTXCNTX,
												VTXCNTZ,
												VTXITV),
												E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyBuffer(m_device,
												Engine::RESOURCE_STATIC,
												L"Buffer_CubeTex",
												Engine::BUFFER_CUBETEX),
												E_FAIL);*/


	lstrcpy(m_loadingStr, L"Texture Loading.............................");
	
	// ÅØ½ºÃÄ

	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device,
												Engine::RESOURCE_STAGE,
												L"Texture_Terrain",
												Engine::TEX_NORMAL,
												L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", 2),
												E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device, Engine::RESOURCE_STAGE, L"Texture_SkyBox", Engine::TEX_CUBE, L"../Resource/Texture/SkyBox/burger%d.dds", 4), E_FAIL);

	
	
	lstrcpy(m_loadingStr, L"Mesh Loading.............................");

	// Stone
	FAILED_CHECK_RETURN(Engine::ReadyMesh(m_device,
										  Engine::RESOURCE_STAGE,
										  L"Mesh_Stone", 
										  Engine::TYPE_STATIC, 
										  L"../Bin/Resource/Mesh/StaticMesh/TombStone/",
										  L"TombStone.X"),
										  E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Player",
	//	Engine::TYPE_DYNAMIC,
	//	L"../Bin/Resource/Mesh/DynamicMesh/PlayerXfile/",
	//	L"Player.X"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Sword",
	//	Engine::TYPE_STATIC,
	//	L"../Bin/Resource/Mesh/StaticMesh/Sword/",
	//	L"Sword.X"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Tree",
	//	Engine::TYPE_STATIC,
	//	L"../Bin/Resource/Mesh/StaticMesh/Tree/",
	//	L"Tree01.X"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Navi",
	//	Engine::TYPE_NAVI,
	//	NULL,
	//	NULL),
	//	E_FAIL);

	
	lstrcpy(m_loadingStr, L"Loading Complete!!!");

	m_isFinish = true;

	return 0;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 device, LOADINGID loadID)
{
	CLoading* instance = new CLoading(device);

	if (FAILED(instance->Ready(loadID)))
		SafeRelease(instance);

	return instance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_thread, INFINITE);
	CloseHandle(m_thread);

	DeleteCriticalSection(&m_crt);

	Engine::SafeRelease(m_device);
}

