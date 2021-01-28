#ifndef TalkUI_h__
#define TalkUI_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRCTex;
class CTexture;
class CRenderer;
class CTransform;
class CShader;

END

BEGIN(Client)
class CTalkUI : public Engine::CGameObject
{
private:
	explicit CTalkUI(LPDIRECT3DDEVICE9 device);
	explicit CTalkUI(LPDIRECT3DDEVICE9 device, const _int& idx, const _float& x, const _float& y, const _float& sizeX, const _float& sizeY);
	virtual ~CTalkUI();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

private:
	HRESULT AddComponent();
	HRESULT SetUpConstantTable(LPD3DXEFFECT& effect);

private:
	Engine::CRCTex* m_bufferCom = nullptr;
	Engine::CTexture* m_textureCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	Engine::CTransform* m_transformCom = nullptr;
	Engine::CShader* m_shaderCom = nullptr;

private:
	_matrix m_projMat;

	_float m_wantedX, m_wantedY;
	_float m_left, m_top;
	_float m_sizeX, m_sizeY;

	_int m_index;

public:
	static CTalkUI*	Create(LPDIRECT3DDEVICE9 device, const _int& idx, const _float& x, const _float& y, const _float& sizeX, const _float& sizeY);

private:
	virtual void Free() override;

};

END
#endif // TalkUI_h__
