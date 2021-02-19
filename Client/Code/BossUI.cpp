#include "stdafx.h"
#include "BossUI.h"
#include "RCTex.h"
#include "Texture.h"

CBossUI::CBossUI(LPDIRECT3DDEVICE9 device) :
	Engine::CGameObject(device)
{

}

CBossUI::CBossUI(LPDIRECT3DDEVICE9 device, const _tchar * texName, const _float & x, const _float & y, const _float & sizeX, const _float & sizeY) :
	Engine::CGameObject(device),
	m_texName(texName),
	m_wantedX(x), m_wantedY(y),
	m_sizeX(sizeX), m_sizeY(sizeY)
{
}

CBossUI::~CBossUI()
{

}

HRESULT Client::CBossUI::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_left = -(WINCX / 2.f) + (m_sizeX / 2.f) + m_wantedX;
	m_top = (WINCY / 2.f) - (m_sizeY / 2.f) - m_wantedY;

	return S_OK;
}

Client::_int Client::CBossUI::Update(const _float& deltaTime)
{
	if (nullptr == m_target)
	{
		m_isDead = true;
		return 0;
	}

	Engine::CGameObject::Update(deltaTime);

	m_rendererCom->AddObject(Engine::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_projMat, WINCX, WINCY, 0.f, 1.f);

	return 0;
}

void Client::CBossUI::Render()
{
	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;
	Engine::SafeAddRef(effect);

	FAILED_CHECK_RETURN(SetUpConstantTable(effect), );

	effect->Begin(nullptr, 0);
	effect->BeginPass(1);

	m_bufferCom->Render();

	effect->EndPass();
	effect->End();

	Engine::SafeRelease(effect);
}

void CBossUI::SetTarget(Engine::CGameObject * target, _int * hp, _int * maxHp)
{
	m_target = target;
	Engine::SafeAddRef(m_target);

	m_hp = hp;
	m_maxHp = maxHp;
}

HRESULT Client::CBossUI::AddComponent()
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

HRESULT CBossUI::SetUpConstantTable(LPD3DXEFFECT & effect)
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
	_float percent = *m_hp / (_float)*m_maxHp;
	effect->SetFloat("g_percent", percent);

	m_textureCom->SetTexture(effect, "g_BaseTexture");

	effect->CommitChanges();

	return S_OK;
}

CBossUI * CBossUI::Create(LPDIRECT3DDEVICE9 device, const _tchar * texName, const _float & x, const _float & y, const _float & sizeX, const _float & sizeY)
{
	CBossUI* instance = new CBossUI(device, texName, x, y, sizeX, sizeY);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CBossUI::Free()
{
	Engine::SafeRelease(m_target);
	Engine::CGameObject::Free();
}