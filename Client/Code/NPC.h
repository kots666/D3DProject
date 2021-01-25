#ifndef NPC_h__
#define NPC_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CShader;
class CTexture;

END

BEGIN(Client)

class CTerrain;

class CNPC : public Engine::CGameObject
{
private:
	explicit CNPC(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle);
	virtual ~CNPC();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

private:
	HRESULT AddComponent();
	HRESULT SetUpConstantTable(LPD3DXEFFECT& effect);

	void LookAtTarget(const _vec3& targetPos);

private:
	Engine::CDynamicMesh* m_meshCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	Engine::CShader* m_shaderCom = nullptr;
	Engine::CTexture* m_normalCom = nullptr;

	_float m_angle;
	_vec3 m_startPosition;

	_matrix m_reviseMat;

public:
	static CNPC* Create(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& angle = 0.f);

private:
	virtual void Free() override;

};

END
#endif
