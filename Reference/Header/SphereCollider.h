#ifndef SphereCollider_h__
#define SphereCollider_h__

#include "Component.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CSphereCollider : public CComponent
{
private:
	explicit CSphereCollider(LPDIRECT3DDEVICE9 device, _tchar* name, _matrix* boneMat, const _vec3& offset, const _float& radius);
	explicit CSphereCollider(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& radius);
	explicit CSphereCollider(const CSphereCollider& rhs);
	virtual ~CSphereCollider();

public:
	_bool GetCanCollide() const { return m_canCollide; }
	_bool GetIsCollide() const { return m_isCollide; }
	_vec3 GetOffset() const { return m_offset; }
	_float GetWorldRadius() const { return m_worldRadius; }
	_float GetLocalRadius() const { return m_localRadius; }
	_matrix GetWorldMat() const { return m_worldMat; }

	void SetCanCollide(const _bool& can) { m_canCollide = can; }
	void SetIsCollide(const _bool& collide) { m_isCollide = collide; }
	void SetOffset(const _vec3& offset) { m_offset = offset; }
	void SetLocalRadius(const _float& radius) { m_localRadius = radius; m_worldRadius = m_localRadius * m_scale; }
	void SetScale(const _float& scale) { m_scale = scale; m_worldRadius = m_localRadius * m_scale; }

public:
	HRESULT Ready();
	virtual _int Update(const _float& deltaTime) override;
	void UpdateByBone(const _matrix* worldMat);
	void Render();

private:
	CGameObject* m_obj;
	_matrix* m_attachBone;
	_tchar* m_name;

	_matrix m_worldMat;
	_vec3 m_offset;
	_vec3 m_worldPos;
	_float m_localRadius;

	_float m_scale;
	_float m_worldRadius;

	_float m_accTime;

	_bool m_canCollide;
	_bool m_isCollide;

	LPDIRECT3DDEVICE9 m_device;
	LPDIRECT3DTEXTURE9 m_texture[COL_END];

public:
	static CSphereCollider* Create(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& radius);
	static CSphereCollider* Create(LPDIRECT3DDEVICE9 device, _tchar* name, _matrix* boneMat, const _vec3& offset, const _float& radius);
	virtual CComponent* Clone();
	virtual void Free();
};
END
#endif // SphereCollider_h__
