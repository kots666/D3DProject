#include "Renderer.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
{

}

CRenderer::~CRenderer()
{
	Free();
}

void CRenderer::AddObject(RENDERID group, CGameObject* gameObject)
{
	if (RENDER_END <= group)
		return;

	m_renderGroup[group].emplace_back(gameObject);
	SafeAddRef(gameObject);
}

void CRenderer::Render()
{
	for (_int i = 0; i < RENDER_END; ++i)
	{
		for (auto& iter : m_renderGroup[i])
		{
			iter->Render();
			SafeRelease(iter);
		}
		m_renderGroup[i].clear();
	}	
}

void CRenderer::Clear()
{
	for (_int i = 0; i < RENDER_END; ++i)
	{
		for_each(m_renderGroup[i].begin(), m_renderGroup[i].end(), CDeleteObj());
		m_renderGroup[i].clear();
	}
}

void CRenderer::Free()
{
	Clear();
}

