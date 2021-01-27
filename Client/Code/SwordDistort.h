#ifndef SwordDistort_h__
#define SwordDistort_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CTexture;
class CTrailRect;
class CTransform;
class CRenderer;
class CShader;

END

BEGIN(Client)

class CSwordDistort : public Engine::CGameObject
{
private:
	explicit CSwordDistort(LPDIRECT3DDEVICE9 device);
	virtual ~CSwordDistort();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

public:
	void AddPos(const _vec3& end, const _vec3& low, const _int& type);

	void SetWorldMat(const _matrix& worldMat) { m_worldMat = worldMat; }

private:
	HRESULT AddComponent();
	HRESULT SetUpConstantTable(LPD3DXEFFECT& effect, const _float& ratio);

	void MakeTrail(const _int& type);

private:
	Engine::CTexture* m_noiseTexCom = nullptr;
	Engine::CTexture* m_maskTexCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	Engine::CShader* m_shaderCom = nullptr;

	_matrix m_worldMat;

	vector<_vec3> m_trailPosVec[2];
	list<Engine::CTrailRect*> m_trailList;

public:
	static CSwordDistort* Create(LPDIRECT3DDEVICE9 device);

private:
	virtual void Free() override;

};

END
#endif
