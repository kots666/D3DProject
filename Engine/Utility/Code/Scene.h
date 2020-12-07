#ifndef Scene_h__
#define Scene_h__

#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 device);
	virtual ~CScene();

public:
	virtual HRESULT Ready();
	virtual _int Update(const _float& deltaTime);
	virtual _int LateUpdate(const _float& deltaTime);
	virtual void Render();

	// Setter
	HRESULT AddLayer(const _tchar* key, CLayer* newLayer);

	// Getter
	CLayer* GetLayer(const _tchar* key);
	CComponent* GetComponent(const _tchar* layerKey, const _tchar* objKey, const _tchar* compKey, COMPONENTID id);

protected:
	LPDIRECT3DDEVICE9 m_device;
	map<const _tchar*, CLayer*> m_layerMap;

public:
	virtual void Free();
};

END

#endif