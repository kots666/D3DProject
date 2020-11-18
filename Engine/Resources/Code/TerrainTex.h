#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
protected:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 device);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex();

public:
	virtual HRESULT	Ready(const _ulong& xCnt, const _ulong& zCnt, const _ulong& vtxItv);
	void LoadHeightMap();

public:
	_vec3* GetVtxPos() { return m_vtxPos; }
	_ulong GetVtxCnt() { return m_vtxCnt; }
	_ulong GetVtxCntX() const { return m_IH.biWidth; }
	_ulong GetVtxCntZ() const { return m_IH.biHeight; }
	_ulong GetTriCnt() { return m_triCnt; }

public:
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 device, const _ulong& xCnt, const _ulong& zCnt, const _ulong& vtxItv = 1);
	CComponent* Clone();
	virtual void Free();

private:
	vector<_ulong> m_heightMap;

	BITMAPFILEHEADER m_FH;
	BITMAPINFOHEADER m_IH;
	_vec3* m_vtxPos;
	_bool m_isClone;
};

END

#endif
