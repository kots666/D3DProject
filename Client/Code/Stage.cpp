#include "stdafx.h"
#include "Stage.h"
#include "Terrain.h"
#include "Transform.h"
#include "Player.h"
#include "DynamicCamera.h"
#include "Calculator.h"
#include "SkyBox.h"
#include "GoblinSword.h"
#include "Stone.h"
#include "Sword.h"
#include "BackMesh.h"
#include "Boss.h"
#include "UI.h"
#include "HPUI.h"
#include "Spawner.h"
#include "NPC.h"

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
	FAILED_CHECK_RETURN(ReadyUILayer(L"UI"), E_FAIL);
	FAILED_CHECK_RETURN(ReadyLightInfo(), E_FAIL);

	return S_OK;
}

_int CStage::Update(const _float& deltaTime)
{
	return Engine::CScene::Update(deltaTime);
}

void CStage::Render()
{
	//Engine::CScene::Render();
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

	HANDLE hFile = CreateFile(L"../Data/Map.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwByte = 0;
	_int cnt = 0;

	while (true)
	{
		_tchar fileTag[30] = L"";

		_vec3 pos;
		_vec3 scale;
		_vec3 rotation;

		ReadFile(hFile, &fileTag, sizeof(fileTag), &dwByte, nullptr);
		ReadFile(hFile, &pos, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &scale, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &rotation, sizeof(_vec3), &dwByte, nullptr);

		if (0 == dwByte) break;

		_tchar number[20] = L"";
		swprintf(number, sizeof(_tchar) * 20, L"%d", cnt);

		_tchar* name = new _tchar[50];
		lstrcpy(name, fileTag);
		lstrcat(name, number);

		gameObject = CBackMesh::Create(m_device, fileTag, name, pos, scale, rotation);
		NULL_CHECK_RETURN(gameObject, E_FAIL);
		FAILED_CHECK_RETURN(layer->AddGameObject(name, gameObject), E_FAIL);
	}

	CloseHandle(hFile);

	m_layerMap.emplace(layerTag, layer);

	return S_OK;
}

HRESULT CStage::ReadyGameLogicLayer(const _tchar * layerTag)
{
	Engine::CLayer* layer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(layer, E_FAIL);

	Engine::CGameObject* gameObject = nullptr;

	gameObject = CPlayer::Create(m_device);
	NULL_CHECK_RETURN(gameObject, E_FAIL);
	FAILED_CHECK_RETURN(layer->AddGameObject(L"Player", gameObject), E_FAIL);

	gameObject = CNPC::Create(m_device, { 12.f, 0.25f, 8.f }, -180.f);
	NULL_CHECK_RETURN(gameObject, E_FAIL);
	FAILED_CHECK_RETURN(layer->AddGameObject(L"NPC", gameObject));

	gameObject = CSpawner::Create(m_device, { 7.f, 0.f, 8.f }, 400);
	NULL_CHECK_RETURN(gameObject, E_FAIL);
	FAILED_CHECK_RETURN(layer->AddGameObject(L"Spawner01", gameObject), E_FAIL);

	gameObject = CSpawner::Create(m_device, { 13.5f, 0.f, 44.f }, 200);
	NULL_CHECK_RETURN(gameObject, E_FAIL);
	FAILED_CHECK_RETURN(layer->AddGameObject(L"Spawner02", gameObject), E_FAIL);

	gameObject = CSpawner::Create(m_device, { 44.f, 0.f, 66.5f }, 200);
	NULL_CHECK_RETURN(gameObject, E_FAIL);
	FAILED_CHECK_RETURN(layer->AddGameObject(L"Spawner03", gameObject), E_FAIL);

	/*

	gameObject = CSword::Create(m_device);
	NULL_CHECK_RETURN(gameObject, E_FAIL);
	FAILED_CHECK_RETURN(layer->AddGameObject(L"Sword", gameObject), E_FAIL);
	*/

	m_layerMap.emplace(layerTag, layer);

	return S_OK;
}

HRESULT CStage::ReadyUILayer(const _tchar * layerTag)
{
	Engine::CLayer* layer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(layer, E_FAIL);

	Engine::CGameObject* gameObject = nullptr;

	gameObject = CUI::Create(m_device, L"Texture_PlayerIcon", 20, 20, 112, 112);
	NULL_CHECK_RETURN(gameObject, E_FAIL);
	FAILED_CHECK_RETURN(layer->AddGameObject(L"UI_PlayerIcon", gameObject), E_FAIL);

	gameObject = CUI::Create(m_device, L"Texture_HPFrame", 124, 62, 179, 14);
	NULL_CHECK_RETURN(gameObject, E_FAIL);
	FAILED_CHECK_RETURN(layer->AddGameObject(L"UI_HPFrame", gameObject), E_FAIL);

	Engine::CGameObject* player = GetLayer(L"GameLogic")->GetGameObject(L"Player");

	gameObject = CHPUI::Create(m_device, player, L"Texture_HPBar", 124, 62, 179, 14);
	NULL_CHECK_RETURN(gameObject, E_FAIL);
	FAILED_CHECK_RETURN(layer->AddGameObject(L"UI_HPBar", gameObject), E_FAIL);

	m_layerMap.emplace(layerTag, layer);

	return S_OK;
}

HRESULT CStage::ReadyLightInfo()
{
	D3DLIGHT9 lightInfo;
	ZeroMemory(&lightInfo, sizeof(D3DLIGHT9));

	lightInfo.Type = D3DLIGHT_DIRECTIONAL;

	lightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	lightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	lightInfo.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);

	lightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(Engine::ReadyLight(m_device, &lightInfo, 0)))
		return E_FAIL;

	return S_OK;
}

void CStage::ReadMapFile(const _tchar * layerTag)
{

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

