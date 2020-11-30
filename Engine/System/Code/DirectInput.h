#ifndef DirectInput_h__
#define DirectInput_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CDirectInput : public CBase
{
	DECLARE_SINGLETON(CDirectInput)

private:
	explicit CDirectInput();
	virtual ~CDirectInput();

public:
	_byte GetDIKeyState(_ubyte keyID) { return m_keyState[keyID]; }
	_byte GetDIKeyDownState(_ubyte keyID) { return m_keyDownState[keyID]; }
	_byte GetDIKeyUpState(_ubyte keyID) { return m_keyUpState[keyID]; }
	_byte GetDIMouseState(MOUSEKEYSTATE mouse) { return m_mouseState.rgbButtons[mouse]; }
	_long GetDIMouseMove(MOUSEMOVESTATE mouseState)
	{
		return *(((_long*)&m_mouseState) + mouseState);
	}


public:
	HRESULT Ready(HINSTANCE hInst, HWND hWnd);
	void	Update();

private:
	LPDIRECTINPUT8 m_inputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8 m_keyBoard = nullptr;
	LPDIRECTINPUTDEVICE8 m_mouse = nullptr;

private:
	_byte m_keyState[256];
	_byte m_keyDownState[256];
	_byte m_keyUpState[256];
	DIMOUSESTATE m_mouseState;

public:
	virtual void Free();

};

END

#endif
