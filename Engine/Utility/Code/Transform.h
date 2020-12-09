#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform();
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	HRESULT Ready();
	virtual _int Update(const _float& deltaTime) override;

public:
	void SetTransform(LPDIRECT3DDEVICE9& device) const;
	void MovePos(const _vec3* const dir);

	void ChaseTarget(const _vec3* targetPos, const _float& speed, const _float& deltaTime);
	const _matrix ComputeLookAtTarget(const _vec3* targetPos);
	
	void MoveToPickPos(const _vec3* pos, const _float & speed, const _float& deltaTime);
	
public:
	// Getter
	void GetInfo(INFO type, _vec3* dir);
	void GetRotation(_vec3* rot);
	void GetWorldMatrix(_matrix* outer) const;
	const _matrix* GetWorldMatrix() const;
	const _matrix* GetNRotWorldMatrix(_matrix* outer) const;

public:
	void SetPos(const _vec3& pos);
	void SetPos(const _float& x, const _float& y, const _float& z);
	void SetMovePosAtWorldMatrix(const _vec3* const dir);
	void SetScale(const _float& xScale, const _float& yScale, const _float& zScale);
	void SetRotation(const _float& xRot, const _float& yRot, const _float& zRot);
	void SetRotation(ROTATION rotType, const _float& angle);
	void SetParentMatrix(const _matrix* parentMat);

public:
	_vec3 m_info[INFO_END];
	_vec3 m_scale;
	_vec3 m_angle;
	_matrix	m_matWorld;
	_matrix m_matNRotWorld;

public:
	static CTransform* Create();
	virtual CComponent* Clone();

private:	
	virtual void Free();

};

END
#endif
