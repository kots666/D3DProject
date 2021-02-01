#include "stdafx.h"
#include "SkyBox.h"
#include "StaticMesh.h"
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

	m_transCom->SetScale(0.01f, 0.01f, 0.01f);

	return S_OK;
}

_int CSkyBox::Update(const _float& deltaTime)
{
	Engine::CGameObject::Update(deltaTime);
	m_rendererCom->AddObject(Engine::RENDER_SKYBOX, this);

	return 0;
}

_int CSkyBox::LateUpdate(const _float & deltaTime)
{
	_matrix	matCamWorld;
	m_device->GetTransform(D3DTS_VIEW, &matCamWorld);

	// 카메라 월드 행렬
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	m_transCom->SetPos(matCamWorld._41, matCamWorld._42 - 230.f, matCamWorld._43);

	return 0;
}

void CSkyBox::Render()
{
	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;
	Engine::SafeAddRef(effect);

	effect->Begin(nullptr, 0);

	FAILED_CHECK_RETURN(SetUpConstantTable(effect), );
	m_meshCom->Render(effect);
	effect->End();

	Engine::SafeRelease(effect);
}

HRESULT CSkyBox::AddComponent()
{
	Engine::CComponent* component = nullptr;

	component = m_meshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Mesh_SkyBox2"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Mesh", component);

	// Renderer
	component = m_rendererCom = Engine::GetRenderer();
	NULL_CHECK_RETURN(component, E_FAIL);
	SafeAddRef(component);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Renderer", component);
	
	// Transform
	component = m_transCom = dynamic_cast<Engine::CTransform*>(Engine::CloneComp(L"Proto_Transform"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", component);

	component = m_shaderCom = dynamic_cast<Engine::CShader*>(Engine::CloneComp(L"Proto_Shader_SkyBox"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Shader", component);

	m_rendererCom->SetFogDensity(0.05f);

	return S_OK;
}

HRESULT CSkyBox::SetUpConstantTable(LPD3DXEFFECT & effect)
{
	_matrix matWorld, matView, matProj;

	m_transCom->GetWorldMatrix(&matWorld);
	m_device->GetTransform(D3DTS_VIEW, &matView);
	m_device->GetTransform(D3DTS_PROJECTION, &matProj);

	effect->SetMatrix("g_matWorld", &matWorld);
	effect->SetMatrix("g_matView", &matView);
	effect->SetMatrix("g_matProj", &matProj);

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