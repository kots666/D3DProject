#ifndef Layer_h__
#define Layer_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer();

public:
	HRESULT Ready();
	_int Update(const _float& deltaTime);

	// Setter
	HRESULT AddGameObject(const _tchar* key, CGameObject* obj);

	// Getter
	CGameObject* GetGameObject(const _tchar* key);
	CComponent* GetComponent(const _tchar* objKey, const _tchar* compKey, COMPONENTID id);

private:
	map<const _tchar*, CGameObject*> m_objMap;

public:
	static CLayer* Create();
	virtual void Free();
};

END

#endif