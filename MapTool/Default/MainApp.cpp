#include "stdafx.h"
#include "MainApp.h"
#include "Stage.h"
#include "NaviMesh.h"

CMainApp::CMainApp() :
	m_deviceClass(nullptr), m_device(nullptr), m_managementClass(nullptr),
	m_frame(0), m_accTime(0.f)
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready()
{
	FAILED_CHECK_RETURN(SetUpDefaultSetting(&m_device), E_FAIL);
	FAILED_CHECK_RETURN(ReadyScene(m_device, &m_managementClass), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyFont(m_device, L"Font_Default", L"¹ÙÅÁ", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::ReadyFont(m_device, L"Font_Jinji", L"±Ã¼­", 30, 30, FW_HEAVY), E_FAIL);

	return S_OK;
}

_int CMainApp::Update(const _float & deltaTime)
{
	if (nullptr == m_managementClass) return -1;

	UpdateDirectInput();

	++m_frame;

	m_accTime += deltaTime;

	if (m_accTime >= 1.f)
	{
		wsprintf(m_fps, L"FPS : %d", m_frame);

		m_accTime = 0.f;
		m_frame = 0;
	}

	CNaviMesh::GetInstance()->Update(deltaTime);
	m_managementClass->UpdateScene(deltaTime);

	return 0;
}

_int CMainApp::LateUpdate(const _float & deltaTime)
{
	if (nullptr == m_managementClass) return -1;

	_int ret = 0;

	ret = m_managementClass->LateUpdateScene(deltaTime);

	return  ret;
}

void CMainApp::Render()
{
	if (nullptr == m_managementClass) return;

	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	Engine::RenderBegin(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f));

	m_managementClass->RenderScene(m_device);
	CNaviMesh::GetInstance()->Render();

	Engine::RenderFont(L"Font_Jinji", m_fps, &_vec2(450.f, 10.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

	Engine::RenderEnd();
}

HRESULT CMainApp::SetUpDefaultSetting(LPDIRECT3DDEVICE9 * outerDevice)
{
	FAILED_CHECK_RETURN(Engine::ReadyGraphicDevice(g_hWnd, Engine::MODE_WIN, WINCX, WINCY, &m_deviceClass), E_FAIL);
	SafeAddRef(m_deviceClass);

	*outerDevice = m_deviceClass->GetDevice();
	SafeAddRef(*outerDevice);

	SafeRelease(m_deviceClass);

	return S_OK;
}

HRESULT CMainApp::ReadyScene(LPDIRECT3DDEVICE9 device, Engine::CManagement ** outerManagement)
{
	Engine::CScene* scene = nullptr;

	scene = CStage::Create(device);
	NULL_CHECK_RETURN(scene, E_FAIL);

	FAILED_CHECK_RETURN(Engine::CreateManagement(outerManagement, m_device), E_FAIL);
	Engine::SafeAddRef(*outerManagement);

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
	SafeRelease(m_device);

	Engine::ReleaseResoures();
	Engine::ReleaseUtility();
	Engine::ReleaseSystem();
}
