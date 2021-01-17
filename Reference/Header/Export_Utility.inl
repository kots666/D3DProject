#include "Export_Utility.h"

HRESULT CreateManagement(CManagement ** outer, LPDIRECT3DDEVICE9 & device)
{
	CManagement* instance = CManagement::GetInstance();
	if (nullptr == instance) return E_FAIL;

	if (FAILED(instance->ReadyShader(device)))
		return E_FAIL;

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

inline _int LateUpdateScene(const _float & deltaTime)
{
	return CManagement::GetInstance()->LateUpdateScene(deltaTime);
}

void RenderScene(LPDIRECT3DDEVICE9& device)
{
	CManagement::GetInstance()->RenderScene(device);
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

inline const D3DLIGHT9 * GetLight(const _uint & index)
{
	return CLightMgr::GetInstance()->GetLight(index);
}

inline HRESULT ReadyLight(LPDIRECT3DDEVICE9 device, const D3DLIGHT9 * lightInfo, const _uint & index)
{
	return CLightMgr::GetInstance()->ReadyLight(device, lightInfo, index);
}

inline void RenderLight(LPD3DXEFFECT & effect)
{
	CLightMgr::GetInstance()->RenderLight(effect);
}

inline HRESULT ReadyRenderTarget(LPDIRECT3DDEVICE9 device, const _tchar * targetTag, const _uint & width, const _uint & height, D3DFORMAT format, D3DXCOLOR color)
{
	return CRenderTargetMgr::GetInstance()->ReadyRenderTarget(device, targetTag, width, height, format, color);
}

inline HRESULT ReadyMRT(const _tchar * MRTTag, const _tchar * targetTag)
{
	return CRenderTargetMgr::GetInstance()->ReadyMRT(MRTTag, targetTag);
}

inline HRESULT BeginMRT(const _tchar * MRTTag)
{
	return CRenderTargetMgr::GetInstance()->BeginMRT(MRTTag);
}

inline HRESULT EndMRT(const _tchar * MRTTag)
{
	return CRenderTargetMgr::GetInstance()->EndMRT(MRTTag);
}

inline HRESULT ReadyDebugBuffer(const _tchar * targetTag, const _float & x, const _float & y, const _float & sizeX, const _float & sizeY)
{
	return CRenderTargetMgr::GetInstance()->ReadyDebugBuffer(targetTag, x, y, sizeX, sizeY);
}

inline void RenderDebugBuffer(const _tchar * MRTTag)
{
	CRenderTargetMgr::GetInstance()->RenderDebugBuffer(MRTTag);
}

inline void ThrowRenderTargetTexture(LPD3DXEFFECT & effect, const _tchar * targetTag, const char * constantName)
{
	CRenderTargetMgr::GetInstance()->ThrowRenderTargetTexture(effect, targetTag, constantName);
}

void ReleaseUtility()
{
	CRenderTargetMgr::DestroyInstance();
	CRenderer::DestroyInstance();
	CManagement::DestroyInstance();
	CProtoMgr::DestroyInstance();
	CLightMgr::DestroyInstance();
}