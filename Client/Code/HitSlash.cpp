#include "stdafx.h"
#include "HitSlash.h"
#include "RCTex.h"
#include "Texture.h"

CHitSlash::CHitSlash(LPDIRECT3DDEVICE9 device) :
	Engine::CGameObject(device)
{

}

CHitSlash::CHitSlash(LPDIRECT3DDEVICE9 device, const _vec3 & pos, const _float& lifeTime) :
	Engine::CGameObject(device),
	m_pos(pos),
	m_lifeTime(lifeTime),
	m_accTime(0.f),
	m_isActive(false)
{
}

CHitSlash::~CHitSlash()
{

}

HRESULT Client::CHitSlash::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_offset = 1 / 3.f;
	m_size = 1.f;
	m_step = 0;

	return S_OK;
}

Client::_int Client::CHitSlash::Update(const _float& deltaTime)
{
	if (m_isActive)
	{
		Engine::CGameObject::Update(deltaTime);

		m_accTime += deltaTime;

		m_step = (m_accTime / m_lifeTime) * 9.f;

		if (m_accTime > m_lifeTime)
		{
			m_accTime = 0.f;
			m_isActive = false;
		}

		m_rendererCom->AddObject(Engine::RENDER_ALPHA, this);
	}

	return 0;
}

void Client::CHitSlash::Render()
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

void CHitSlash::SetActive(const _vec3 & pos, const _float& size, const _float & lifeTime)
{
	m_pos = pos;
	m_size = size;
	m_lifeTime = lifeTime;
	m_isActive = true;
}

HRESULT Client::CHitSlash::AddComponent()
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

HRESULT CHitSlash::SetUpConstantTable(LPD3DXEFFECT & effect)
{
	_matrix matWorld, matView, matProj;

	D3DXMatrixIdentity(&matWorld);

	m_device->GetTransform(D3DTS_VIEW, &matView);
	m_device->GetTransform(D3DTS_PROJECTION, &matProj);

	matWorld = matView;

	matWorld._41 = 0.f;
	matWorld._42 = 0.f;
	matWorld._43 = 0.f;

	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);

	for (_int i = 0; i < 3; ++i)
	{
		for (_int j = 0; j < 4; ++j)
		{
			matWorld.m[i][j] *= m_size;
		}
	}

	matWorld._41 = m_pos.x;
	matWorld._42 = m_pos.y;
	matWorld._43 = m_pos.z;

	effect->SetMatrix("g_matWorld", &matWorld);
	effect->SetMatrix("g_matView", &matView);
	effect->SetMatrix("g_matProj", &matProj);

	m_textureCom->SetTexture(effect, "g_BaseTexture");

	effect->SetInt("g_Step", m_step);
	effect->SetFloat("g_Offset", 0.2f);

	_vec4 color = { 3.f, 0.f, 0.f, 1.f };
	effect->SetVector("g_Color", &color);

	effect->CommitChanges();

	return S_OK;
}

CHitSlash * CHitSlash::Create(LPDIRECT3DDEVICE9 device, const _vec3 & pos, const _float& lifeTime)
{
	CHitSlash* instance = new CHitSlash(device, pos, lifeTime);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CHitSlash::Free()
{
	Engine::CGameObject::Free();
}