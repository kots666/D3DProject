#ifndef FireBall_h__
#define FireBall_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CShader;

END

BEGIN(Client)

class CTerrain;

class CFireBall : public Engine::CGameObject
{
private:
	explicit CFireBall(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _vec3& target);
	virtual ~CFireBall();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

public:
	virtual _bool AttackColliderOverlapped(Engine::CGameObject* target) override;

private:
	HRESULT AddComponent();
	HRESULT LoadCollider();
	HRESULT SetUpConstantTable(LPD3DXEFFECT& effect);
	void LookAtTarget();

private:
	Engine::CStaticMesh* m_headMeshCom = nullptr;
	Engine::CStaticMesh* m_tailMeshCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	Engine::CShader* m_shaderCom = nullptr;

	_vec3 m_pos;
	_vec3 m_target;
	_vec3 m_dir;

	_float m_lifeTime;

public:
	static CFireBall* Create(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _vec3& target);

private:
	virtual void Free() override;

};

END
#endif
