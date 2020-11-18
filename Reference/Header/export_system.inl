HRESULT ReadyGraphicDevice(HWND hWnd, WINMODE mode, const _uint& sizeX, const _uint& sizeY, CGraphicDevice** graphicDevice)
{
	return CGraphicDevice::GetInstance()->Ready(hWnd, mode, sizeX, sizeY, graphicDevice);
}
void RenderBegin(D3DXCOLOR color)
{
	CGraphicDevice::GetInstance()->RenderBegin(color);
}
void RenderEnd()
{
	CGraphicDevice::GetInstance()->RenderEnd();
}

_float GetDeltaTime(const _tchar* timerTag)
{
	return CTimerManager::GetInstance()->GetDeltaTime(timerTag);
}

void SetDeltaTime(const _tchar* timerTag)
{
	CTimerManager::GetInstance()->Update(timerTag);
}

HRESULT ReadyTimer(const _tchar* timerTag)
{
	return CTimerManager::GetInstance()->Ready(timerTag);
}

_bool IsPermitCall(const _tchar* frameTag, const _float& deltaTime)
{
	return CFrameManager::GetInstance()->IsPermitCall(frameTag, deltaTime);
}

HRESULT ReadyFrame(const _tchar* frameTag, const _float& callLimit)
{
	return CFrameManager::GetInstance()->Ready(frameTag, callLimit);
}

HRESULT ReadyFont(LPDIRECT3DDEVICE9 device, const _tchar* fontTag, const _tchar* fontType, const _uint& width, const _uint& height, const _uint& weight)
{
	return CFontMgr::GetInstance()->Ready(device, fontTag, fontType, width, height, weight);
}
void RenderFont(const _tchar* fontTag, const _tchar* string, const _vec2* pos, D3DXCOLOR color)
{
	CFontMgr::GetInstance()->Render(fontTag, string, pos, color);
}

_byte GetDIKeyState(_ubyte keyID)
{
	return CDirectInput::GetInstance()->GetDIKeyState(keyID);
}
_byte GetDIMouseState(MOUSEKEYSTATE mouse)
{
	return CDirectInput::GetInstance()->GetDIMouseState(mouse);
}
_long GetDIMouseMove(MOUSEMOVESTATE mouseState)
{
	return CDirectInput::GetInstance()->GetDIMouseMove(mouseState);
}
HRESULT ReadyDirectInput(HINSTANCE hInst, HWND hWnd)
{
	return CDirectInput::GetInstance()->Ready(hInst, hWnd);
}
void UpdateDirectInput()
{
	CDirectInput::GetInstance()->Update();
}

// ============================================================

void ReleaseSystem()
{
	CDirectInput::DestroyInstance();
	CFontMgr::DestroyInstance();
	CTimerManager::DestroyInstance();
	CFrameManager::DestroyInstance();
	CGraphicDevice::DestroyInstance();
}