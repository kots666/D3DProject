#include "Pipeline.h"

USING(Engine)

Engine::CPipeline::CPipeline()
{

}

Engine::CPipeline::~CPipeline()
{

}

void Engine::CPipeline::MakeTransformMatrix(_matrix* out, const _vec3* right, const _vec3* up, const _vec3* look, const _vec3* pos)
{
	memcpy(&out->m[0][0], right, sizeof(_float) * 3);
	memcpy(&out->m[1][0], up, sizeof(_float) * 3);
	memcpy(&out->m[2][0], look, sizeof(_float) * 3);
	memcpy(&out->m[3][0], pos, sizeof(_float) * 3);

}

void CPipeline::MyRotationX(_vec3* out, _vec3* in, _float angle)
{
	/*	1	0	0	0
		0	c	s	0
		0	-s	c	0
		0	0	0	1	*/

	_vec3 temp = *in;

	out->x = temp.x;
	out->y = temp.y * cosf(angle) - temp.z * sinf(angle);
	out->z = temp.y * sinf(angle) + temp.z * cosf(angle);

}

void CPipeline::MyRotationY(_vec3* out, _vec3* in, _float angle)
{
	/*	c	0	-s	0
		0	1	0	0
		s	0	c	0
		0	0	0	1	*/

	_vec3 temp = *in;

	out->x = temp.x * cosf(angle) + temp.z * sinf(angle);
	out->y = temp.y;
	out->z = temp.x * -sinf(angle) + temp.z * cosf(angle);
}



void CPipeline::MyRotationZ(_vec3* out, _vec3* in, _float angle)
{
	/*	c	s	0	0
		-s	c	0	0
		0	0	1	0
		0	0	0	1	*/

	_vec3 temp = *in;

	out->x = temp.x * cosf(angle) + temp.y * -sinf(angle);
	out->y = temp.x * sinf(angle) + temp.y * cosf(angle);
	out->z = temp.z;

}

void Engine::CPipeline::MakeViewSpaceMatrix(_matrix* out, const _vec3* eye, const _vec3* at, const _vec3* up)
{
	D3DXMatrixIdentity(out);

	_vec3 right = _vec3(1.f, 0.f, 0.f);
	_vec3 myUp = _vec3(0.f, 1.f, 0.f);
	_vec3 look = _vec3(0.f, 0.f, 1.f);
	_vec3 pos = _vec3(0.f, 0.f, 0.f);

	// look 벡터를 구해주자.
	look = *at - *eye;
	D3DXVec3Normalize(&look, &look);

	// RIght벡터를 구해주자.
	D3DXVec3Cross(&right, up, &look);
	D3DXVec3Normalize(&right, &right);

	// up벡터를 구해주자.
	D3DXVec3Cross(&myUp, &look, &right);
	D3DXVec3Normalize(&myUp, &myUp);

	pos = *eye;

	// 카메라 행렬의 월드를 구성
	MakeTransformMatrix(out, &right, &myUp, &look, &pos);

	// 뷰스페이스 변환 행렬 구성
	D3DXMatrixInverse(out, NULL, out);
}

void Engine::CPipeline::MakePerspectiveMatrix(_matrix* out, const _float& fovY, const _float& aspect, const _float& nearZ, const _float& farZ)
{
	D3DXMatrixIdentity(out);

	out->_11 = (1.f / tanf(fovY / 2.f)) / aspect;
	out->_22 = 1.f / tanf(fovY / 2.f);
	out->_33 = farZ / (farZ - nearZ);
	out->_43 = (-nearZ * farZ) / (farZ - nearZ);

	out->_34 = 1.f;
	out->_44 = 0.f;
}