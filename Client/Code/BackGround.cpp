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
	m_texCom->RenderTexture(0);
	m_bufferCom->Render();
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