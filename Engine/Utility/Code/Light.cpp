#include "Light.h"

USING(Engine)

CLight::CLight(LPDIRECT3DDEVICE9 device) :
	m_device(device), m_index(0)
{
	ZeroMemory(&m_lightInfo, sizeof(D3DLIGHT9));
	SafeAddRef(m_device);
}

CLight::~CLight()
{

}

HRESULT CLight::Ready(const D3DLIGHT9* lightInfo, const _uint& index)
{
	memcpy(&m_lightInfo, lightInfo, sizeof(D3DLIGHT9));

	m_index = index;

	m_device->SetLight(index, lightInfo);
	m_device->LightEnable(index, TRUE);

	return S_OK;
}

CLight* CLight::Create(LPDIRECT3DDEVICE9 device, const D3DLIGHT9* lightInfo, const _uint& index)
{
	CLight* instance = new CLight(device);

	if (FAILED(instance->Ready(lightInfo, index)))
		SafeRelease(instance);

	return instance;
}

void CLight::Free()
{
	m_device->LightEnable(m_index, FALSE);
	SafeRelease(m_device);
}

