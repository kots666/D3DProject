#include "stdafx.h"
#include "SkyBox.h"
#include "CubeTex.h"
#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 device) :
	CGameObject(device)
{

}

CSkyBox::~CSkyBox()
{

}

HRESULT CSkyBox::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_transCom->SetScale(40.f, 40.f, 40.f);

	return S_OK;
}

_int CSkyBox::Update(const _float& deltaTime)
{
	Engine::CGameObject::Update(deltaTime);
	m_rendererCom->AddObject(Engine::RENDER_SKYBOX, this);

	_matrix	matCamWorld;
	m_device->GetTransform(D3DTS_VIEW, &matCamWorld);

	// 카메라 월드 행렬
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	m_transCom->SetPos(matCamWorld._41, matCamWorld._42, matCamWorld._43);

	m_transCom->SetTransform(m_device);

	return 0;
}
void CSkyBox::Render()
{
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_texCom->RenderTexture(3);
	m_bufferCom->Render();

	m_device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_device->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CSkyBox::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// buffer
	component = m_bufferCom = dynamic_cast<Engine::CCubeTex*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Buffer_CubeTex"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Buffer", component);

	// texture
	component = m_texCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Texture_SkyBox"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Texture", component);

	// Renderer
	component = m_rendererCom = Engine::GetRenderer();
	NULL_CHECK_RETURN(component, E_FAIL);
	SafeAddRef(component);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Renderer", component);
	
	// Transform
	component = m_transCom = dynamic_cast<Engine::CTransform*>(Engine::CloneComp(L"Proto_Transform"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", component);

	return S_OK;
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 device)
{
	CSkyBox* instance = new CSkyBox(device);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CSkyBox::Free()
{
	Engine::CGameObject::Free();
}