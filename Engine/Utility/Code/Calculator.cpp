#include "Calculator.h"
#include "TerrainTex.h"
#include "Transform.h"
#include <iostream>

using namespace std;

USING(Engine)

CCalculator::CCalculator(LPDIRECT3DDEVICE9 device) :
	m_device(device)
{
	SafeAddRef(m_device);
}

CCalculator::CCalculator(const CCalculator& rhs) :
	m_device(rhs.m_device)
{
	SafeAddRef(m_device);
}

CCalculator::~CCalculator()
{

}

HRESULT CCalculator::Ready()
{
	return S_OK;
}

_float CCalculator::ComputeHeightOnTerrain(const _vec3* pos, const _vec3* terrainVtxPos, const _ulong& cntX,  const _ulong& cntZ, const _ulong& vtxItv)
{
	_ulong	index = _ulong(pos->z / vtxItv) * cntX + _ulong(pos->x / vtxItv);

	_float	ratioX = (pos->x - terrainVtxPos[index + cntX].x) / vtxItv;
	_float	ratioZ = (terrainVtxPos[index + cntX].z - pos->z) / vtxItv;

	D3DXPLANE plane;

	// 오른쪽 위 삼각형
	if (ratioX > ratioZ)
	{
		D3DXPlaneFromPoints(&plane, 
			&terrainVtxPos[index + cntX],
			&terrainVtxPos[index + cntX + 1],
			&terrainVtxPos[index + 1]);
	}

	else
	{
		D3DXPlaneFromPoints(&plane,
			&terrainVtxPos[index + cntX],
			&terrainVtxPos[index + 1],
			&terrainVtxPos[index]);
	}
	/*ax + by + cz + d 
	by = -ax - cz - d
	y = (-ax - cz - d) / b*/

	_float y = (-plane.a * pos->x - plane.c * pos->z - plane.d) / plane.b;
	
	return y;


	/*_float	fHeight[4] = {
		terrainVtxPos[index + cntX].y,
		terrainVtxPos[index + cntX + 1].y,
		terrainVtxPos[index + 1].y,
		terrainVtxPos[index ].y
	};

	if (fRatioX > fRatioZ)
	{
		return fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;
	}
	else
	{
		return fHeight[0] + (fHeight[2] - fHeight[3]) * fRatioX + (fHeight[3] - fHeight[0]) * fRatioZ;
	}*/
}

