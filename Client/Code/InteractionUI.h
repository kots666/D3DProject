#ifndef InteractionUI_h__
#define InteractionUI_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRCTex;
class CTexture;
class CRenderer;
class CTransform;

END

BEGIN(Client)
class CInteractionUI : public Engine::CGameObject
{
private:
	explicit CInteractionUI(LPDIRECT3DDEVICE9 device);
	explicit CInteractionUI(LPDIRECT3DDEVICE9 device, const _tchar* texName);
	virtual ~CInteractionUI();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual _int LateUpdate(const _float& deltaTime) override;
	virtual void Render() override;

public:
	_bool GetActive() const { return m_isActive; }
	void SetActive(const _vec3& pos, const _float& xSize, const _float& ySize);
	void SetActive(const _bool& isActive) { m_isActive = isActive; }

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
	_vec3 m_pos;
	const _tchar* m_textureName;

	_float m_size[3];

	_bool m_isActive;

public:
	static CInteractionUI* Create(LPDIRECT3DDEVICE9 device, const _tchar* texName);

private:
	virtual void Free() override;

};

END
#endif // InteractionUI_h__
