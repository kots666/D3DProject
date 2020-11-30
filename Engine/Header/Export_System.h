#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDevice.h"
#include "TimerManager.h"
#include "FrameManager.h"
#include "FontMgr.h"
#include "DirectInput.h"

BEGIN(Engine)

// GraphicDev
// Get
// Set
// General
inline HRESULT ReadyGraphicDevice(HWND hWnd, WINMODE mode, const _uint& sizeX, const _uint& sizeY, CGraphicDevice** graphicDevice);
inline void	 RenderBegin(D3DXCOLOR color);
inline void	 RenderEnd();

// TimerManager
// Get
inline _float GetDeltaTime(const _tchar* timerTag);
// Set
inline void SetDeltaTime(const _tchar* timerTag);
// General
inline HRESULT ReadyTimer(const _tchar* timerTag);

// FrameManager
// Get
inline _bool IsPermitCall(const _tchar* frameTag, const _float& deltaTime);
// Set
// General
inline HRESULT ReadyFrame(const _tchar* frameTag, const _float& callLimit);

// FontManager
inline HRESULT ReadyFont(LPDIRECT3DDEVICE9 device, const _tchar* fontTag, const _tchar* fontType, const _uint& width, const _uint& height, const _uint& weight);
inline void RenderFont(const _tchar* fontTag, const _tchar* string, const _vec2* pos, D3DXCOLOR color);

// DirectInput
inline _byte GetDIKeyState(_ubyte keyID);
inline _byte GetDIKeyDownState(_ubyte keyID);
inline _byte GetDIKeyUpState(_ubyte keyID);
inline _byte GetDIMouseState(MOUSEKEYSTATE mouse);
inline _long GetDIMouseMove(MOUSEMOVESTATE mouseState);
inline HRESULT ReadyDirectInput(HINSTANCE hInst, HWND hWnd);
inline	void UpdateDirectInput();

// Release
inline void ReleaseSystem();

#include "Export_System.inl"

END

#endif
