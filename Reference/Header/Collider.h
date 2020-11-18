#ifndef Collider_h__
#define Collider_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider : public CComponent
{
private:
	explicit CCollider(LPDIRECT3DDEVICE9 device);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider();

public:
	const _vec3* GetMinPos() { return &m_minPos; }
	const _vec3* GetMaxPos() { return &m_maxPos; }
	const _matrix* GetCollMatrix() { return &m_collMatrix; }

public:
	HRESULT Ready(const _vec3* vtxPos, const _ulong& numVtxCnt, const _ulong& stride);
	void Render(COLLTYPE type, const _matrix* colliderMatrix);

private:
	_vec3 m_minPos, m_maxPos;
	_matrix m_collMatrix;

#ifdef _DEBUG
	LPDIRECT3DDEVICE9 m_device;
	LPDIRECT3DVERTEXBUFFER9 m_VB;
	LPDIRECT3DINDEXBUFFER9 m_IB;
	LPDIRECT3DTEXTURE9 m_texture[COL_END];

#endif

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 device, const _vec3* vtxPos, const _ulong& numVtxCnt, const _ulong& stride);
	virtual CComponent* Clone();
	virtual void Free();
};
END
#endif // Collider_h__
