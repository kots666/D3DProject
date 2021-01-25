#ifndef FontUI_h__
#define FontUI_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRCTex;
class CTexture;
class CRenderer;
class CTransform;

END

BEGIN(Client)
class CFontUI : public Engine::CGameObject
{
private:
	explicit CFontUI(LPDIRECT3DDEVICE9 device);
	explicit CFontUI(LPDIRECT3DDEVICE9 device, const _tchar* texName, const _int& index, const _float& sizeX, const _float& sizeY);
	virtual ~CFontUI();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

public:
	void Active(const _vec3& pos, const _vec3& offset, const _float& xSize = 1.f, const _float& ySize = 1.f, const _float& lifeTime = 2.f);

	// Getter
	_bool GetActive() const { return m_isActive; }
	_float GetSize() const { return m_sizeX; }

	// Setter
	void SetActive(const _bool& isActive) { m_isActive = isActive; }
	void SetSize(const _float& size) { m_sizeX = size; m_sizeY = size; }

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
	_vec3 m_startPos;
	_vec3 m_offset;
	_float m_sizeX, m_sizeY;
	_float m_percent = 0.f;

	_float m_accTime;
	_float m_lifeTime;

	_int m_index;

	_bool m_isActive;

public:
	static CFontUI* Create(LPDIRECT3DDEVICE9 device, const _tchar* texName, const _int& index, const _float& sizeX, const _float& sizeY);

private:
	virtual void Free() override;

};

END
#endif // FontUI_h__
