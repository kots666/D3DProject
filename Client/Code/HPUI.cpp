#include "stdafx.h"
#include "HPUI.h"
#include "RCTex.h"
#include "Texture.h"

CHPUI::CHPUI(LPDIRECT3DDEVICE9 device) :
	Engine::CGameObject(device)
{

}

CHPUI::CHPUI(LPDIRECT3DDEVICE9 device, Engine::CGameObject * target, const _tchar * texName, const _float & x, const _float & y, const _float & sizeX, const _float & sizeY, const _bool& isFixed, const _bool& isFloat) :
	Engine::CGameObject(device),
	m_target(target),
	m_texName(texName),
	m_wantedX(x), m_wantedY(y),
	m_sizeX(sizeX), m_sizeY(sizeY),
	m_isFixed(isFixed),
	m_isFloat(isFloat)
{
	Engine::SafeAddRef(m_target);
}

CHPUI::~CHPUI()
{

}

HRESULT Client::CHPUI::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_left = -(WINCX / 2.f) + (m_sizeX / 2.f) + m_wantedX;
	m_top = (WINCY / 2.f) - (m_sizeY / 2.f) - m_wantedY;

	m_maxHp = m_target->GetMaxHp();

	return S_OK;
}

Client::_int Client::CHPUI::Update(const _float& deltaTime)
{
	Engine::CGameObject::Update(deltaTime);

	if (m_isFixed)
		m_rendererCom->AddObject(Engine::RENDER_ALPHA, this);
	else
		m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	D3DXMatrixOrthoLH(&m_projMat, WINCX, WINCY, 0.f, 1.f);

	if (nullptr == m_target)
	{
		m_isDead = true;
		return 0;
	}

	if (m_target->GetIsDead())
	{
		m_isDead = true;
		return 0;
	}

	m_hp = m_target->GetHp();

	return 0;
}

void Client::CHPUI::Render()
{
	if (nullptr == m_target) return;

	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;
	Engine::SafeAddRef(effect);

	if (FAILED(SetUpConstantTable(effect)))
	{
		Engine::SafeRelease(effect);
		return;
	}

	effect->Begin(nullptr, 0);
	if (m_isFloat)
		effect->BeginPass(1);
	else if (m_isFixed)
		effect->BeginPass(1);
	else
		effect->BeginPass(2);

	m_bufferCom->Render();

	effect->EndPass();
	effect->End();

	Engine::SafeRelease(effect);
}

HRESULT Client::CHPUI::AddComponent()
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

HRESULT CHPUI::SetUpConstantTable(LPD3DXEFFECT & effect)
{
	_matrix worldMat, viewMat;

	D3DXMatrixIdentity(&worldMat);
	D3DXMatrixIdentity(&viewMat);

	if (m_isFixed)
	{
		m_percent = (m_hp / m_maxHp);

		worldMat._11 = m_sizeX;
		worldMat._22 = m_sizeY;
		worldMat._33 = 1.f;
		worldMat._41 = m_left;
		worldMat._42 = m_top;

		// worldMat
		// viewMat
		// m_projMat
		effect->SetMatrix("g_matWorld", &worldMat);
		effect->SetMatrix("g_matView", &viewMat);
		effect->SetMatrix("g_matProj", &m_projMat);
		effect->SetFloat("g_percent", m_percent);

		m_textureCom->SetTexture(effect, "g_BaseTexture");
	}
	else
	{
		Engine::CTransform* targetTransform = dynamic_cast<Engine::CTransform*>(m_target->GetComponent(L"Com_Transform", Engine::ID_DYNAMIC));
		if (nullptr == targetTransform) return E_FAIL;

		_matrix originViewMat, originProjMat;
		m_device->GetTransform(D3DTS_VIEW, &originViewMat);
		m_device->GetTransform(D3DTS_PROJECTION, &originProjMat);

		_vec3 targetPos;
		targetTransform->GetInfo(Engine::INFO_POS, &targetPos);

		m_percent = (m_hp / m_maxHp);

		worldMat._11 = m_sizeX;
		worldMat._22 = m_sizeY;
		worldMat._33 = 1.f;
		worldMat._41 = targetPos.x;
		worldMat._42 = targetPos.y + 10.f;
		worldMat._43 = targetPos.z;

		viewMat = originViewMat;
		viewMat._41 = 0;
		viewMat._42 = 0;
		viewMat._43 = 0;

		D3DXMatrixInverse(&viewMat, 0, &viewMat);

		viewMat._41 = targetPos.x;
		viewMat._42 = targetPos.y + 2.f;
		viewMat._43 = targetPos.z;

		viewMat._11 *= m_sizeX;
		viewMat._12 *= m_sizeX;
		viewMat._13 *= m_sizeX;

		viewMat._21 *= m_sizeY;
		viewMat._22 *= m_sizeY;
		viewMat._23 *= m_sizeY;

		worldMat = viewMat;

		// worldMat
		// originViewMat
		// originProjMat

		effect->SetMatrix("g_matWorld", &worldMat);
		effect->SetMatrix("g_matView", &originViewMat);
		effect->SetMatrix("g_matProj", &originProjMat);
		effect->SetFloat("g_percent", m_percent);

		m_textureCom->SetTexture(effect, "g_BaseTexture");
	}

	return S_OK;
}

CHPUI * CHPUI::Create(LPDIRECT3DDEVICE9 device, Engine::CGameObject * target, const _tchar * texName, const _float & x, const _float & y, const _float & sizeX, const _float & sizeY, const _bool & isFixed, const _bool & isFloat)
{
	CHPUI* instance = new CHPUI(device, target, texName, x, y, sizeX, sizeY, isFixed, isFloat);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CHPUI::Free()
{
	Engine::SafeRelease(m_target);

	Engine::CGameObject::Free();
}