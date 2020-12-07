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
#include "Optimization.h"

BEGIN(Engine)

// Management
inline HRESULT CreateManagement(CManagement** outer);

inline CComponent* GetComponent(const _tchar* layerKey, const _tchar* objKey, const _tchar* compKey, COMPONENTID id);

inline HRESULT SetUpScene(CScene* scene);
inline _int UpdateScene(const _float& deltaTime);
inline _int LateUpdateScene(const _float& deltaTime);
inline void RenderScene();

inline CScene* GetCurScene();

// protomgr
inline HRESULT ReadyProto(const _tchar* protoTag, CComponent* comp);
inline CComponent* CloneComp(const _tchar* protoTag);

// Renderer
inline CRenderer* GetRenderer();

// LightMgr
inline HRESULT ReadyLight(LPDIRECT3DDEVICE9 device, const D3DLIGHT9* lightInfo, const _uint& index);

// Release Utility
inline void ReleaseUtility();

#include "Export_Utility.inl"

END

#endif