_vec3 CCalculator::PickingOnTerrain(HWND hWnd, CTerrainTex * terrainBufferCom, CTransform * terrainTransCom)
{
	POINT mouse{ 0 };

	GetCursorPos(&mouse);
	ScreenToClient(hWnd, &mouse);

	_vec3 mousePos;

	D3DVIEWPORT9 viewPort;
	ZeroMemory(&viewPort, sizeof(D3DVIEWPORT9));
	m_device->GetViewport(&viewPort);

	// 뷰포트 -> 투영

	mousePos.x = (mouse.x / (viewPort.Width * 0.5f)) - 1.f;
	mousePos.y = (mouse.y / -(viewPort.Height * 0.5f)) + 1.f;
	mousePos.z = 0.f;

	// L * W * V * P * (P^-1)
	// L * W * V

	// 투영 -> 뷰 스페이스
	_matrix	matProj;
	m_device->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&mousePos, &mousePos, &matProj);

	// 뷰 스페이스 -> 월드
	_matrix	matView;
	m_device->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3 rayPos, rayDir;

	rayPos = _vec3(0.f, 0.f, 0.f);
	rayDir = mousePos - rayPos;

	D3DXVec3TransformCoord(&rayPos, &rayPos, &matView);
	D3DXVec3TransformNormal(&rayDir, &rayDir, &matView);

	// 월드 -> 로컬
	_matrix	matWorld;
	terrainTransCom->GetWorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&rayPos, &rayPos, &matWorld);
	D3DXVec3TransformNormal(&rayDir, &rayDir, &matWorld);

	_ulong vtxCntX = terrainBufferCom->GetVtxCntX();
	_ulong vtxCntZ = terrainBufferCom->GetVtxCntZ();

	const _vec3* terrainVtxPos = terrainBufferCom->GetVtxPos();

	_ulong vtxIdx[3];
	_float uValue, vValue, dist;

	for (_ulong i = 0; i < vtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < vtxCntX - 1; ++j)
		{
			_ulong index = i * vtxCntX + j;

			// 오른쪽 위
			vtxIdx[0] = index + vtxCntX;
			vtxIdx[1] = index + vtxCntX + 1;
			vtxIdx[2] = index + 1;

			// V1 + U(V2 - V1) + V(V3 - V1)
			if (D3DXIntersectTri(&terrainVtxPos[vtxIdx[0]],
				&terrainVtxPos[vtxIdx[1]],
				&terrainVtxPos[vtxIdx[2]], &rayPos, &rayDir, &uValue, &vValue, &dist))
			{
				_vec3 pos = terrainVtxPos[vtxIdx[0]] + uValue * (terrainVtxPos[vtxIdx[1]] - terrainVtxPos[vtxIdx[0]]) + vValue * (terrainVtxPos[vtxIdx[2]] - terrainVtxPos[vtxIdx[0]]);
				pos.y = 0.f;

				return pos;
			}

			// 왼쪽 아래
			vtxIdx[0] = index + vtxCntX;
			vtxIdx[1] = index + 1;
			vtxIdx[2] = index;

			// V1 + U(V2 - V1) + V(V3 - V1)
			if (D3DXIntersectTri(&terrainVtxPos[vtxIdx[0]],
				&terrainVtxPos[vtxIdx[1]],
				&terrainVtxPos[vtxIdx[2]], &rayPos, &rayDir, &uValue, &vValue, &dist))
			{
				_vec3 pos = terrainVtxPos[vtxIdx[0]] + uValue * (terrainVtxPos[vtxIdx[1]] - terrainVtxPos[vtxIdx[0]]) + vValue * (terrainVtxPos[vtxIdx[2]] - terrainVtxPos[vtxIdx[0]]);
				pos.y = 0.f;

				return pos;
			}

		}
	}

	return _vec3(0.f, 0.f, 0.f);
}

_bool CCalculator::CollisionAABB(const _vec3 * localDstMin, const _vec3 * localDstMax, const _matrix * dstWorldMat, const _vec3 * localSrcMin, const _vec3 * localSrcMax, const _matrix * srcWorldMat)
{
	_vec3	worldDstMin, worldDstMax, worldSrcMin, worldSrcMax;
	_float	min, max;

	D3DXVec3TransformCoord(&worldDstMin, localDstMin, dstWorldMat);
	D3DXVec3TransformCoord(&worldDstMax, localDstMax, dstWorldMat);

	D3DXVec3TransformCoord(&worldSrcMin, localSrcMin, srcWorldMat);
	D3DXVec3TransformCoord(&worldSrcMax, localSrcMax, srcWorldMat);

	// x축
	min = max(worldDstMin.x, worldSrcMin.x);
	max = min(worldDstMax.x, worldSrcMax.x);

	if (max < min)
		return false;

	// y축
	min = max(worldDstMin.y, worldSrcMin.y);
	max = min(worldDstMax.y, worldSrcMax.y);

	if (max < min)
		return false;

	// z축
	min = max(worldDstMin.z, worldSrcMin.z);
	max = min(worldDstMax.z, worldSrcMax.z);

	if (max < min)
		return false;

	return true;
}

CCalculator* CCalculator::Create(LPDIRECT3DDEVICE9 device)
{
	CCalculator* instance = new CCalculator(device);

	if (FAILED(instance->Ready()))
		SafeRelease(instance);

	return instance;
}

CComponent* CCalculator::Clone()
{
	return new CCalculator(*this);
}


void CCalculator::Free()
{
	SafeRelease(m_device);
}