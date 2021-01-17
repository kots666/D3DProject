#ifndef Management_h__
#define Management_h__

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CScene;
class CComponent;

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement();

public:
	HRESULT ReadyShader(LPDIRECT3DDEVICE9& device);
	HRESULT	SetUpScene(CScene* scene);
	_int UpdateScene(const _float& deltaTime);
	_int LateUpdateScene(const _float& deltaTime);
	void RenderScene(LPDIRECT3DDEVICE9& device);

	// Getter
	CScene* GetScene() { return m_scene; }
	CComponent* GetComponent(const _tchar* layerKey, const _tchar* objKey, const _tchar* compKey, COMPONENTID id);

private:
	CScene* m_scene;

public:
	virtual void Free();
};

END

#endif