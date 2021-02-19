#include "stdafx.h"
#include "MainApp.h"
#include "Logo.h"
#include "Stage.h"

CMainApp::CMainApp() :
	m_deviceClass(nullptr), m_device(nullptr), m_managementClass(nullptr)
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready()
{
	FAILED_CHECK_RETURN(SetUpDefaultSetting(&m_device), E_FAIL);
	FAILED_CHECK_RETURN(Engine::GetRenderer()->ReadyRenderer(m_device), E_FAIL);
	FAILED_CHECK_RETURN(ReadyScene(m_device, &m_managementClass), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyFont(m_device, L"Font_Default", L"¹ÙÅÁ", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::ReadyFont(m_device, L"Font_Jinji", L"±Ã¼­", 30, 30, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::ReadyFont(m_device, L"Font_Square", L"³ª´®½ºÄù¾î", 20, 25, FW_REGULAR), E_FAIL);

	Engine::ReadyDirectInput(g_hInst, g_hWnd);

	CSoundManager::GetInstance()->Initialize();

	CSoundManager::PlayBGM(L"MainTitle.ogg");

	CSpawnManager::GetInstance()->Ready(m_device);

	return S_OK;
}

_int CMainApp::Update(const _float & deltaTime)
{
	if (nullptr == m_managementClass) return -1;

	m_accTime += deltaTime;

	Engine::UpdateDirectInput();

	++m_frame;

	if (m_accTime >= 1.f)
	{
		wsprintf(m_fps, L"FPS : %d", m_frame);

		m_accTime = 0.f;
		m_frame = 0;
	}

	CPlayTimeManager::GetInstance()->Update(deltaTime);

	if (CPlayTimeManager::GetInstance()->GetIsStopNow())
		m_managementClass->UpdateScene(0.f);
	else if (CPlayTimeManager::GetInstance()->GetIsSlowNow())
		m_managementClass->UpdateScene(deltaTime * 0.01f);
	else
		m_managementClass->UpdateScene(deltaTime);

	CColliderManager::GetInstance()->CheckCollision(OBJ_PLAYER, OBJ_ENEMY);
	CColliderManager::GetInstance()->CheckCollision(OBJ_ENEMY, OBJ_PLAYER);
	CColliderManager::GetInstance()->CheckEventCollision();
	CSpawnManager::GetInstance()->CheckSpawnCondition();
	CSpawnManager::GetInstance()->CheckQuestComplete();
	CSoundManager::Update(deltaTime);

	return 0;
}

_int CMainApp::LateUpdate(const _float & deltaTime)
{
	_int ret = 0;

	ret = m_managementClass->LateUpdateScene(deltaTime);

	return ret;
}

void CMainApp::Render()
{
	if (nullptr == m_managementClass) return;

	Engine::RenderBegin(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f));
	m_managementClass->RenderScene(m_device);

#ifdef _DEBUG
	Engine::RenderFont(L"Font_Square", m_fps, &_vec2(WINCX - 300, 10.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
#endif

	/*
	Engine::RenderFont(L"Font_Jinji", m_fps, &_vec2(WINCX - 300, 10.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

	_tchar questStep[40];
	wsprintf(questStep, L"Quest : %d", CQuestManager::GetInstance()->GetStep());
	Engine::RenderFont(L"Font_Jinji", questStep, &_vec2(WINCX - 300, 100.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

	_tchar quest[40];
	if (CQuestManager::GetInstance()->GetIsComplete())
		wsprintf(quest, L"Äù½ºÆ® ¿Ï·á");
	else
	{
		if (CQuestManager::GetInstance()->GetIsProgress())
			wsprintf(quest, L"Äù½ºÆ® ÁøÇàÁß");
		else
			wsprintf(quest, L"Äù½ºÆ® ¹Þ¾Æ¾ßÇÔ");
	}
	Engine::RenderFont(L"Font_Jinji", quest, &_vec2(WINCX - 300, 150.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	*/

	Engine::RenderEnd();
}

HRESULT CMainApp::SetUpDefaultSetting(LPDIRECT3DDEVICE9 * outerDevice)
{
	FAILED_CHECK_RETURN(Engine::ReadyGraphicDevice(g_hWnd, Engine::MODE_FULL, WINCX, WINCY, &m_deviceClass), E_FAIL);
	Engine::SafeAddRef(m_deviceClass);

	*outerDevice = m_deviceClass->GetDevice();
	Engine::SafeAddRef(*outerDevice);

	Client::SafeRelease(m_deviceClass);

	return S_OK;
}

HRESULT CMainApp::ReadyScene(LPDIRECT3DDEVICE9 device, Engine::CManagement ** outerManagement)
{
	FAILED_CHECK_RETURN(Engine::CreateManagement(outerManagement, m_device), E_FAIL);
	Engine::SafeAddRef(*outerManagement);

	Engine::CScene* scene = nullptr;

	scene = CLogo::Create(device);
	NULL_CHECK_RETURN(scene, E_FAIL);

	FAILED_CHECK_RETURN((*outerManagement)->SetUpScene(scene), E_FAIL);

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp* instance = new CMainApp;

	if (FAILED(instance->Ready()))
	{
		MSG_BOX("CMainApp Create Failed");
		SafeRelease(instance);
	}

	return instance;
}

void CMainApp::Free()
{
	CFontManager::DestroyInstance();
	CHitManager::DestroyInstance();
	CQuestManager::DestroyInstance();
	CSoundManager::Destroy();

	Client::SafeRelease(m_device);

	Engine::ReleaseResoures();
	Engine::ReleaseUtility();

	CHPUIManager::DestroyInstance();
	CPlayTimeManager::DestroyInstance();
	CSpawnManager::DestroyInstance();
	CColliderManager::DestroyInstance();
	
	Engine::ReleaseSystem();
}
