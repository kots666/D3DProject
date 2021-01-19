#ifndef Dog_h__
#define Dog_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CNaviMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CShader;
class CTexture;

END

BEGIN(Client)

class CTerrain;

class CDog : public Engine::CGameObject
{
private:
	explicit CDog(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle);
	virtual ~CDog();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

public:
	virtual _bool AttackColliderOverlapped(Engine::CGameObject* target) override;
	virtual void HitColliderOverlapped(Engine::CGameObject* causer) override;

private:
	HRESULT AddComponent();
	HRESULT LoadCollider();
	HRESULT SetUpConstantTable(LPD3DXEFFECT& effect);

	void CalcState(const _float& deltaTime);
	void MonsterAI(const _float& deltaTime);
	void DoChase(const _vec3& target, const _vec3& myPos, const _float& deltaTime);
	void DoAttack(const _vec3& target, const _vec3& myPos);
	void DoIdle();
	void DoHit();
	void DoDeadAnim();
	void LookAtTarget(const _vec3& targetPos);

private:
	Engine::CDynamicMesh* m_meshCom = nullptr;
	Engine::CNaviMesh* m_naviMeshCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;
	Engine::CCalculator* m_calcCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	Engine::CCollider* m_colliderCom = nullptr;
	Engine::CShader* m_shaderCom = nullptr;
	Engine::CTexture* m_normalTex = nullptr;

	_vec3 m_startPosition;
	_float m_startAngle;

	_matrix m_reviseMat;

	_bool m_isInterval;
	_bool m_isHit;
	_bool m_isAttack;
	_bool m_isDeadAnim;

	_float m_intervalTime;

public:
	static CDog* Create(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle = 0);

private:
	virtual void Free() override;

};

END
#endif
