#include "stdafx.h"
#include "FontUI.h"
#include "RCTex.h"
#include "Texture.h"

CFontUI::CFontUI(LPDIRECT3DDEVICE9 device) :
	Engine::CGameObject(device)
{

}

CFontUI::CFontUI(LPDIRECT3DDEVICE9 device, const _tchar * texName, const _int& index, const _float & sizeX, const _float & sizeY) :
	Engine::CGameObject(device),
	m_texName(texName),
	m_index(index),
	m_sizeX(sizeX),
	m_sizeY(sizeY)
{
}

CFontUI::~CFontUI()
{

}

HRESULT Client::CFontUI::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_accTime = 0.f;
	m_lifeTime = 2.f;

	m_isActive = false;

	return S_OK;
}

Client::_int Client::CFontUI::Update(const _float& deltaTime)
{
	if (m_isActive)
	{
		Engine::CGameObject::Update(deltaTime);

		m_startPos.y += deltaTime * 2.f;

		m_accTime += deltaTime;

		if (m_accTime > m_lifeTime)
		{
			m_isActive = false;
			m_accTime = 0.f;
			m_lifeTime = 0.f;
		}
		else
			m_rendererCom->AddObject(Engine::RENDER_UI, this);
	}

	return 0;
}

void Client::CFontUI::Render()
{
	if (m_isActive)
	{
		LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
		if (nullptr == effect) return;
		Engine::SafeAddRef(effect);

		if (FAILED(SetUpConstantTable(effect)))
		{
			Engine::SafeRelease(effect);
			return;
		}

		effect->Begin(nullptr, 0);
		effect->BeginPass(3);

		m_bufferCom->Render();

		effect->EndPass();
		effect->End();

		Engine::SafeRelease(effect);
	}
}

void CFontUI::Active(const _vec3 & pos, const _vec3 & offset, const _float & lifeTime, const _float & xSize, const _float & ySize)
{
	if (!m_isActive)
	{
		m_startPos = pos;
		m_offset = offset;
		m_lifeTime = lifeTime;
		m_sizeX = xSize;
		m_sizeY = ySize;
		m_isActive = true;
	}
}

HRESULT Client::CFontUI::AddComponent()
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

HRESULT CFontUI::SetUpConstantTable(LPD3DXEFFECT & effect)
{
	_matrix worldMat, viewMat;

	D3DXMatrixIdentity(&worldMat);
	D3DXMatrixIdentity(&viewMat);

	_matrix originViewMat, originProjMat;
	m_device->GetTransform(D3DTS_VIEW, &originViewMat);
	m_device->GetTransform(D3DTS_PROJECTION, &originProjMat);

	m_percent = (m_lifeTime - m_accTime) / m_lifeTime;

	viewMat = originViewMat;
	viewMat._41 = 0;
	viewMat._42 = 0;
	viewMat._43 = 0;

	D3DXMatrixIdentity(&worldMat);

	worldMat._41 = m_offset.x;
	worldMat._42 = m_offset.y;
	worldMat._43 = m_offset.z;

	D3DXMatrixInverse(&viewMat, 0, &viewMat);

	viewMat._41 = m_startPos.x;
	viewMat._42 = m_startPos.y + 2.f;
	viewMat._43 = m_startPos.z;

	viewMat._11 *= m_sizeX;
	viewMat._12 *= m_sizeX;
	viewMat._13 *= m_sizeX;

	viewMat._21 *= m_sizeY;
	viewMat._22 *= m_sizeY;
	viewMat._23 *= m_sizeY;

	worldMat *= viewMat;

	effect->SetMatrix("g_matWorld", &worldMat);
	effect->SetMatrix("g_matView", &originViewMat);
	effect->SetMatrix("g_matProj", &originProjMat);
	effect->SetFloat("g_percent", m_percent);

	m_textureCom->SetTexture(effect, "g_BaseTexture", m_index);

	return S_OK;
}

CFontUI * CFontUI::Create(LPDIRECT3DDEVICE9 device, const _tchar * texName, const _int& index, const _float & sizeX, const _float & sizeY)
{
	CFontUI* instance = new CFontUI(device, texName, index, sizeX, sizeY);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CFontUI::Free()
{
	Engine::CGameObject::Free();
}