#include "stdafx.h"
#include "TestMonster.h"
#include "RCTex.h"
#include "Texture.h"
#include "Transform.h"
#include "TestPlayer.h"

CTestMonster::CTestMonster(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* player)
	: Engine::CGameObject(pGraphicDev),
	m_player(player)
{

}

CTestMonster::~CTestMonster(void)
{

}

HRESULT Client::CTestMonster::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRCTex*>(Engine::CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_LOGO, L"Texture_Monster"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::CloneComp(L"Proto_Transform_Monster"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Transform_Monster", pComponent);

	m_pTransformCom->m_info[Engine::INFO_POS] = { 0.f, -10.f, 0.f };

	return S_OK;
}

void Client::CTestMonster::Chase(const _float& fTimeDelta)
{
	_vec3 up;
	
	m_pTransformCom->GetInfo(Engine::INFO_UP, &up);

	_vec3 targetPos = player->m_pTransformCom->m_info[Engine::INFO_POS];
	_vec3 myPos = m_pTransformCom->m_info[Engine::INFO_POS];
	_vec3 look = _vec3{ 0.f, 1.f, 0.f };

	_vec3 myDir = { m_pTransformCom->m_matWorld.m[1][0],  m_pTransformCom->m_matWorld.m[1][1] , m_pTransformCom->m_matWorld.m[1][2] };
	_vec3 targetDir = targetPos - myPos;

	cout << myDir.x << "\t" << myDir.y << "\t" << myDir.z << endl;

	D3DXVec3Normalize(&targetDir, &targetDir);

	_float radianZ = atan2f(targetDir.y, targetDir.x);

	_float radianX = atan2f(targetDir.z, targetDir.y);

	if (targetPos.y < myPos.y)
	{
		m_pTransformCom->m_angle.z = radianZ + D3DXToRadian(90.f);
		m_pTransformCom->m_angle.x = radianX;
	}
	else
	{
		m_pTransformCom->m_angle.z = radianZ + D3DXToRadian(-90.f);
		m_pTransformCom->m_angle.x = radianX;
	}

	m_pTransformCom->m_info[Engine::INFO_POS] += myDir * 3.f * fTimeDelta;
}

CTestMonster* CTestMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* player)
{
	CTestMonster*	pInstance = new CTestMonster(pGraphicDev, player);

	if (FAILED(pInstance->Ready()))
		Client::SafeRelease(pInstance);

	return pInstance;
}

void CTestMonster::Free(void)
{
	Engine::CGameObject::Free();
}


HRESULT Client::CTestMonster::Ready(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	player = dynamic_cast<CTestPlayer*>(m_player);
	if (nullptr == player) return E_FAIL;

	return S_OK;
}
Client::_int Client::CTestMonster::Update(const _float& fTimeDelta)
{
	Engine::CGameObject::Update(fTimeDelta);

	Engine::CTransform*	pPlayerTransform = dynamic_cast<Engine::CTransform*>(Engine::GetComponent(L"Environment", L"TestPlayer", L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransform, -1);

	_vec3 vPlayerPos;
	pPlayerTransform->GetInfo(Engine::INFO_POS, &vPlayerPos);

	m_pTransformCom->ChaseTarget(&vPlayerPos, m_fSpeed, fTimeDelta);

	return 0;
}
void Client::CTestMonster::Render(void)
{
	m_pTransformCom->SetTransform(m_device);
	m_pTextureCom->RenderTexture(0);
	m_pBufferCom->Render();

}
