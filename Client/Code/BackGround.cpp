#include "stdafx.h"
#include "BackGround.h"
#include "RCTex.h"
#include "Texture.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDev) :
	Engine::CGameObject(pGraphicDev)
{

}

CBackGround::~CBackGround(void)
{

}

HRESULT CBackGround::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_transCom->SetScale(2.f, 2.f, 1.f);

	return S_OK;
}

_int CBackGround::Update(const _float & deltaTime)
{
	Engine::CGameObject::Update(deltaTime);

	m_rendererCom->AddObject(Engine::RENDER_PRIORITY, this);

	return 0;
}

void CBackGround::Render()
{
	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;

	Engine::SafeAddRef(effect);

	_uint maxPass = 0;
	effect->Begin(&maxPass, 0);
	effect->BeginPass(0);

	FAILED_CHECK_RETURN(SetUpConstantTable(effect), );

	m_bufferCom->Render();

	effect->EndPass();
	effect->End();

	Engine::SafeRelease(effect);

	/*m_texCom->RenderTexture(0);
	m_bufferCom->Render();*/
}

HRESULT CBackGround::AddComponent()
{
	Engine::CComponent* comp = nullptr;

	// buffer
	comp = m_bufferCom = dynamic_cast<Engine::CRCTex*>(Engine::CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RCTex"));
	NULL_CHECK_RETURN(comp, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Buffer", comp);

	// texture
	comp = m_texCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_LOGO, L"Texture_Logo"));
	NULL_CHECK_RETURN(comp, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Texture", comp);

	// Renderer
	comp = m_rendererCom = Engine::GetRenderer();
	NULL_CHECK_RETURN(comp, E_FAIL);
	Engine::SafeAddRef(comp);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Renderer", comp);

	// Transform
	comp = m_transCom = dynamic_cast<Engine::CTransform*>(Engine::CloneComp(L"Proto_Transform"));
	NULL_CHECK_RETURN(comp, E_FAIL);
	m_compMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", comp);

	// Shader
	comp = m_shaderCom = dynamic_cast<Engine::CShader*>(Engine::CloneComp(L"Proto_Shader_Sample"));
	NULL_CHECK_RETURN(comp, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Shader", comp);

	return S_OK;
}

HRESULT CBackGround::SetUpConstantTable(LPD3DXEFFECT & effect)
{
	_matrix worldMat, viewMat, projMat;

	m_transCom->GetWorldMatrix(&worldMat);
	m_device->GetTransform(D3DTS_VIEW, &viewMat);
	m_device->GetTransform(D3DTS_PROJECTION, &projMat);

	effect->SetMatrix("g_matWorld", &worldMat);
	effect->SetMatrix("g_matView", &viewMat);
	effect->SetMatrix("g_matProj", &projMat);

	m_texCom->SetTexture(effect, "g_BaseTexture");

	return S_OK;
}

CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 device)
{
	CBackGround* instance = new CBackGround(device);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CBackGround::Free(void)
{
	Engine::CGameObject::Free();
}