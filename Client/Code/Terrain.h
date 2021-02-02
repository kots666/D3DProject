#ifndef Terrain_h__
#define Terrain_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CTexture;
class CRenderer;
class CTransform;
class CTerrainTex;
class CShader;

END

BEGIN(Client)
class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 device);
	virtual ~CTerrain();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

private:
	HRESULT AddComponent();
	HRESULT SetUpConstantTable(LPD3DXEFFECT& effect);

private:
	Engine::CTexture* m_texCom = nullptr;
	Engine::CTexture* m_normalTexCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	Engine::CTerrainTex* m_bufferCom = nullptr;
	Engine::CShader* m_shaderCom = nullptr;

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 device);

private:
	virtual void Free() override;

};

END

#endif
