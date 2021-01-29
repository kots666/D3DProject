#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Define.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer : public CComponent
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer();
	virtual ~CRenderer();

public:
	HRESULT ReadyRenderer(LPDIRECT3DDEVICE9& device);

public:
	void AddObject(RENDERID group, CGameObject* gameObject);
	void Render(LPDIRECT3DDEVICE9& device);
	void Clear();

public:
	void SwitchShowMRT() { m_isShowMRT ^= true; }
	void SetFogDensity(const _float& density) { m_fogDensity = density; }

private:
	void RenderPriority(LPDIRECT3DDEVICE9& device);
	void RenderSkyBox(LPDIRECT3DDEVICE9& device);
	void RenderNonAlpha(LPDIRECT3DDEVICE9& device);
	void RenderDistort(LPDIRECT3DDEVICE9& device);
	void RenderAlpha(LPDIRECT3DDEVICE9& device);
	void RenderUI(LPDIRECT3DDEVICE9& device);

	void RenderDeferred(LPDIRECT3DDEVICE9& device);
	void RenderLightAcc(LPDIRECT3DDEVICE9& device);
	void RenderDistortion(LPDIRECT3DDEVICE9& device);
	void RenderBlend(LPDIRECT3DDEVICE9& device);

private:
	list<CGameObject*> m_renderGroup[RENDER_END];
	LPDIRECT3DVERTEXBUFFER9 m_VB;
	LPDIRECT3DINDEXBUFFER9 m_IB;

	_bool m_isShowMRT;
	_float m_fogDensity;

private:
	virtual CComponent* Clone() { return nullptr; }
	virtual void Free();
};

END

#endif
