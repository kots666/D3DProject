#ifndef UI_h__
#define UI_h__

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
class CUI : public Engine::CGameObject
{
private:
	explicit CUI(LPDIRECT3DDEVICE9 device);
	explicit CUI(LPDIRECT3DDEVICE9 device, const _tchar* texName, const _float& x, const _float& y, const _float& sizeX, const _float& sizeY);
	virtual ~CUI();

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
	const _tchar* m_texName;
	_matrix m_projMat;
	_float m_wantedX, m_wantedY;
	_float m_left, m_top;
	_float m_sizeX, m_sizeY;

public:
	static CUI*	Create(LPDIRECT3DDEVICE9 device, const _tchar* texName, const _float& x, const _float& y, const _float& sizeX, const _float& sizeY);

private:
	virtual void Free() override;

};

END
#endif // UI_h__
