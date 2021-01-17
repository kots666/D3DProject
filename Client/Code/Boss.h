#ifndef Boss_h__
#define Boss_h__

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

class CBoss : public Engine::CGameObject
{
private:
	explicit CBoss(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle);
	virtual ~CBoss();

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

	void CalcState(const _float& deltaTime);
	void MonsterAI(const _float& deltaTime);

	void DoPhase1(const _float& deltaTime);
	void DoPhase2(const _float& deltaTime);

	void DoChase(const _vec3& target, const _vec3& myPos, const _float& deltaTime);

	// ATK
	void DoAttack1(const _vec3& target, const _vec3& myPos);
	void DoAttack2(const _vec3& target, const _vec3& myPos);
	void DoAttack3(const _vec3& target, const _vec3& myPos);
	void DoAttack4(const _vec3& target, const _vec3& myPos);
	void DoAttack5(const _vec3& target, const _vec3& myPos);

	void DoRageAttack1(const _vec3& target, const _vec3& myPos);
	void DoRageAttack2(const _vec3& target, const _vec3& myPos);
	void DoRageAttack3(const _vec3& target, const _vec3& myPos);
	void DoRageAttack4(const _vec3& target, const _vec3& myPos);
	void DoRageAttack5(const _vec3& target, const _vec3& myPos);
	
	void DoIdle();
	void DoRageIdle();

	void DoKnockBack();
	void DoKnockUp();
	void DoDeadAnim();
	void LookAtTarget(const _vec3& targetPos);

private:
	Engine::CDynamicMesh* m_meshCom = nullptr;
	Engine::CNaviMesh* m_naviMeshCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;
	Engine::CCalculator* m_calcCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;

	_float m_angle;
	_vec3 m_startPosition;

	_matrix m_reviseMat;

	_bool m_isInterval;
	_bool m_isHit;
	_bool m_isAttack;
	_bool m_isDeadAnim;

	_int m_phase;

	_float m_intervalTime;

public:
	static CBoss* Create(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle = 0.f);

private:
	virtual void Free() override;

};

END
#endif
