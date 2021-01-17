#ifndef GoblinMagician_h__
#define GoblinMagician_h__

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

class CGoblinMagician : public Engine::CGameObject
{
private:
	explicit CGoblinMagician(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle);
	virtual ~CGoblinMagician();

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
	void DoChase(const _vec3& target, const _vec3& myPos, const _float& deltaTime);
	void DoAttack(const _vec3& target, const _vec3& myPos);
	void DoSpawn(const _vec3& target, const _vec3& myPos);
	void DoIdle();
	void DoHit();
	void DoDeadAnim();
	void LookAtTarget(const _vec3& targetPos);
	void SpawnFireBall(const _vec3& targetPos, const _vec3& myPos);
	void SpawnGoblinSword();

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
	_bool m_isSpawn;
	_bool m_isSpawned;

	_float m_intervalTime;

public:
	static CGoblinMagician* Create(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle = 0.f);

private:
	virtual void Free() override;

};

END
#endif
