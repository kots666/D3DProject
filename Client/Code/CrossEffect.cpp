#include "stdafx.h"
#include "CrossEffect.h"
#include "CrossTex.h"
#include "Texture.h"

CCrossEffect::CCrossEffect(LPDIRECT3DDEVICE9 device) :
	Engine::CGameObject(device)
{

}

CCrossEffect::CCrossEffect(LPDIRECT3DDEVICE9 device, const _vec4 & color, const _tchar * texName, const _int & uCnt, const _int & vCnt, const _bool & isRandRot, const _int & passIndex, const _float & lifeTime) :
	Engine::CGameObject(device),
	m_color(color),
	m_textureName(texName),
	m_uCnt(uCnt), m_vCnt(vCnt),
	m_isRandomRotation(isRandRot),
	m_passIndex(passIndex),
	m_lifeTime(lifeTime),
	m_accTime(0.f),
	m_isActive(false),
	m_rotAngle(0)
{
}

CCrossEffect::~CCrossEffect()
{

}

HRESULT Client::CCrossEffect::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_uOffset = 1.f / m_uCnt;
	m_vOffset = 1.f / m_vCnt;

	m_step = 0;

	RandRotMatrix();

	return S_OK;
}

Client::_int Client::CCrossEffect::Update(const _float& deltaTime)
{
	if (m_isActive)
	{
		Engine::CGameObject::Update(deltaTime);

		m_accTime += deltaTime;

		m_step = (m_accTime / m_lifeTime) * m_uCnt * m_vCnt;

		m_uStep = m_step % m_uCnt;
		m_vStep = m_step / m_uCnt;

		if (m_accTime > m_lifeTime)
		{
			m_accTime = 0.f;
			m_isActive = false;
		}

		m_rendererCom->AddObject(Engine::RENDER_ALPHA, this);
	}

	return 0;
}

void Client::CCrossEffect::Render()
{
	if (m_isActive)
	{
		LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
		if (nullptr == effect) return;

		Engine::SafeAddRef(effect);

		FAILED_CHECK_RETURN(SetUpConstantTable(effect), );

		effect->Begin(nullptr, 0);
		effect->BeginPass(m_passIndex);

		m_bufferCom->Render();

		effect->EndPass();
		effect->End();

		Engine::SafeRelease(effect);
	}
}

void CCrossEffect::SetActive(const _vec3 & pos, const _float& xSize, const _float& ySize, const _float & lifeTime)
{
	m_pos = pos;
	m_transformCom->SetPos(pos);
	m_transformCom->SetScale(xSize, ySize, 1.f);

	m_size[0] = xSize;
	m_size[1] = ySize;

	m_accTime = 0.f;
	m_step = 0;
	m_lifeTime = lifeTime;
	m_isActive = true;

	RandRotMatrix();
}

HRESULT Client::CCrossEffect::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// buffer
	component = m_bufferCom = dynamic_cast<Engine::CCrossTex*>(Engine::CloneResource(Engine::RESOURCE_STATIC, L"Buffer_CrossTex"));
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
	component = m_shaderCom = dynamic_cast<Engine::CShader*>(Engine::CloneComp(L"Proto_Shader_HitEffect"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Shader", component);

	return S_OK;
}

HRESULT CCrossEffect::SetUpConstantTable(LPD3DXEFFECT & effect)
{
	_matrix matWorld, matBill, matView, matProj;
	D3DXMatrixIdentity(&matBill);

	m_device->GetTransform(D3DTS_VIEW, &matView);
	m_device->GetTransform(D3DTS_PROJECTION, &matProj);

	matBill = matView;
	matBill._41 = 0;
	matBill._42 = 0;
	matBill._43 = 0;

	D3DXMatrixInverse(&matBill, nullptr, &matBill);

	matWorld = matBill * *m_transformCom->GetWorldMatrix();

	matWorld = *m_transformCom->GetWorldMatrix();

	effect->SetMatrix("g_matWorld", &matWorld);
	effect->SetMatrix("g_matView", &matView);
	effect->SetMatrix("g_matProj", &matProj);

	m_textureCom->SetTexture(effect, "g_BaseTexture");

	effect->SetInt("g_uStep", m_uStep);
	effect->SetInt("g_vStep", m_vStep);
	effect->SetFloat("g_uOffset", m_uOffset);
	effect->SetFloat("g_vOffset", m_vOffset);

	effect->SetVector("g_Color", &m_color);

	effect->CommitChanges();

	return S_OK;
}

void CCrossEffect::RandRotMatrix()
{
	if (m_isRandomRotation)
	{
		m_rotAngle = rand() % 360;

		m_transformCom->SetRotation(Engine::ROT_Z, D3DXToRadian(m_rotAngle));
	}
}

CCrossEffect * CCrossEffect::Create(LPDIRECT3DDEVICE9 device, const _vec4 & color, const _tchar * texName, const _int & uCnt, const _int & vCnt, const _bool & isRandRot, const _int & passIndex, const _float & lifeTime)
{
	CCrossEffect* instance = new CCrossEffect(device, color, texName, uCnt, vCnt, isRandRot, passIndex, lifeTime);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CCrossEffect::Free()
{
	Engine::CGameObject::Free();
}