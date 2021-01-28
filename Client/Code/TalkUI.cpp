#include "stdafx.h"
#include "TalkUI.h"
#include "RCTex.h"
#include "Texture.h"
#include "Shader.h"

CTalkUI::CTalkUI(LPDIRECT3DDEVICE9 device) :
	Engine::CGameObject(device)
{

}

CTalkUI::CTalkUI(LPDIRECT3DDEVICE9 device, const _int& idx, const _float & x, const _float & y, const _float & sizeX, const _float & sizeY) :
	Engine::CGameObject(device),
	m_index(idx),
	m_wantedX(x), m_wantedY(y),
	m_sizeX(sizeX), m_sizeY(sizeY)
{
}

CTalkUI::~CTalkUI()
{

}

HRESULT Client::CTalkUI::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_left = -(WINCX / 2.f) + (m_sizeX / 2.f) + m_wantedX;
	m_top = (WINCY / 2.f) - (m_sizeY / 2.f) - m_wantedY;

	return S_OK;
}

Client::_int Client::CTalkUI::Update(const _float& deltaTime)
{
	Engine::CGameObject::Update(deltaTime);

	m_rendererCom->AddObject(Engine::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_projMat, WINCX, WINCY, 0.f, 1.f);

	return 0;
}

void Client::CTalkUI::Render()
{
	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;
	Engine::SafeAddRef(effect);

	FAILED_CHECK_RETURN(SetUpConstantTable(effect), );

	effect->Begin(nullptr, 0);
	effect->BeginPass(4);

	m_bufferCom->Render();

	effect->EndPass();
	effect->End();

	Engine::SafeRelease(effect);
}

HRESULT Client::CTalkUI::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// buffer
	component = m_bufferCom = dynamic_cast<Engine::CRCTex*>(Engine::CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RCTex"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Buffer", component);

	// texture
	component = m_textureCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Texture_Talk"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Texture", component);

	// Renderer
	component = m_rendererCom = Engine::GetRenderer();
	NULL_CHECK_RETURN(component, E_FAIL);
	SafeAddRef(component);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Renderer", component);

	// Transform
	component = m_transformCom = dynamic_cast<Engine::CTransform*>(Engine::CloneComp(L"Proto_Transform"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", component);

	// Shader
	component = m_shaderCom = dynamic_cast<Engine::CShader*>(Engine::CloneComp(L"Proto_Shader_UI"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Shader", component);

	return S_OK;
}

HRESULT CTalkUI::SetUpConstantTable(LPD3DXEFFECT & effect)
{
	_matrix matWorld, matView;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._11 = m_sizeX;
	matWorld._22 = m_sizeY;
	matWorld._33 = 1.f;
	matWorld._41 = m_left;
	matWorld._42 = m_top;

	effect->SetMatrix("g_matWorld", &matWorld);
	effect->SetMatrix("g_matView", &matView);
	effect->SetMatrix("g_matProj", &m_projMat);

	m_textureCom->SetTexture(effect, "g_BaseTexture", m_index);

	effect->CommitChanges();

	return S_OK;
}

CTalkUI * CTalkUI::Create(LPDIRECT3DDEVICE9 device, const _int& idx, const _float & x, const _float & y, const _float & sizeX, const _float & sizeY)
{
	CTalkUI* instance = new CTalkUI(device, idx, x, y, sizeX, sizeY);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CTalkUI::Free()
{
	Engine::CGameObject::Free();
}