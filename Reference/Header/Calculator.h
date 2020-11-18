#ifndef Calculator_h__
#define Calculator_h__

#include "Component.h"

BEGIN(Engine)

class CTerrainTex;
class CTransform;

class ENGINE_DLL CCalculator : public CComponent
{
private:
	typedef struct tagOBB
	{
		_vec3 point[8];
		_vec3 center;
		_vec3 projAxis[3]; // 박스의 중점으로부터 각 축으로 뻗어나가는 벡터
		_vec3 axis[3];	 // 면체와 평행한 축 벡터

	}OBB;

	explicit CCalculator(LPDIRECT3DDEVICE9 device);
	explicit CCalculator(const CCalculator& rhs);
	virtual ~CCalculator();

public:
	HRESULT Ready();

	_float ComputeHeightOnTerrain(const _vec3* pos, 
								const _vec3* terrainVtxPos, 
								const _ulong& cntX, 
								const _ulong& cntZ,
								const _ulong& vtxItv = 1);
	_vec3 PickingOnTerrain(HWND hWnd, CTerrainTex* terrainBufferCom, CTransform* terrainTransCom);
	_bool CollisionAABB(const _vec3* localDstMin, const _vec3* localDstMax, const _matrix* dstWorldMat, const _vec3* localSrcMin, const _vec3* localSrcMax, const _matrix* srcWorldMat);
	_bool CollisionOBB(const _vec3* localDstMin, const _vec3* localDstMax, const _matrix* dstWorldMat, const _vec3* localSrcMin, const _vec3* localSrcMax, const _matrix* srcWorldMat);

private:
	void SetPoint(OBB* obb, const _vec3* min, const _vec3* max);
	void SetAxis(OBB* obb);

private:
	LPDIRECT3DDEVICE9 m_device;

public:
	static CCalculator* Create(LPDIRECT3DDEVICE9 device);
	virtual CComponent* Clone();

private:
	virtual void Free();

};

END
#endif // Calculator_h__
