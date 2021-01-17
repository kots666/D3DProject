#include "TerrainTex.h"
#include <fstream>

USING(Engine)

CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 device) :
	CVIBuffer(device),
	m_vtxPos(nullptr),
	m_isClone(false)
{

}

CTerrainTex::CTerrainTex(const CTerrainTex& rhs) :
	CVIBuffer(rhs), m_FH(rhs.m_FH), m_IH(rhs.m_IH),
	m_heightMap(rhs.m_heightMap),
	m_vtxPos(rhs.m_vtxPos), m_isClone(true)
{

}

CTerrainTex::~CTerrainTex()
{
}

HRESULT CTerrainTex::Ready(const _ulong& xCnt, const _ulong& zCnt, const _ulong& vtxItv)
{
	m_vtxCnt = xCnt * zCnt;
	m_triCnt = (xCnt - 1) * (zCnt - 1) * 2;
	m_vtxSize = sizeof(VTXTEX);
	m_FVF = FVF_TEX;
	m_idxSize = sizeof(INDEX32);
	m_format = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready(), E_FAIL);

	LoadHeightMap();

	m_vtxPos = new _vec3[m_vtxCnt];

	VTXTEX* vertices = nullptr;

	m_VB->Lock(0, 0, (void**)&vertices, NULL);

	_ulong index = 0;

	for (_int i = 0; i < zCnt; ++i)
	{
		for (_int j = 0; j < xCnt; ++j)
		{
			index = i * xCnt + j;

			vertices[index].pos = { _float(j * vtxItv), -0.2f, _float(i * vtxItv) };
			//vertices[index].pos = { _float(j * vtxItv), (_float)(m_heightMap[index] & 0x000000ff) / 20.f, _float(i * vtxItv) };
			m_vtxPos[index] = vertices[index].pos; // 버텍스 위치정보만 따로 저장

			vertices[index].normal = { 0.f, 0.f, 0.f };
			vertices[index].texUV = { _float(j) / (xCnt - 1), _float(i) / (zCnt - 1) };
		}
	}

	INDEX32* indices = nullptr;

	m_IB->Lock(0, 0, (void**)&indices, NULL);

	_ulong triIndex = 0;

	for (_ulong i = 0; i < zCnt - 1; ++i)
	{
		for (_ulong j = 0; j < xCnt - 1; ++j)
		{
			index = i * xCnt + j;
			_vec3 dst, src, normal;

			// 오른쪽 위
			indices[triIndex]._0 = index + xCnt;
			indices[triIndex]._1 = index + xCnt + 1;
			indices[triIndex]._2 = index + 1;

			dst = vertices[indices[triIndex]._1].pos - vertices[indices[triIndex]._0].pos;
			src = vertices[indices[triIndex]._2].pos - vertices[indices[triIndex]._1].pos;
			D3DXVec3Cross(&normal, &dst, &src);

			vertices[indices[triIndex]._0].normal += normal;
			vertices[indices[triIndex]._1].normal += normal;
			vertices[indices[triIndex]._2].normal += normal;

			++triIndex;


			// 왼쪽 아래
			indices[triIndex]._0 = index + xCnt;
			indices[triIndex]._1 = index + 1;
			indices[triIndex]._2 = index;

			dst = vertices[indices[triIndex]._1].pos - vertices[indices[triIndex]._0].pos;
			src = vertices[indices[triIndex]._2].pos - vertices[indices[triIndex]._1].pos;
			D3DXVec3Cross(&normal, &dst, &src);

			vertices[indices[triIndex]._0].normal += normal;
			vertices[indices[triIndex]._1].normal += normal;
			vertices[indices[triIndex]._2].normal += normal;

			++triIndex;
		}
	}
	for (_ulong i = 0; i < m_vtxCnt; ++i)
		D3DXVec3Normalize(&vertices[i].normal, &vertices[i].normal);

	m_VB->Unlock();
	m_IB->Unlock();

	return S_OK;
}

void CTerrainTex::LoadHeightMap()
{
	HANDLE hFile = CreateFile(L"../Resource/Texture/Terrain/Height2.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	_ulong byte = 0;

	ReadFile(hFile, &m_FH, sizeof(BITMAPFILEHEADER), &byte, NULL);
	ReadFile(hFile, &m_IH, sizeof(BITMAPINFOHEADER), &byte, NULL);

	CloseHandle(hFile);

	/*_ulong size = m_IH.biWidth * m_IH.biHeight;

	m_heightMap.reserve(size);

	_ulong data = 0;

	for (_ulong i = 0; i < size; ++i)
	{
		ReadFile(hFile, &data, sizeof(_ulong), &byte, NULL);
		m_heightMap.emplace_back(data);
	}*/
}

CTerrainTex* CTerrainTex::Create(LPDIRECT3DDEVICE9 device, const _ulong& xCnt, const _ulong& zCnt, const _ulong& vtxItv)
{
	CTerrainTex* instance = new CTerrainTex(device);

	if (FAILED(instance->Ready(xCnt, zCnt, vtxItv)))
		SafeRelease(instance);

	return instance;
}

CComponent * CTerrainTex::Clone()
{
	return new CTerrainTex(*this);
}

void CTerrainTex::Free()
{
	if (!m_isClone)
	{
		SafeDeleteArray(m_vtxPos);
	}

	m_heightMap.clear();
	m_heightMap.shrink_to_fit();

	CVIBuffer::Free();
}

