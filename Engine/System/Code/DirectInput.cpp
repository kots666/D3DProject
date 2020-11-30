#include "DirectInput.h"

USING(Engine)
IMPLEMENT_SINGLETON(CDirectInput)

CDirectInput::CDirectInput()
{

}

CDirectInput::~CDirectInput()
{
	Free();
}

HRESULT CDirectInput::Ready(HINSTANCE hInst, HWND hWnd)
{

	// DInput 컴객체를 생성하는 함수
	FAILED_CHECK_RETURN(DirectInput8Create(hInst,
											DIRECTINPUT_VERSION,
											IID_IDirectInput8,
											(void**)&m_inputSDK,
											NULL), E_FAIL);

	// 키보드 객체 생성
	FAILED_CHECK_RETURN(m_inputSDK->CreateDevice(GUID_SysKeyboard, &m_keyBoard, nullptr), E_FAIL);

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_keyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_keyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_keyBoard->Acquire();


	// 마우스 객체 생성
	FAILED_CHECK_RETURN(m_inputSDK->CreateDevice(GUID_SysMouse, &m_mouse, nullptr), E_FAIL);

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_mouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_mouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_mouse->Acquire();


	return S_OK;
}

void CDirectInput::Update()
{
	m_keyBoard->GetDeviceState(256, m_keyState);
	for (_int i = 0; i < 256; ++i)
	{
		_short keyState = GetAsyncKeyState(i);
		m_keyDownState[i] = keyState & 0x0001;
		m_keyUpState[i] = keyState & 0x8000;
	}
	m_mouse->GetDeviceState(sizeof(m_mouseState), &m_mouseState);
}

void CDirectInput::Free()
{
	SafeRelease(m_mouse);
	SafeRelease(m_keyBoard);
	SafeRelease(m_inputSDK);
}

