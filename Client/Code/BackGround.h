#ifndef BackGround_h__
#define BackGround_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRCTex;
class CTexture;
class CRenderer;
class CShader;
class CTransform;

END

BEGIN(Client)
class CBackGround : public Engine::CGameObject
{
private:
	explicit CBackGround(LPDIRECT3DDEVICE9 device);
	virtual ~CBackGround();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

	void SetPercent(const _float& percent) { m_percent = percent; }

private:
	HRESULT AddComponent();
	HRESULT SetUpConstantTable(LPD3DXEFFECT& effect);
	HRESULT SetUpConstantTableForProgressiveBar(LPD3DXEFFECT& effect);

private:
	Engine::CRCTex* m_bufferCom = nullptr;
	Engine::CTexture* m_texCom = nullptr;
	Engine::CTexture* m_progressiveTexCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;
	Engine::CShader* m_shaderCom = nullptr;

	_float m_percent = 0.f;
	
public:
	static CBackGround*	Create(LPDIRECT3DDEVICE9 device);

private:
	virtual void Free() override;
};

END
#endif
