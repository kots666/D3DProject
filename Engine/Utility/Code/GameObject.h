#ifndef GameObj_h__
#define GameObj_h__

#include "Component.h"

BEGIN(Engine)

class CSphereCollider;

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 device);
	//explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	virtual HRESULT Ready() { return S_OK; }
	virtual _int Update(const _float& deltaTime);
	virtual _int LateUpdate(const _float& deltaTime);
	virtual _bool AttackColliderOverlapped(CGameObject* causesr) { return true; }
	virtual void HitColliderOverlapped(CGameObject* target) {}
	virtual void Render() {}

public:
	_bool IsInCollideList(CGameObject* obj);
	void ClearCollideList();

	virtual void EnableAttackCollider();
	virtual void EnableHitCollider();

	virtual void DisableAttackCollider();
	virtual void DisableHitCollider();

public:
	// Setter
	void SetIsDead(const _bool& isDead) { m_isDead = isDead; }

	// Getter
	_int GetHp() const { return m_hp; }
	_int GetMaxHp() const { return m_maxHp; }
	_int GetATK() const { return m_atk; }
	_float GetViewZ() const { return m_viewZ; }

	_bool GetIsDead() const { return m_isDead; }
	CComponent* GetComponent(const _tchar* key, COMPONENTID id);
	vector<CSphereCollider*>* GetHitColliderVec() { return &m_hitCollider; }
	vector<CSphereCollider*>* GetAttackColliderVec() { return &m_attackCollider; }

	void ComputeViewZ(const _vec3* pos);

protected:
	LPDIRECT3DDEVICE9 m_device;
	map<const _tchar*, CComponent*> m_compMap[ID_END];
	vector<CSphereCollider*> m_hitCollider;
	vector<CSphereCollider*> m_attackCollider;
	list<CGameObject*> m_collideList;

	_bool m_isDead;
	_float m_speed;
	_int m_hp;
	_int m_maxHp;
	_int m_atk;

	_float m_viewZ = 0.f;

public:
	virtual void Free();
};

END

#endif