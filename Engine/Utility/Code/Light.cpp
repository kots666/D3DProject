#include "Light.h"

USING(Engine)

CLight::CLight(LPDIRECT3DDEVICE9 device) :
	m_device(device), m_index(0),
	m_VB(nullptr), m_IB(nullptr)
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

	FAILED_CHECK_RETURN(m_device->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,
		0, // 정적버퍼로 할당하겠다는 옵션
		FVF_SCREEN,
		D3DPOOL_MANAGED,
		&m_VB,
		NULL),
		E_FAIL);

	FAILED_CHECK_RETURN(m_device->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0, // 정적버퍼로 할당하겠다는 옵션
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_IB,
		NULL),
		E_FAIL);

	D3DVIEWPORT9 viewPort;
	m_device->GetViewport(&viewPort);

	VTXSCREEN* vertices = NULL;

	m_VB->Lock(0, 0, (void**)&vertices, NULL);

	// 오른쪽 위
	vertices[0].pos = _vec4(0.f - 0.5f, 0.f - 0.5f, 0.f, 1.f);
	vertices[0].texUV = _vec2(0.f, 0.f);

	vertices[1].pos = _vec4(viewPort.Width - 0.5f, 0.f - 0.5f, 0.f, 1.f);
	vertices[1].texUV = _vec2(1.f, 0.f);

	vertices[2].pos = _vec4(viewPort.Width - 0.5f, viewPort.Height - 0.5f, 0.f, 1.f);
	vertices[2].texUV = _vec2(1.f, 1.f);

	vertices[3].pos = _vec4(0.f - 0.5f, viewPort.Height - 0.5f, 0.f, 1.f);
	vertices[3].texUV = _vec2(0.f, 1.f);

	m_VB->Unlock();

	INDEX16* indices = nullptr;

	m_IB->Lock(0, 0, (void**)&indices, 0);

	indices[0]._0 = 0;
	indices[0]._1 = 1;
	indices[0]._2 = 2;

	indices[1]._0 = 0;
	indices[1]._1 = 2;
	indices[1]._2 = 3;

	m_IB->Unlock();

	m_device->SetLight(index, lightInfo);
	m_device->LightEnable(index, TRUE);

	return S_OK;
}

void CLight::Render(LPD3DXEFFECT & effect)
{
	effect->SetVector("g_vLightDir", &_vec4(m_lightInfo.Direction, 0.f));
	effect->SetVector("g_vLightDiffuse", (_vec4*)&m_lightInfo.Diffuse);
	effect->SetVector("g_vLightAmbient", (_vec4*)&m_lightInfo.Ambient);

	effect->CommitChanges();

	effect->BeginPass(0);

	m_device->SetStreamSource(0, m_VB, 0, sizeof(VTXSCREEN));
	m_device->SetFVF(FVF_SCREEN);
	m_device->SetIndices(m_IB);
	m_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	effect->EndPass();
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
	SafeRelease(m_IB);
	SafeRelease(m_VB);
	SafeRelease(m_device);
}

