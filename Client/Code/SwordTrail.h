#ifndef SwordTrail_h__
#define SwordTrail_h__

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

class CSwordTrail : public Engine::CGameObject
{
private:
	explicit CSwordTrail(LPDIRECT3DDEVICE9 device);
	virtual ~CSwordTrail();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

public:
	void AddPos(const _vec3& end, const _vec3& low);

	void SetWorldMat(const _matrix& worldMat) { m_worldMat = worldMat; }

private:
	HRESULT AddComponent();
	HRESULT SetUpConstantTable(LPD3DXEFFECT& effect);

	void MakeTrail();

private:
	Engine::CTexture* m_texCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	Engine::CShader* m_shaderCom = nullptr;

	_matrix m_worldMat;

	vector<_vec3> m_trailPosVec;
	list<Engine::CTrailRect*> m_trailList;

public:
	static CSwordTrail* Create(LPDIRECT3DDEVICE9 device);

private:
	virtual void Free() override;

};

END
#endif
