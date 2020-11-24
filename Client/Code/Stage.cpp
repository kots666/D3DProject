#include "stdafx.h"
#include "Stage.h"
#include "Terrain.h"
#include "Transform.h"
#include "Player.h"
#include "DynamicCamera.h"
#include "Calculator.h"
#include "SkyBox.h"
#include "Enemy.h"
#include "Stone.h"
#include "Sword.h"

CStage::CStage(LPDIRECT3DDEVICE9 device) :
	Engine::CScene(device)
{

}

CStage::~CStage()
{

}

HRESULT CStage::Ready()
{
	FAILED_CHECK_RETURN(ReadyEnvironmentLayer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(ReadyGameLogicLayer(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(ReadyLightInfo(), E_FAIL);

	return S_OK;
}

_int CStage::Update(const _float& deltaTime)
{
	return Engine::CScene::Update(deltaTime);
}

void CStage::Render()
{
	Engine::CScene::Render();
	//m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT CStage::ReadyEnvironmentLayer(const _tchar * layerTag)
{
	Engine::CLayer* layer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(layer, E_FAIL);

	Engine::CGameObject* gameObject = CDynamicCamera::Create(m_device, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(gameObject, E_FAIL);
	FAILED_CHECK_RETURN(layer->AddGameObject(L"DynamicCamera", gameObject));

	gameObject = CSkyBox::Create(m_device);
	NULL_CHECK_RETURN(gameObject, E_FAIL);
	FAILED_CHECK_RETURN(layer->AddGameObject(L"SkyBox", gameObject), E_FAIL);

	gameObject = CTerrain::Create(m_device);
	NULL_CHECK_RETURN(gameObject, E_FAIL);
	FAILED_CHECK_RETURN(layer->AddGameObject(L"Terrain", gameObject), E_FAIL);

	m_layerMap.emplace(layerTag, layer);

	return S_OK;
}

HRESULT CStage::ReadyGameLogicLayer(const _tchar * layerTag)
{
	Engine::CLayer* layer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(layer, E_FAIL);

	Engine::CGameObject* gameObject = nullptr;

	gameObject = CStone::Create(m_device);
	NULL_CHECK_RETURN(gameObject, E_FAIL);
	FAILED_CHECK_RETURN(layer->AddGameObject(L"Stone", gameObject), E_FAIL);

	/*
	gameObject = CPlayer::Create(m_device);
	NULL_CHECK_RETURN(gameObject, E_FAIL);
	FAILED_CHECK_RETURN(layer->AddGameObject(L"Player", gameObject), E_FAIL);

	gameObject = CSword::Create(m_device);
	NULL_CHECK_RETURN(gameObject, E_FAIL);
	FAILED_CHECK_RETURN(layer->AddGameObject(L"Sword", gameObject), E_FAIL);
	*/

	m_layerMap.emplace(layerTag, layer);

	return S_OK;
}

HRESULT CStage::ReadyUILayer(const _tchar * layerTag)
{
	return S_OK;
}

HRESULT CStage::ReadyLightInfo()
{
	D3DLIGHT9 lightInfo;
	ZeroMemory(&lightInfo, sizeof(D3DLIGHT9));

	lightInfo.Type = D3DLIGHT_DIRECTIONAL;

	lightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	lightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	lightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	lightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(Engine::ReadyLight(m_device, &lightInfo, 0)))
		return E_FAIL;

	return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 device)
{
	CStage*	instance = new CStage(device);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CStage::Free()
{
	Engine::CScene::Free();
}

