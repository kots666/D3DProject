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

	// DInput �İ�ü�� �����ϴ� �Լ�
	FAILED_CHECK_RETURN(DirectInput8Create(hInst,
											DIRECTINPUT_VERSION,
											IID_IDirectInput8,
											(void**)&m_inputSDK,
											NULL), E_FAIL);

	// Ű���� ��ü ����
	FAILED_CHECK_RETURN(m_inputSDK->CreateDevice(GUID_SysKeyboard, &m_keyBoard, nullptr), E_FAIL);

	// ������ Ű���� ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_keyBoard->SetDataFormat(&c_dfDIKeyboard);

	// ��ġ�� ���� �������� �������ִ� �Լ�, (Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�)
	m_keyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_keyBoard->Acquire();


	// ���콺 ��ü ����
	FAILED_CHECK_RETURN(m_inputSDK->CreateDevice(GUID_SysMouse, &m_mouse, nullptr), E_FAIL);

	// ������ ���콺 ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_mouse->SetDataFormat(&c_dfDIMouse);

	// ��ġ�� ���� �������� �������ִ� �Լ�, Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�
	m_mouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
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

