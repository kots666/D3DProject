#include "Transform.h"

USING(Engine)

CTransform::CTransform() :
	m_scale(1.f, 1.f, 1.f),
	m_angle(0.f, 0.f, 0.f)
{
	ZeroMemory(m_info, sizeof(_vec3) * INFO_END);
	D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(const CTransform& rhs) :
	m_scale(rhs.m_scale),
	m_angle(rhs.m_angle)
{
	for (_int i = 0; i < INFO_END; ++i)
		m_info[i] = rhs.m_info[i];
	
	m_matWorld = rhs.m_matWorld;
}

CTransform::~CTransform()
{

}

HRESULT CTransform::Ready()
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_int i = 0; i < INFO_END; ++i)
		memcpy(&m_info[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return S_OK;
}

_int CTransform::Update(const _float & deltaTime)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matNRotWorld);

	// 위치 벡터를 제외하고 나머지 벡터의 초기화 값을 세팅한 상황
	for (_uint i = 0; i < INFO_POS; ++i)
		memcpy(&m_info[i], &m_matWorld.m[i][0], sizeof(_vec3));

	// 크기 변환 적용

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_info[i], &m_info[i]);
		m_info[i] *= *(((_float*)&m_scale + i));
	}
	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matNRotWorld.m[i][0], m_info[i], sizeof(_vec3));

	_matrix matRot[ROT_END];
	D3DXMatrixRotationX(&matRot[ROT_X], m_angle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_angle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_angle.z);

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		for (_uint j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_info[i], &m_info[i], &matRot[j]);
		}
	}

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_info[i], sizeof(_vec3));

	return 0;
}

void CTransform::SetTransform(LPDIRECT3DDEVICE9& device) const
{
	device->SetTransform(D3DTS_WORLD, &m_matWorld);
}

void CTransform::MovePos(const _vec3 * const dir)
{
	m_info[INFO_POS] += *dir;
}

void CTransform::ChaseTarget(const _vec3 * targetPos, const _float & speed, const _float & deltaTime)
{
	_vec3 dir = *targetPos - m_info[INFO_POS];
	D3DXVec3Normalize(&dir, &dir);

	m_info[INFO_POS] += dir * speed * deltaTime;

	_matrix matScale, matRot, matTrans;

	matRot = ComputeLookAtTarget(targetPos);

	D3DXMatrixScaling(&matScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixTranslation(&matTrans, m_info[INFO_POS].x, m_info[INFO_POS].y, m_info[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;
}

const _matrix CTransform::ComputeLookAtTarget(const _vec3 * targetPos)
{
	_vec3 dir = *targetPos - m_info[INFO_POS];
	_vec3 axis, up;
	_matrix matRot;

	D3DXVec3Normalize(&up, &m_info[INFO_UP]);
	D3DXVec3Normalize(&dir, &dir);

	D3DXVec3Cross(&axis, &up, &dir);

	_float angle = acosf(D3DXVec3Dot(&up, &dir));

	D3DXMatrixRotationAxis(&matRot, &axis, angle);

	return matRot;
}

void CTransform::MoveToPickPos(const _vec3 * pos, const _float & speed, const _float & deltaTime)
{
	_vec3 dir = *pos - m_info[INFO_POS];
	D3DXVec3Normalize(&dir, &dir);

	m_info[INFO_POS] += dir * speed * deltaTime;
}

void CTransform::GetInfo(INFO type, _vec3 * dir)
{
	memcpy(dir, &m_matWorld.m[type][0], sizeof(_vec3));
}

void CTransform::GetWorldMatrix(_matrix * outer) const
{
	*outer = m_matWorld;
}

const _matrix * CTransform::GetWorldMatrix() const
{
	return &m_matWorld;
}

const _matrix * CTransform::GetNRotWorldMatrix(_matrix * outer) const
{
	if (nullptr != outer)
		*outer = m_matNRotWorld;

	return &m_matNRotWorld;
}

void CTransform::SetPos(const _vec3 & pos)
{
	m_info[INFO_POS] = pos;
}

void CTransform::SetPos(const _float& x, const _float& y, const _float& z)
{
	m_info[INFO_POS].x = x;
	m_info[INFO_POS].y = y;
	m_info[INFO_POS].z = z;
}

void CTransform::SetMovePosAtWorldMatrix(const _vec3 * const dir)
{
	_vec3 curPos;
	memcpy(&curPos, &(m_matWorld.m[3]), sizeof(_vec3));

	curPos += *dir;

	memcpy(&(m_matWorld.m[3]), &curPos, sizeof(_vec3));
}

void CTransform::SetScale(const _float & xScale, const _float & yScale, const _float & zScale)
{
	m_scale = _vec3(xScale, yScale, zScale);
}

void CTransform::SetRotation(const _float & xRot, const _float & yRot, const _float & zRot)
{
	m_angle = { xRot, yRot, zRot };
}

void CTransform::SetRotation(ROTATION rotType, const _float & angle)
{
	*(((_float*)&m_angle) + rotType) += angle;
}

void CTransform::SetParentMatrix(const _matrix * parentMat)
{
	m_matWorld *= *parentMat;
}

CTransform* CTransform::Create()
{
	CTransform*	instance = new CTransform;

	if (FAILED(instance->Ready()))
		SafeRelease(instance);

	return instance;
}

CComponent* CTransform::Clone()
{
	return new CTransform(*this);
}

void CTransform::Free()
{

}