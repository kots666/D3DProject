#include "stdafx.h"
#include "HitEffect.h"
#include "RCTex.h"
#include "Texture.h"

CHitEffect::CHitEffect(LPDIRECT3DDEVICE9 device) :
	Engine::CGameObject(device)
{

}

CHitEffect::CHitEffect(LPDIRECT3DDEVICE9 device, const _vec3 & pos, const _float& lifeTime) :
	Engine::CGameObject(device),
	m_pos(pos),
	m_lifeTime(lifeTime),
	m_accTime(0.f),
	m_isActive(false)
{
}

CHitEffect::~CHitEffect()
{

}

HRESULT Client::CHitEffect::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_offset = 1 / 3.f;

	m_step = 0;

	return S_OK;
}

Client::_int Client::CHitEffect::Update(const _float& deltaTime)
{
	if (m_isActive)
	{
		Engine::CGameObject::Update(deltaTime);

		m_accTime += deltaTime;

		if (m_accTime > m_lifeTime)
		{
			m_accTime = 0.f;
			m_isActive = false;
		}

		m_rendererCom->AddObject(Engine::RENDER_ALPHA, this);
	}

	return 0;
}

void Client::CHitEffect::Render()
{
	if (m_isActive)
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
	}
}

void CHitEffect::SetActive(const _vec3 & pos, const _float & lifeTime)
{
	m_pos = pos;
	m_lifeTime = lifeTime;
	m_isActive = true;
}

HRESULT Client::CHitEffect::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// buffer
	component = m_bufferCom = dynamic_cast<Engine::CRCTex*>(Engine::CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RCTex"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Buffer", component);

	// texture
	component = m_textureCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Texture_HitSlash"));
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
	component = m_shaderCom = dynamic_cast<Engine::CShader*>(Engine::CloneComp(L"Proto_Shader_HitEffect"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Shader", component);

	return S_OK;
}

HRESULT CHitEffect::SetUpConstantTable(LPD3DXEFFECT & effect)
{
	_matrix matWorld, matView, matProj;

	D3DXMatrixIdentity(&matWorld);

	m_device->GetTransform(D3DTS_VIEW, &matView);
	m_device->GetTransform(D3DTS_PROJECTION, &matProj);

	matWorld._41 = m_pos.x;
	matWorld._42 = m_pos.y;
	matWorld._43 = m_pos.z;

	effect->SetMatrix("g_matWorld", &matWorld);
	effect->SetMatrix("g_matView", &matView);
	effect->SetMatrix("g_matProj", &matProj);

	m_textureCom->SetTexture(effect, "g_BaseTexture");

	effect->SetInt("g_Step", m_step);
	effect->SetFloat("g_Offset", m_offset);

	return S_OK;
}

CHitEffect * CHitEffect::Create(LPDIRECT3DDEVICE9 device, const _vec3 & pos, const _float& lifeTime)
{
	CHitEffect* instance = new CHitEffect(device, pos, lifeTime);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CHitEffect::Free()
{
	Engine::CGameObject::Free();
}