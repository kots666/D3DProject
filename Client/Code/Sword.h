#ifndef Sword_h__
#define Sword_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;

END

BEGIN(Client)

class CSword : public Engine::CGameObject
{
private:
	explicit CSword(LPDIRECT3DDEVICE9 device);
	virtual ~CSword();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

private:
	HRESULT AddComponent();
	_bool CollisionToObject(const _tchar* layerTag, const _tchar* objTag);

private:

	Engine::CStaticMesh* m_meshCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	Engine::CCalculator* m_calcCom = nullptr;
	Engine::CCollider* m_colliderCom = nullptr;
	_bool m_isColl = false;

	const _matrix* m_parentBoneMatrix = nullptr;
	const _matrix* m_parentWorldMatrix = nullptr;

public:
	static CSword* Create(LPDIRECT3DDEVICE9 device);

private:
	virtual void Free() override;

};

END
#endif // Sword_h__
