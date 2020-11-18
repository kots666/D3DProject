#include "Export_Utility.h"
HRESULT CreateManagement(CManagement ** outer)
{
	CManagement* instance = CManagement::GetInstance();
	if (nullptr == instance) return E_FAIL;

	*outer = instance;

	return S_OK;
}

CComponent * GetComponent(const _tchar * layerKey, const _tchar * objKey, const _tchar * compKey, COMPONENTID id)
{
	return CManagement::GetInstance()->GetComponent(layerKey, objKey, compKey, id);
}

HRESULT SetUpScene(CScene * scene)
{
	return CManagement::GetInstance()->SetUpScene(scene);
}

_int UpdateScene(const _float & deltaTime)
{
	return CManagement::GetInstance()->UpdateScene(deltaTime);
}

void RenderScene()
{
	CManagement::GetInstance()->RenderScene();
}

CScene* GetCurScene()
{
	return CManagement::GetInstance()->GetScene();
}

HRESULT ReadyProto(const _tchar* protoTag, CComponent* comp)
{
	return CProtoMgr::GetInstance()->Ready(protoTag, comp);
}

CComponent* CloneComp(const _tchar* protoTag)
{
	return CProtoMgr::GetInstance()->Clone(protoTag);
}

CRenderer* GetRenderer()
{
	return CRenderer::GetInstance();
}

inline HRESULT ReadyLight(LPDIRECT3DDEVICE9 device, const D3DLIGHT9 * lightInfo, const _uint & index)
{
	return CLightMgr::GetInstance()->ReadyLight(device, lightInfo, index);
}

void ReleaseUtility()
{
	CRenderer::DestroyInstance();
	CManagement::DestroyInstance();
	CProtoMgr::DestroyInstance();
	CLightMgr::DestroyInstance();
}