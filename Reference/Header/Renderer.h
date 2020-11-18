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
	void AddObject(RENDERID group, CGameObject* gameObject);
	void Render();
	void Clear();

private:
	list<CGameObject*> m_renderGroup[RENDER_END];

private:
	virtual CComponent* Clone() { return nullptr; }
	virtual void Free();
};

END

#endif
