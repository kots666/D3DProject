#include "RenderTarget.h"
USING(Engine)

Engine::CRenderTarget::CRenderTarget(LPDIRECT3DDEVICE9 device) :
	m_device(device),
	m_targetTexture(nullptr),
	m_oldTargetSurface(nullptr),
	m_targetSurface(nullptr),
	m_VB(nullptr),
	m_IB(nullptr)
{
	SafeAddRef(m_device);
}

Engine::CRenderTarget::~CRenderTarget()
{

}

HRESULT Engine::CRenderTarget::ReadyRenderTarget(const _uint& width, const _uint& height, D3DFORMAT format, D3DXCOLOR color)
{
	if (FAILED(D3DXCreateTexture(m_device, 
		width, 
		height,
		1, 
		D3DUSAGE_RENDERTARGET, 
		format,
		D3DPOOL_DEFAULT, 
		&m_targetTexture)))
		return E_FAIL;

	m_clearColor = color;

	// 텍스쳐로부터 렌더타겟 성분을 추출하는 함수
	m_targetTexture->GetSurfaceLevel(0, &m_targetSurface);

	SafeRelease(m_targetTexture);
	
	return S_OK;
}

HRESULT CRenderTarget::ReadyDebugBuffer(const _float & x, const _float & y, const _float & sizeX, const _float & sizeY)
{
	FAILED_CHECK_RETURN(m_device->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,
		0, // 정적버퍼로 할당하겠다는 옵션
		FVF_SCREEN,
		D3DPOOL_MANAGED,
		&m_VB,
		NULL),
		E_FAIL);

	FAILED_CHECK_RETURN(m_device->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0, // 정적버퍼로 할당하겠다는 옵션
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_IB,
		NULL),
		E_FAIL);

	VTXSCREEN* vertices = NULL;

	m_VB->Lock(0, 0, (void**)&vertices, NULL);

	// 오른쪽 위
	vertices[0].pos = _vec4(x - 0.5f, y - 0.5f, 0.f, 1.f);
	vertices[0].texUV = _vec2(0.f, 0.f);

	vertices[1].pos = _vec4((x + sizeX) - 0.5f, y - 0.5f, 0.f, 1.f);
	vertices[1].texUV = _vec2(1.f, 0.f);

	vertices[2].pos = _vec4((x + sizeX) - 0.5f, (y + sizeY) - 0.5f, 0.f, 1.f);
	vertices[2].texUV = _vec2(1.f, 1.f);

	vertices[3].pos = _vec4(x - 0.5f, (y + sizeY) - 0.5f, 0.f, 1.f);
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

void Engine::CRenderTarget::Clear()
{
	m_device->GetRenderTarget(0, &m_oldTargetSurface);

	m_device->SetRenderTarget(0, m_targetSurface);

	m_device->Clear(0, NULL, D3DCLEAR_TARGET, m_clearColor, 1.f, 0);

	m_device->SetRenderTarget(0, m_oldTargetSurface);

	SafeRelease(m_oldTargetSurface);
}

void Engine::CRenderTarget::SetUpOnDevice(const _uint& index)
{
	m_device->GetRenderTarget(index, &m_oldTargetSurface);

	m_device->SetRenderTarget(index, m_targetSurface);
}

void CRenderTarget::ReleaseOnDevice(const _uint & index)
{
	m_device->SetRenderTarget(index, m_oldTargetSurface);
	SafeRelease(m_oldTargetSurface);
}

void Engine::CRenderTarget::ThrowRenderTargetTexture(LPD3DXEFFECT & effect, const char * constantName)
{
	effect->SetTexture(constantName, m_targetTexture);
}

void CRenderTarget::RenderDebugBuffer()
{
	m_device->SetTexture(0, m_targetTexture);

	m_device->SetStreamSource(0, m_VB, 0, sizeof(VTXSCREEN));
	m_device->SetFVF(FVF_SCREEN);
	m_device->SetIndices(m_IB);
	m_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

CRenderTarget* Engine::CRenderTarget::Create(LPDIRECT3DDEVICE9 device, const _uint& width, const _uint& height, D3DFORMAT format, D3DXCOLOR color)
{
	CRenderTarget* instance = new CRenderTarget(device);

	if (FAILED(instance->ReadyRenderTarget(width, height, format, color)))
		SafeRelease(instance);

	return instance;
}

void Engine::CRenderTarget::Free()
{
	SafeRelease(m_oldTargetSurface);
	SafeRelease(m_targetSurface);

	SafeRelease(m_VB);
	SafeRelease(m_IB);
	SafeRelease(m_device);
}

