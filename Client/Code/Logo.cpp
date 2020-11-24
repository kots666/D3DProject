#include "stdafx.h"
#include "Logo.h"
#include "BackGround.h"
#include "Transform.h"
#include "Loading.h"
#include "Stage.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 device) :
	Engine::CScene(device)
{

}

CLogo::~CLogo()
{
	
}

HRESULT CLogo::Ready()
{
	FAILED_CHECK_RETURN(InitResource(Engine::RESOURCE_END), E_FAIL);
	FAILED_CHECK_RETURN(ReadyEnvironmentLayer(L"Environment"), E_FAIL);

	m_loading = CLoading::Create(m_device, CLoading::LOADING_STAGE);
	NULL_CHECK_RETURN(m_loading, E_FAIL);

	return S_OK;
}

_int CLogo::Update(const _float& deltaTime)
{
	_int ret = Engine::CScene::Update(deltaTime);

	if (true == m_loading->GetIsFinish())
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			CScene* scene = nullptr;

			scene = CStage::Create(m_device);

			FAILED_CHECK_RETURN(Engine::SetUpScene(scene), E_FAIL);

			return ret;
		}
	}

	return ret;
}

void CLogo::Render()
{
	Engine::RenderFont(L"Font_Jinji", m_loading->GetLoadString(), &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
}

HRESULT CLogo::ReadyEnvironmentLayer(const _tchar * layerTag)
{
	Engine::CLayer* layer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(layer, E_FAIL);

	Engine::CGameObject* gameObject = nullptr;

	gameObject = CBackGround::Create(m_device);
	NULL_CHECK_RETURN(gameObject, E_FAIL);
	FAILED_CHECK_RETURN(layer->AddGameObject(L"BackGround", gameObject), E_FAIL);

	m_layerMap.emplace(layerTag, layer);

	return S_OK;
}

HRESULT CLogo::ReadyGameLogicLayer(const _tchar * layerTag)
{
	return S_OK;
}

HRESULT CLogo::ReadyUILayer(const _tchar * layerTag)
{
	return S_OK;
}

HRESULT CLogo::InitResource(Engine::RESOURCETYPE type)
{
	FAILED_CHECK_RETURN(Engine::ReserveContainerSize(type), E_FAIL);
	FAILED_CHECK_RETURN(Engine::ReadyBuffer(m_device, Engine::RESOURCE_STATIC, L"Buffer_RCTex", Engine::BUFFER_RCTEX), E_FAIL);
	FAILED_CHECK_RETURN(Engine::ReadyTexture(m_device, Engine::RESOURCE_LOGO, L"Texture_Logo", Engine::TEX_NORMAL, L"../Resource/Texture/Logo/Logo.jpg"), E_FAIL);

	return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 device)
{
	CLogo* instance = new CLogo(device);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CLogo::Free()
{
	Engine::CScene::Free();
	SafeRelease(m_loading);
}

