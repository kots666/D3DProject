#include "Export_Utility.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer() :
	m_VB(nullptr),
	m_IB(nullptr)
{

}

CRenderer::~CRenderer()
{
	Free();
}

HRESULT CRenderer::ReadyRenderer(LPDIRECT3DDEVICE9 & device)
{
	FAILED_CHECK_RETURN(device->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,
		0, // �������۷� �Ҵ��ϰڴٴ� �ɼ�
		FVF_SCREEN,
		D3DPOOL_MANAGED,
		&m_VB,
		NULL),
		E_FAIL);

	FAILED_CHECK_RETURN(device->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0, // �������۷� �Ҵ��ϰڴٴ� �ɼ�
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_IB,
		NULL),
		E_FAIL);

	D3DVIEWPORT9 viewPort;
	device->GetViewport(&viewPort);

	VTXSCREEN* vertices = NULL;

	m_VB->Lock(0, 0, (void**)&vertices, NULL);

	// ������ ��
	vertices[0].pos = _vec4(0.f - 0.5f, 0.f - 0.5f, 0.f, 1.f);
	vertices[0].texUV = _vec2(0.f, 0.f);

	vertices[1].pos = _vec4(viewPort.Width - 0.5f, 0.f - 0.5f, 0.f, 1.f);
	vertices[1].texUV = _vec2(1.f, 0.f);

	vertices[2].pos = _vec4(viewPort.Width - 0.5f, viewPort.Height - 0.5f, 0.f, 1.f);
	vertices[2].texUV = _vec2(1.f, 1.f);

	vertices[3].pos = _vec4(0.f - 0.5f, viewPort.Height - 0.5f, 0.f, 1.f);
	vertices[3].texUV = _vec2(0.f, 1.f);

	m_VB->Unlock();

	INDEX16* indices = nullptr;

	m_IB->Lock(0, 0, (void**)&indices, 0);

	indices[0]._0 = 0;
	indices[0]._1 = 1;
	indices[0]._2 = 2;

	indices[1]._0 = 0;
	indices[1]._1 = 2;
	indices[1]._2 = 3;

	m_IB->Unlock();

	return S_OK;
}

void CRenderer::AddObject(RENDERID group, CGameObject* gameObject)
{
	if (RENDER_END <= group)
		return;

	m_renderGroup[group].emplace_back(gameObject);
	SafeAddRef(gameObject);
}

void CRenderer::Render(LPDIRECT3DDEVICE9& device)
{
	// SkyBox
	RenderPriority(device);
	
	// Deferred Rendering
	RenderDeferred(device);
	RenderLightAcc(device);

	RenderBlend(device);

	// Forward Rendering for AlphaBlending
	RenderAlpha(device);
	RenderUI(device);

	RenderDebugBuffer(L"MRT_Deferred");
	RenderDebugBuffer(L"MRT_LightAcc");

	Clear();
}

void CRenderer::Clear()
{
	for (_int i = 0; i < RENDER_END; ++i)
	{
		for_each(m_renderGroup[i].begin(), m_renderGroup[i].end(), CDeleteObj());
		m_renderGroup[i].clear();
	}
}

_bool CompareViewZGreater(CGameObject * dst, CGameObject * src)
{
	return dst->GetViewZ() > src->GetViewZ();
}

_bool CompareViewZless(CGameObject * dst, CGameObject * src)
{
	return dst->GetViewZ() > src->GetViewZ();
}

void CRenderer::RenderPriority(LPDIRECT3DDEVICE9 & device)
{
	for (auto& iter : m_renderGroup[RENDER_PRIORITY])
		iter->Render();
}

void CRenderer::RenderNonAlpha(LPDIRECT3DDEVICE9 & device)
{
	m_renderGroup[RENDER_NONALPHA].sort(CompareViewZless);

	for (auto& iter : m_renderGroup[RENDER_NONALPHA])
		iter->Render();
}

void CRenderer::RenderAlpha(LPDIRECT3DDEVICE9 & device)
{
	//m_renderGroup[RENDER_ALPHA].sort(CompareViewZGreater);

	for (auto& iter : m_renderGroup[RENDER_ALPHA])
		iter->Render();
}

void CRenderer::RenderUI(LPDIRECT3DDEVICE9 & device)
{
	for (auto& iter : m_renderGroup[RENDER_UI])
		iter->Render();
}

void CRenderer::RenderDeferred(LPDIRECT3DDEVICE9 & device)
{
	BeginMRT(L"MRT_Deferred");
	RenderNonAlpha(device);
	EndMRT(L"MRT_Deferred");
}

void CRenderer::RenderLightAcc(LPDIRECT3DDEVICE9 & device)
{
	BeginMRT(L"MRT_LightAcc");

	CShader* shader = dynamic_cast<Engine::CShader*>(Engine::CloneComp(L"Proto_Shader_Shade"));
	if (nullptr == shader)
	{
		EndMRT(L"MRT_LightAcc");
		return;
	}

	LPD3DXEFFECT effect = shader->GetEffectHandle();
	SafeAddRef(effect);

	Engine::ThrowRenderTargetTexture(effect, L"Target_Normal", "g_NormalTexture");
	Engine::ThrowRenderTargetTexture(effect, L"Target_Depth", "g_DepthTexture");

	effect->Begin(NULL, 0);

	Engine::RenderLight(effect);

	effect->End();

	EndMRT(L"MRT_LightAcc");

	SafeRelease(effect);
	SafeRelease(shader);
}

void CRenderer::RenderBlend(LPDIRECT3DDEVICE9 & device)
{
	CShader* shader = dynamic_cast<Engine::CShader*>(Engine::CloneComp(L"Proto_Shader_Blend"));
	NULL_CHECK(shader);

	LPD3DXEFFECT pEffect = shader->GetEffectHandle();
	SafeAddRef(pEffect);

	Engine::ThrowRenderTargetTexture(pEffect, L"Target_Albedo", "g_AlbedoTexture");
	Engine::ThrowRenderTargetTexture(pEffect, L"Target_Shade", "g_ShadeTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	device->SetStreamSource(0, m_VB, 0, sizeof(VTXSCREEN));
	device->SetFVF(FVF_SCREEN);
	device->SetIndices(m_IB);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	SafeRelease(pEffect);
	SafeRelease(shader);
}

void CRenderer::Free()
{
	Clear();

	SafeRelease(m_VB);
	SafeRelease(m_IB);
}