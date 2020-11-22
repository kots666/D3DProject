#include "GraphicDevice.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGraphicDevice)

CGraphicDevice::CGraphicDevice()
{
}

CGraphicDevice::~CGraphicDevice()
{
	Free();
}

HRESULT CGraphicDevice::Ready(HWND hWnd, WINMODE mode, const _uint & sizeX, const _uint & sizeY, CGraphicDevice ** graphicDevice)
{
	m_SDK = Direct3DCreate9(D3D_SDK_VERSION);
	if (nullptr == m_SDK) return E_FAIL;

	D3DCAPS9 caps;
	ZeroMemory(&caps, sizeof(D3DCAPS9));

	// 장치의 성능정보를 CAPS 구조체에 얻어옴
	if (FAILED(m_SDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		MSG_BOX("Fail to get device caps");
		return E_FAIL;
	}

	_ulong	vp = 0;

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS		params;
	ZeroMemory(&params, sizeof(D3DPRESENT_PARAMETERS));

	// 백버퍼 관련
	params.BackBufferWidth = sizeX;
	params.BackBufferHeight = sizeY;
	params.BackBufferFormat = D3DFMT_A8R8G8B8;
	params.BackBufferCount = 1;

	// 멀티 샘플링
	params.MultiSampleType = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality = 0;

	// 스왑 방식
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = hWnd;

	// 전체화면 or 창모드
	params.Windowed = mode;

	// 버퍼에는 타겟버퍼, 뎁스버퍼(Z-Buffer), 스텐실 버퍼가 존재한다.
	params.EnableAutoDepthStencil = TRUE;
	params.AutoDepthStencilFormat = D3DFMT_D24S8;

	// 화면 주사율과 인터벌 세팅
	params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_SDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &params, &m_device)))
	{
		MSG_BOX("Fail to create device");
		return E_FAIL;
	}

	*graphicDevice = this;

	return S_OK;
}

void CGraphicDevice::RenderBegin(D3DXCOLOR color)
{
	m_device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, color, 1.f, 0);
	m_device->BeginScene();
}

void CGraphicDevice::RenderEnd(HWND hWnd)
{
	m_device->EndScene();
	m_device->Present(nullptr, nullptr, hWnd, nullptr);
}

void CGraphicDevice::Free()
{
	_ulong refCnt = 0;

	if (refCnt = SafeRelease(m_device))
	{
		MSG_BOX("Fail to release graphic device");
	}

	if (refCnt = SafeRelease(m_SDK))
	{
		MSG_BOX("Fail to release SDK");
	}
}
