#include "stdafx.h"
#include "TestPlayer.h"
#include "RCTex.h"
#include "Texture.h"
#include "Transform.h"

CTestPlayer::CTestPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTestPlayer::~CTestPlayer(void)
{

}

HRESULT Client::CTestPlayer::AddComponent(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRCTex*>(Engine::CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_LOGO, L"Texture_Player"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Transform
	pComponent = m_transCom = dynamic_cast<Engine::CTransform*>(Engine::CloneComp(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_compMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	return S_OK;
}

void Client::CTestPlayer::Key_Input(const _float& fTimeDelta)
{
	_matrix mat = m_transCom->m_matWorld;
	_vec3 dir = { mat.m[1][0], mat.m[1][1], mat.m[1][2] };

	if (GetAsyncKeyState('Q') & 0x8000)
	{
		m_transCom->m_angle.x += D3DXToRadian(180.f * fTimeDelta);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_transCom->m_angle.x -= D3DXToRadian(180.f * fTimeDelta);
	}
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_transCom->m_angle.y += D3DXToRadian(180.f * fTimeDelta);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_transCom->m_angle.y -= D3DXToRadian(180.f * fTimeDelta);
	}
	if (GetAsyncKeyState('E') & 0x8000)
	{
		m_transCom->m_angle.z += D3DXToRadian(180.f * fTimeDelta);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_transCom->m_angle.z -= D3DXToRadian(180.f * fTimeDelta);
	}

	if (GetAsyncKeyState(VK_UP) & 0X8000)
	{
		m_transCom->m_info[Engine::INFO_POS] += dir * 5.f * fTimeDelta;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0X8000)
	{
		m_transCom->m_info[Engine::INFO_POS] -= dir * 5.f * fTimeDelta;
	}
}

CTestPlayer* CTestPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestPlayer*	pInstance = new CTestPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::SafeRelease(pInstance);

	return pInstance;
}

void CTestPlayer::Free(void)
{
	Engine::CGameObject::Free();
}


HRESULT Client::CTestPlayer::Ready(void)
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	//m_pTransformCom->m_vScale = _vec3(4.f, 2.f, 1.f);

	return S_OK;
}
Client::_int Client::CTestPlayer::Update(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
	Engine::CGameObject::Update(fTimeDelta);

	return 0;
}
void Client::CTestPlayer::Render(void)
{
	m_transCom->SetTransform(m_device);
	m_pTextureCom->RenderTexture(0);
	m_pBufferCom->Render();

}
