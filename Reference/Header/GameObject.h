#ifndef GameObj_h__
#define GameObj_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 device);
	//explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	virtual HRESULT Ready() { return S_OK; }
	virtual _int Update(const _float& deltaTime);
	virtual void Render() {}

	// Getter
	CComponent* GetComponent(const _tchar* key, COMPONENTID id);

protected:
	LPDIRECT3DDEVICE9 m_device;
	map<const _tchar*, CComponent*> m_compMap[ID_END];

public:
	virtual void Free();
};

END

#endif