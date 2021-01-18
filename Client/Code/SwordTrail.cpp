#include "stdafx.h"
#include "SwordTrail.h"
#include "Texture.h"
#include "TrailRect.h"

CSwordTrail::CSwordTrail(LPDIRECT3DDEVICE9 device) :
	Engine::CGameObject(device)
{
}

CSwordTrail::~CSwordTrail()
{
}

HRESULT CSwordTrail::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_trailPosVec[0].reserve(8);
	m_trailPosVec[1].reserve(8);

	return S_OK;
}

_int CSwordTrail::Update(const _float& deltaTime)
{
	for (auto iter = m_trailList.begin(); iter != m_trailList.end();)
	{
		(*iter)->DecreaseLifeTime(deltaTime);

		if (0.f >= (*iter)->GetLifeTime())
		{
			Engine::SafeRelease(*iter);
			iter = m_trailList.erase(iter);
		}
		else
			++iter;
	}

	m_rendererCom->AddObject(Engine::RENDER_ALPHA, this);

	return 0;
}

void CSwordTrail::Render()
{
	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;

	FAILED_CHECK_RETURN(SetUpConstantTable(effect), );

	Engine::SafeAddRef(effect);

	effect->Begin(nullptr, 0);
	effect->BeginPass(0);

	for (auto& elem : m_trailList)
	{
		elem->Render();
	}

	effect->EndPass();
	effect->End();

	Engine::SafeRelease(effect);
}

void CSwordTrail::AddPos(const _vec3 & end, const _vec3 & low, const _int & type)
{
	m_trailPosVec[type].emplace_back(end);
	m_trailPosVec[type].emplace_back(low);

	if (8 <= m_trailPosVec[type].size())
		MakeTrail(type);
}

HRESULT CSwordTrail::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// Texture
	component = m_texCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Texture_Trail"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Texture", component);

	// Renderer
	component = m_rendererCom = Engine::GetRenderer();
	NULL_CHECK_RETURN(component, E_FAIL);
	Engine::SafeAddRef(component);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Renderer", component);

	// Shader
	component = m_shaderCom = dynamic_cast<Engine::CShader*>(Engine::CloneComp(L"Proto_Shader_SwordTrail"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Shader", component);

	return S_OK;
}

HRESULT CSwordTrail::SetUpConstantTable(LPD3DXEFFECT & effect)
{
	// view, proj
	_matrix matView, matProj;

	m_device->GetTransform(D3DTS_VIEW, &matView);
	m_device->GetTransform(D3DTS_PROJECTION, &matProj);

	effect->SetMatrix("g_matWorld", &m_worldMat);
	effect->SetMatrix("g_matView", &matView);
	effect->SetMatrix("g_matProj", &matProj);
	
	m_texCom->SetTexture(effect, "g_BaseTexture");

	return S_OK;
}

void CSwordTrail::MakeTrail(const _int & type)
{
	_vec3 pos[4];
	_vec2 texUV[4];

	_int cutCnt = 4;

	for (_int i = 0; i < cutCnt; ++i)
	{
		_vec3 beforeEnd, beforeLow;
		_vec3 afterEnd, afterLow;

		_float beforeS = i * (1.f / cutCnt);
		_float afterS = (i + 1) * (1.f / cutCnt);

		D3DXVec3CatmullRom(&beforeEnd, &m_trailPosVec[type][0], &m_trailPosVec[type][2], &m_trailPosVec[type][4], &m_trailPosVec[type][6], beforeS);
		D3DXVec3CatmullRom(&afterEnd, &m_trailPosVec[type][0], &m_trailPosVec[type][2], &m_trailPosVec[type][4], &m_trailPosVec[type][6], afterS);
		D3DXVec3CatmullRom(&beforeLow, &m_trailPosVec[type][1], &m_trailPosVec[type][3], &m_trailPosVec[type][5], &m_trailPosVec[type][7], beforeS);
		D3DXVec3CatmullRom(&afterLow, &m_trailPosVec[type][1], &m_trailPosVec[type][3], &m_trailPosVec[type][5], &m_trailPosVec[type][7], afterS);

		pos[0] = beforeEnd;
		pos[1] = afterEnd;
		pos[2] = afterLow;
		pos[3] = beforeLow;

		texUV[0] = { beforeS, 0 };
		texUV[1] = { afterS, 0 };
		texUV[2] = { afterS, 1 };
		texUV[3] = { beforeS, 1 };

		Engine::CTrailRect* newTrail = Engine::CTrailRect::Create(m_device, pos, texUV, 0.02f);

		m_trailList.emplace_back(newTrail);
	}

	for (_int i = 0; i < 2; ++i)
		m_trailPosVec[type].erase(m_trailPosVec[type].begin());
}

CSwordTrail * CSwordTrail::Create(LPDIRECT3DDEVICE9 device)
{
	CSwordTrail* instance = new CSwordTrail(device);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CSwordTrail::Free()
{
	for (auto& elem : m_trailList)
	{
		Engine::SafeRelease(elem);
	}

	Engine::CGameObject::Free();
}