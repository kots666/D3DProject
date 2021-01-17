#ifndef Spawner_h__
#define Spawner_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CNaviMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;

END

BEGIN(Client)

class CTerrain;

class CSpawner : public Engine::CGameObject
{
private:
	explicit CSpawner(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& radius);
	virtual ~CSpawner();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

public:
	virtual void HitColliderOverlapped(Engine::CGameObject* causer) override;

private:
	HRESULT AddComponent();
	HRESULT LoadCollider();

private:
	Engine::CTransform* m_transCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;

	_vec3 m_pos;
	_float m_radius;

public:
	static CSpawner* Create(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& radius);

private:
	virtual void Free() override;

};

END
#endif
