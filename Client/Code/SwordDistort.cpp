#include "stdafx.h"
#include "SwordDistort.h"
#include "Texture.h"
#include "TrailRect.h"

CSwordDistort::CSwordDistort(LPDIRECT3DDEVICE9 device) :
	Engine::CGameObject(device)
{
}

CSwordDistort::~CSwordDistort()
{
}

HRESULT CSwordDistort::Ready()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_trailPosVec[0].reserve(8);
	m_trailPosVec[1].reserve(8);

	return S_OK;
}

_int CSwordDistort::Update(const _float& deltaTime)
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

	m_rendererCom->AddObject(Engine::RENDER_DISTORT, this);

	return 0;
}

void CSwordDistort::Render()
{
	LPD3DXEFFECT effect = m_shaderCom->GetEffectHandle();
	if (nullptr == effect) return;

	Engine::SafeAddRef(effect);

	effect->Begin(nullptr, 0);
	effect->BeginPass(0);

	for (auto& elem : m_trailList)
	{
		FAILED_CHECK_RETURN(SetUpConstantTable(effect, elem->GetLifeRatio()), );
		elem->Render();
	}

	effect->EndPass();
	effect->End();

	Engine::SafeRelease(effect);
}

void CSwordDistort::AddPos(const _vec3 & end, const _vec3 & low, const _int & type)
{
	m_trailPosVec[type].emplace_back(end);
	m_trailPosVec[type].emplace_back(low);

	if (8 <= m_trailPosVec[type].size())
		MakeTrail(type);
}

HRESULT CSwordDistort::AddComponent()
{
	Engine::CComponent* component = nullptr;

	// Texture
	component = m_noiseTexCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Texture_Distortion"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_NoiseTexture", component);

	component = m_maskTexCom = dynamic_cast<Engine::CTexture*>(Engine::CloneResource(Engine::RESOURCE_STAGE, L"Texture_CircleMask"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_MaskTexture", component);

	// Renderer
	component = m_rendererCom = Engine::GetRenderer();
	NULL_CHECK_RETURN(component, E_FAIL);
	Engine::SafeAddRef(component);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Renderer", component);

	// Shader
	component = m_shaderCom = dynamic_cast<Engine::CShader*>(Engine::CloneComp(L"Proto_Shader_Distortion"));
	NULL_CHECK_RETURN(component, E_FAIL);
	m_compMap[Engine::ID_STATIC].emplace(L"Com_Shader", component);

	return S_OK;
}

HRESULT CSwordDistort::SetUpConstantTable(LPD3DXEFFECT & effect, const _float& ratio)
{
	// view, proj
	_matrix matView, matProj;

	m_device->GetTransform(D3DTS_VIEW, &matView);
	m_device->GetTransform(D3DTS_PROJECTION, &matProj);

	/*_matrix worldMat, viewMat;

	viewMat = matView;
	viewMat._41 = 0;
	viewMat._42 = 0;
	viewMat._43 = 0;

	D3DXMatrixInverse(&viewMat, nullptr, &viewMat);

	viewMat._41 = m_worldMat._41;
	viewMat._42 = m_worldMat._42;
	viewMat._43 = m_worldMat._43;

	viewMat._11 *= 0.01f;
	viewMat._12 *= 0.01f;
	viewMat._13 *= 0.01f;

	viewMat._21 *= 0.01f;
	viewMat._22 *= 0.01f;
	viewMat._23 *= 0.01f;

	worldMat = viewMat;

	effect->SetMatrix("g_matWorld", &worldMat);*/
	effect->SetMatrix("g_matWorld", &m_worldMat);
	effect->SetMatrix("g_matView", &matView);
	effect->SetMatrix("g_matProj", &matProj);
	
	effect->SetFloat("g_Offset", ratio * 0.9f);

	m_noiseTexCom->SetTexture(effect, "g_NoiseTexture");
	m_maskTexCom->SetTexture(effect, "g_MaskTexture");

	effect->CommitChanges();

	return S_OK;
}

void CSwordDistort::MakeTrail(const _int & type)
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

		Engine::CTrailRect* newTrail = Engine::CTrailRect::Create(m_device, pos, texUV, 0.11f);

		m_trailList.emplace_back(newTrail);
	}

	for (_int i = 0; i < 2; ++i)
		m_trailPosVec[type].erase(m_trailPosVec[type].begin());
}

CSwordDistort * CSwordDistort::Create(LPDIRECT3DDEVICE9 device)
{
	CSwordDistort* instance = new CSwordDistort(device);

	if (FAILED(instance->Ready()))
		Client::SafeRelease(instance);

	return instance;
}

void CSwordDistort::Free()
{
	for (auto& elem : m_trailList)
	{
		Engine::SafeRelease(elem);
	}

	Engine::CGameObject::Free();
}