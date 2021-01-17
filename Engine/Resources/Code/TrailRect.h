#ifndef TrailRect_h__
#define TrailRect_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTrailRect : public CVIBuffer
{
protected:
	explicit CTrailRect(LPDIRECT3DDEVICE9 device, const _vec3* vtxPos, const _vec2* texUV, const _float& lifeTime);
	explicit CTrailRect(const CTrailRect& rhs);
	virtual ~CTrailRect();

public:
	HRESULT Ready(const _vec3* pos, const _vec2* uv);

	HRESULT ChangePos(const _vec3* vtxPos, const _vec2* texUV);

public:
	_float GetLifeTime() const { return m_lifeTime; }

	void SetLifeTime(const _float& time) { m_lifeTime = time; }
	void DecreaseLifeTime(const _float& time) { m_lifeTime -= time; }

private:
	_vec3 m_vtxPos[4];
	_vec2 m_texUV[4];
	_float m_lifeTime = 0.1f;

public:
	static CTrailRect* Create(LPDIRECT3DDEVICE9 device, const _vec3* vtxPos, const _vec2* texUV, const _float& lifeTime = 0.1f);
	CComponent* Clone();
	virtual void Free();
};
END

#endif
