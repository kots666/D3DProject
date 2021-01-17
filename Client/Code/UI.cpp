#include "stdafx.h"
#include "UI.h"
#include "RCTex.h"
#include "Texture.h"

CUI::CUI(LPDIRECT3DDEVICE9 device) :
	Engine::CGameObject(device)
{

}

CUI::CUI(LPDIRECT3DDEVICE9 device, const _tchar * texName, const _float & x, const _float & y, const _float & sizeX, const _float & sizeY) :
	Engine::CGameObject(device),
	m_texName(texName),
	m_wantedX(x), m_wantedY(y),
	m_sizeX(sizeX), m_sizeY(sizeY)
{
}

CUI::~CUI()
{

}

HRESULT Client::CUI::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_left = -(WINCX / 2.f) + (m_sizeX / 2.f) + m_wantedX;
	m_top = (WINCY / 2.f) - (m_sizeY / 2.f) - m_wantedY;

	return S_OK;
}

Client::_int Client::CUI::Update(const _float& deltaTime)
{
	Engine::CGameObject::Update(deltaTime);

	m_rendererCom->AddObject(Engine::RENDER_ALPHA, this);

	D3DXMatrixOrthoLH(&m_projMat, WINCX, WINCY, 0.f, 1.f);

	return 0;
}

void Client::CUI::Render()
{
	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;
	Engine::SafeAddRef(effect);

	FAILED_CHECK_RETURN(SetUpConstantTable(effect), );

	effect->Begin(nullptr, 0);
	effect->BeginPass(0);

	m_bufferCom->Render();

	effect->EndPass();
	effect->End();

	Engine::SafeRelease(effect);

	/*_matrix wolrdMat, viewMat, originViewMat, originProjMat;

	m_device->GetTransform(D3DTS_VIEW, &originViewMat);
	m_device->GetTransform(D3DTS_PROJECTION, &originProjMat);

	D3DXMatrixIdentity(&wolrdMat);
	D3DXMatrixIdentity(&viewMat);

	wolrdMat._11 = m_sizeX;
	wolrdMat._22 = m_sizeY;
	wolrdMat._33 = 1.f;
	wolrdMat._41 = m_left;
	wolrdMat._42 = m_top;

	m_device->SetTransform(D3DTS_WORLD, &wolrdMat);
	m_device->SetTransform(D3DTS_VIEW, &viewMat);
	m_device->SetTransform(D3DTS_PROJECTION, &m_projMat);
	m_device->SetRenderState(D3DRS_LIGHTING, false);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_textureCom->RenderTexture();
	m_bufferCom->Render();

	m_device->SetRenderState(D3DRS_LIGHTING, true);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_device->SetTransform(D3DTS_VIEW, &originViewMat);
	m_device->SetTransform(D3DTS_PROJECTION, &originProjMat);*/
}

HRESULT Client::CUI::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// buffer
	component = m_bufferCom = dynamic_cast<Engine::CRCTex*>(Engine::CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RCTex"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Buffer", component);

	// texture
	component = m_textureCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_STAGE, m_texName));
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

HRESULT CUI::SetUpConstantTable(LPD3DXEFFECT & effect)
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

	m_textureCom->SetTexture(effect, "g_BaseTexture");

	return S_OK;
}

CUI * CUI::Create(LPDIRECT3DDEVICE9 device, const _tchar * texName, const _float & x, const _float & y, const _float & sizeX, const _float & sizeY)
{
	CUI* instance = new CUI(device, texName, x, y, sizeX, sizeY);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CUI::Free()
{
	Engine::CGameObject::Free();
}