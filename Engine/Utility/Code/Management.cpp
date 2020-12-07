#include "Management.h"
#include "Scene.h"
#include "Renderer.h"

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

void CManagement::RenderScene()
{
	CRenderer::GetInstance()->Render();

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
