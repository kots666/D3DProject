#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "Scene.h"
#include "ProtoMgr.h"
#include "Renderer.h"
#include "Camera.h"
#include "Transform.h"
#include "Calculator.h"
#include "LightMgr.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "Optimization.h"
#include "Shader.h"
#include "RenderTargetMgr.h"

BEGIN(Engine)

// Management
inline HRESULT CreateManagement(CManagement** outer, LPDIRECT3DDEVICE9& device);

inline CComponent* GetComponent(const _tchar* layerKey, const _tchar* objKey, const _tchar* compKey, COMPONENTID id);

inline HRESULT SetUpScene(CScene* scene);
inline _int UpdateScene(const _float& deltaTime);
inline _int LateUpdateScene(const _float& deltaTime);
inline void RenderScene(LPDIRECT3DDEVICE9& device);

inline CScene* GetCurScene();

// protomgr
inline HRESULT ReadyProto(const _tchar* protoTag, CComponent* comp);
inline CComponent* CloneComp(const _tchar* protoTag);

// Renderer
inline CRenderer* GetRenderer();

// LightMgr
inline const D3DLIGHT9* GetLight(const _uint& index);
inline HRESULT ReadyLight(LPDIRECT3DDEVICE9 device, const D3DLIGHT9* lightInfo, const _uint& index);
inline void RenderLight(LPD3DXEFFECT& effect);

// RenderTargetMgr
inline HRESULT ReadyRenderTarget(LPDIRECT3DDEVICE9 device,
	const _tchar* targetTag, const _uint& width,
	const _uint& height,
	D3DFORMAT format,
	D3DXCOLOR color);
inline HRESULT ReadyMRT(const _tchar* MRTTag, const _tchar* targetTag);
inline HRESULT BeginMRT(const _tchar* MRTTag);
inline HRESULT EndMRT(const _tchar* MRTTag);

inline HRESULT ReadyDebugBuffer(const _tchar* targetTag, const _float& x, const _float& y, const _float& sizeX, const _float& sizeY);
inline void RenderDebugBuffer(const _tchar* MRTTag);

inline void ThrowRenderTargetTexture(LPD3DXEFFECT& effect, const _tchar* targetTag, const char* constantName);

// Release Utility
inline void ReleaseUtility();

#include "Export_Utility.inl"

END

#endif