#include "stdafx.h"
#include "InteractionUI.h"
#include "RCTex.h"
#include "Texture.h"

CInteractionUI::CInteractionUI(LPDIRECT3DDEVICE9 device) :
	Engine::CGameObject(device)
{

}

CInteractionUI::CInteractionUI(LPDIRECT3DDEVICE9 device, const _tchar * texName) :
	Engine::CGameObject(device),
	m_textureName(texName),
	m_isActive(false)
{
}

CInteractionUI::~CInteractionUI()
{

}

HRESULT Client::CInteractionUI::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	return S_OK;
}

Client::_int Client::CInteractionUI::Update(const _float& deltaTime)
{
	if (m_isActive)
	{
		Engine::CGameObject::Update(deltaTime);

		m_rendererCom->AddObject(Engine::RENDER_ALPHA, this);
	}

	return 0;
}

_int CInteractionUI::LateUpdate(const _float & deltaTime)
{
	if (m_isActive)
	{
		Engine::CGameObject::Update(deltaTime);

		m_rendererCom->AddObject(Engine::RENDER_ALPHA, this);
	}

	return 0;
}

void Client::CInteractionUI::Render()
{
	if (m_isActive)
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
}

void CInteractionUI::SetActive(const _vec3 & pos, const _float& xSize, const _float& ySize)
{
	m_pos = pos;
	m_size[0] = xSize;
	m_size[1] = ySize;
	m_isActive = true;
}

HRESULT Client::CInteractionUI::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// buffer
	component = m_bufferCom = dynamic_cast<Engine::CRCTex*>(Engine::CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RCTex"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Buffer", component);

	// texture
	component = m_textureCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_STAGE, m_textureName));
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

HRESULT CInteractionUI::SetUpConstantTable(LPD3DXEFFECT & effect)
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
			matWorld.m[i][j] *= m_size[i];
		}
	}

	matWorld._41 = m_pos.x;
	matWorld._42 = m_pos.y;
	matWorld._43 = m_pos.z;

	effect->SetMatrix("g_matWorld", &matWorld);
	effect->SetMatrix("g_matView", &matView);
	effect->SetMatrix("g_matProj", &matProj);

	m_textureCom->SetTexture(effect, "g_BaseTexture");

	effect->CommitChanges();

	return S_OK;
}

CInteractionUI * CInteractionUI::Create(LPDIRECT3DDEVICE9 device, const _tchar * texName)
{
	CInteractionUI* instance = new CInteractionUI(device, texName);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CInteractionUI::Free()
{
	Engine::CGameObject::Free();
}