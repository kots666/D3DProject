#include "stdafx.h"
#include "Enemy.h"
#include "RCTex.h"
#include "Texture.h"
#include "Transform.h"
#include "Calculator.h"

CEnemy::CEnemy(LPDIRECT3DDEVICE9 device) :
	Engine::CGameObject(device)
{

}

CEnemy::~CEnemy()
{

}

HRESULT Client::CEnemy::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_transCom->SetPos(rand() % 10 + 1, rand() % 10 + 1, rand() % 10 + 1);

	return S_OK;
}

Client::_int Client::CEnemy::Update(const _float& deltaTime)
{
	Engine::CGameObject::Update(deltaTime);

	m_rendererCom->AddObject(Engine::RENDER_NONALPHA, this);

	return 0;
}

void Client::CEnemy::Render()
{
	SetBillboardMatrix();

	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_texCom->RenderTexture(0);
	m_bufferCom->Render();

	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CEnemy::SetBillboardMatrix()
{
	_matrix view;

	m_device->GetTransform(D3DTS_VIEW, &view);
	memset(&view.m[3][0], 0, sizeof(_vec3));
	D3DXMatrixInverse(&view, nullptr, &view);
	
	_vec3 pos;
	m_transCom->GetInfo(Engine::INFO_POS, &pos);

	memcpy(&view.m[3][0], &pos, sizeof(_vec3));

	m_device->SetTransform(D3DTS_WORLD, &view);
}

HRESULT Client::CEnemy::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// buffer
	component = m_bufferCom = dynamic_cast<Engine::CRCTex*>(Engine::CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RCTex"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Buffer", component);

	// texture
	component = m_texCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Texture_Enemy"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Texture", component);

	// Transform
	component = m_transCom = dynamic_cast<Engine::CTransform*>(Engine::CloneComp(L"Proto_Transform"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", component);

	// Renderer
	component = m_rendererCom = Engine::GetRenderer();
	NULL_CHECK_RETURN(component, E_FAIL);
	Engine::SafeAddRef(component);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Renderer", component);

	return S_OK;
}

CEnemy* CEnemy::Create(LPDIRECT3DDEVICE9 device)
{
	CEnemy* instance = new CEnemy(device);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CEnemy::Free()
{
	Engine::CGameObject::Free();
}