#ifndef HPUI_h__
#define HPUI_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRCTex;
class CTexture;
class CRenderer;
class CTransform;

END

BEGIN(Client)
class CHPUI : public Engine::CGameObject
{
private:
	explicit CHPUI(LPDIRECT3DDEVICE9 device);
	explicit CHPUI(LPDIRECT3DDEVICE9 device, Engine::CGameObject* target, const _tchar* texName, const _float& x, const _float& y, const _float& sizeX, const _float& sizeY, const _bool& isFixed, const _bool& isFloat);
	virtual ~CHPUI();

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
	_float m_percent = 0.f;

	Engine::CGameObject* m_target;

	_bool m_isFixed;
	_bool m_isFloat;

	_float m_hp;
	_float m_maxHp;

public:
	static CHPUI* Create(LPDIRECT3DDEVICE9 device, Engine::CGameObject* target, const _tchar* texName, const _float& x, const _float& y, const _float& sizeX, const _float& sizeY, const _bool& isFixed = true, const _bool& isFloat = false);

private:
	virtual void Free() override;

};

END
#endif // HPUI_h__
