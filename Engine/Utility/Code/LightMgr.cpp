#include "LightMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CLightMgr)

CLightMgr::CLightMgr()
{

}

CLightMgr::~CLightMgr()
{
	Free();
}

HRESULT CLightMgr::ReadyLight(LPDIRECT3DDEVICE9 device, const D3DLIGHT9* lightInfo, const _uint& index)
{
	CLight* instance = CLight::Create(device, lightInfo, index);
	NULL_CHECK_RETURN(instance, E_FAIL);

	m_lightList.push_back(instance);

	return S_OK;
}

void CLightMgr::Free()
{
	for_each(m_lightList.begin(), m_lightList.end(), CDeleteObj());
	m_lightList.clear();
}

