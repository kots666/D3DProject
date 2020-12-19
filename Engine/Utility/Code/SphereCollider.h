#ifndef SphereCollider_h__
#define SphereCollider_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CSphereCollider : public CComponent
{
private:
	explicit CSphereCollider(LPDIRECT3DDEVICE9 device, _matrix* boneMat, const _vec3& offset, const _float& radius);
	explicit CSphereCollider(const CSphereCollider& rhs);
	virtual ~CSphereCollider();

public:
	_vec3 GetOffset() const { return m_offset; }
	_float GetRadius() const { return m_radius; }

	void SetOffset(const _vec3& offset) { m_offset = offset; }
	void SetRadius(const _float& radius) { m_radius = radius; }

public:
	HRESULT Ready();
	void UpdateByBone();
	void Render(COLLTYPE type, _matrix* worldMat);

private:
	_matrix m_localMat;
	_matrix* m_attachBone;
	_vec3 m_offset;
	_float m_radius;

	LPDIRECT3DDEVICE9 m_device;
	LPDIRECT3DTEXTURE9 m_texture[COL_END];

public:
	static CSphereCollider* Create(LPDIRECT3DDEVICE9 device, _matrix* boneMat, const _vec3& offset, const _float& radius);
	virtual CComponent* Clone();
	virtual void Free();
};
END
#endif // SphereCollider_h__
