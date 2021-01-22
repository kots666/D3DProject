#include "Export_Utility.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement() :
	m_scene(nullptr)
{
}

CManagement::~CManagement()
{
	Free();
}

HRESULT CManagement::ReadyShader(LPDIRECT3DDEVICE9 & device)
{
	D3DVIEWPORT9 viewPort;
	device->GetViewport(&viewPort);

	FAILED_CHECK_RETURN(ReadyRenderTarget(device,
		L"Target_Albedo",
		viewPort.Width,
		viewPort.Height,
		D3DFMT_A16B16G16R16F,
		D3DXCOLOR(0.f, 0.f, 0.f, 0.f)),
		E_FAIL);

	FAILED_CHECK_RETURN(ReadyDebugBuffer(L"Target_Albedo", 0.f, 0.f, 100.f, 100.f), E_FAIL);

	FAILED_CHECK_RETURN(ReadyRenderTarget(device,
		L"Target_Normal",
		viewPort.Width,
		viewPort.Height,
		D3DFMT_A16B16G16R16F,
		D3DXCOLOR(0.f, 0.f, 0.f, 1.f)),
		E_FAIL);
	FAILED_CHECK_RETURN(ReadyDebugBuffer(L"Target_Normal", 0.f, 100.f, 100.f, 100.f), E_FAIL);

	FAILED_CHECK_RETURN(ReadyRenderTarget(device,
		L"Target_Shade",
		viewPort.Width,
		viewPort.Height,
		D3DFMT_A16B16G16R16F,
		D3DXCOLOR(0.f, 0.f, 0.f, 1.f)),
		E_FAIL);
	FAILED_CHECK_RETURN(ReadyDebugBuffer(L"Target_Shade", 0.f, 200.f, 100.f, 100.f), E_FAIL);

	FAILED_CHECK_RETURN(ReadyRenderTarget(device,
		L"Target_Depth",
		viewPort.Width,
		viewPort.Height,
		D3DFMT_A32B32G32R32F,
		D3DXCOLOR(1.f, 1.f, 1.f, 1.f)),
		E_FAIL);
	FAILED_CHECK_RETURN(ReadyDebugBuffer(L"Target_Depth", 0.f, 300.f, 100.f, 100.f), E_FAIL);

	FAILED_CHECK_RETURN(ReadyMRT(L"MRT_Deferred", L"Target_Albedo"), E_FAIL);
	FAILED_CHECK_RETURN(ReadyMRT(L"MRT_Deferred", L"Target_Normal"), E_FAIL);
	FAILED_CHECK_RETURN(ReadyMRT(L"MRT_Deferred", L"Target_Depth"), E_FAIL);

	FAILED_CHECK_RETURN(ReadyMRT(L"MRT_LightAcc", L"Target_Shade"), E_FAIL);

	CShader* shader = nullptr;

	// Sample
	shader = CShader::Create(device, L"../../Reference/Header/Shader_Sample.hpp");
	NULL_CHECK_RETURN(shader, E_FAIL);
	FAILED_CHECK_RETURN(ReadyProto(L"Proto_Shader_Sample", shader), E_FAIL);

	// Terrain
	shader = CShader::Create(device, L"../../Reference/Header/Shader_Terrain.hpp");
	NULL_CHECK_RETURN(shader, E_FAIL);
	FAILED_CHECK_RETURN(ReadyProto(L"Proto_Shader_Terrain", shader), E_FAIL);

	// Mesh
	shader = CShader::Create(device, L"../../Reference/Header/Shader_Mesh.hpp");
	NULL_CHECK_RETURN(shader, E_FAIL);
	FAILED_CHECK_RETURN(ReadyProto(L"Proto_Shader_Mesh", shader), E_FAIL);

	// Shade
	shader = CShader::Create(device, L"../../Reference/Header/Shader_Shade.hpp");
	NULL_CHECK_RETURN(shader, E_FAIL);
	FAILED_CHECK_RETURN(ReadyProto(L"Proto_Shader_Shade", shader), E_FAIL);

	// Blend
	shader = CShader::Create(device, L"../../Reference/Header/Shader_Blend.hpp");
	NULL_CHECK_RETURN(shader, E_FAIL);
	FAILED_CHECK_RETURN(ReadyProto(L"Proto_Shader_Blend", shader), E_FAIL);

	// SkyBox
	shader = CShader::Create(device, L"../../Reference/Header/Shader_SkyBox.hpp");
	NULL_CHECK_RETURN(shader, E_FAIL);
	FAILED_CHECK_RETURN(ReadyProto(L"Proto_Shader_SkyBox", shader), E_FAIL);

	// UI
	shader = CShader::Create(device, L"../../Reference/Header/Shader_UI.hpp");
	NULL_CHECK_RETURN(shader, E_FAIL);
	FAILED_CHECK_RETURN(ReadyProto(L"Proto_Shader_UI", shader), E_FAIL);

	// SwordTrail
	shader = CShader::Create(device, L"../../Reference/Header/Shader_SwordTrail.hpp");
	NULL_CHECK_RETURN(shader, E_FAIL);
	FAILED_CHECK_RETURN(ReadyProto(L"Proto_Shader_SwordTrail", shader), E_FAIL);

	// SwordTrail
	shader = CShader::Create(device, L"../../Reference/Header/Shader_Alpha.hpp");
	NULL_CHECK_RETURN(shader, E_FAIL);
	FAILED_CHECK_RETURN(ReadyProto(L"Proto_Shader_Alpha", shader), E_FAIL);

	return S_OK;
}

HRESULT CManagement::SetUpScene(CScene * scene)
{
	SafeRelease(m_scene);

	m_scene = scene;

	return S_OK;
}

_int CManagement::UpdateScene(const _float & deltaTime)
{
	if (nullptr == m_scene) return -1;

	m_scene->Update(deltaTime);

	return 0;
}

_int CManagement::LateUpdateScene(const _float & deltaTime)
{
	if (nullptr == m_scene) return -1;

	m_scene->LateUpdate(deltaTime);

	return 0;
}

void CManagement::RenderScene(LPDIRECT3DDEVICE9& device)
{
	CRenderer::GetInstance()->Render(device);

	if (nullptr == m_scene) return;

	m_scene->Render();
}

CComponent * CManagement::GetComponent(const _tchar * layerKey, const _tchar * objKey, const _tchar * compKey, COMPONENTID id)
{
	return m_scene->GetComponent(layerKey, objKey, compKey, id);
}

void CManagement::Free()
{
	SafeRelease(m_scene);
}
