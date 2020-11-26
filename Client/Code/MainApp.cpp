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
	FAILED_CHECK_RETURN(ReadyScene(m_device, &m_managementClass), E_FAIL);

	FAILED_CHECK_RETURN(Engine::ReadyFont(m_device, L"Font_Default", L"바탕", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::ReadyFont(m_device, L"Font_Jinji", L"궁서", 30, 30, FW_HEAVY), E_FAIL);

	Engine::ReadyDirectInput(g_hInst, g_hWnd);

	//m_device->SetRenderState(D3DRS_ZENABLE, TRUE);	// Z버퍼에 기록은 한다. 단, 정렬을 수행할지 말지 결정
	//m_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE); // Z버퍼에 기록을 할지 말지 결정하는 옵션

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

	m_managementClass->UpdateScene(deltaTime);

	return 0;
}

void CMainApp::Render()
{
	if (nullptr == m_managementClass) return;

	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	Engine::RenderBegin(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f));

	m_managementClass->RenderScene();
	Engine::RenderFont(L"Font_Jinji", m_fps, &_vec2(500.f, 10.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	Engine::RenderEnd();
}

HRESULT CMainApp::SetUpDefaultSetting(LPDIRECT3DDEVICE9 * outerDevice)
{
	FAILED_CHECK_RETURN(Engine::ReadyGraphicDevice(g_hWnd, Engine::MODE_WIN, WINCX, WINCY, &m_deviceClass), E_FAIL);
	Engine::SafeAddRef(m_deviceClass);

	*outerDevice = m_deviceClass->GetDevice();
	Engine::SafeAddRef(*outerDevice);

	Client::SafeRelease(m_deviceClass);

	return S_OK;
}

HRESULT CMainApp::ReadyScene(LPDIRECT3DDEVICE9 device, Engine::CManagement ** outerManagement)
{
	Engine::CScene* scene = nullptr;

	scene = CLogo::Create(device);
	NULL_CHECK_RETURN(scene, E_FAIL);

	FAILED_CHECK_RETURN(Engine::CreateManagement(outerManagement), E_FAIL);
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
	Client::SafeRelease(m_device);

	Engine::ReleaseResoures();
	Engine::ReleaseUtility();
	Engine::ReleaseSystem();
}
