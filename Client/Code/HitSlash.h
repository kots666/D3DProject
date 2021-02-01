#ifndef HitSlash_h__
#define HitSlash_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRCTex;
class CTexture;
class CRenderer;
class CTransform;

END

BEGIN(Client)
class CHitSlash : public Engine::CGameObject
{
private:
	explicit CHitSlash(LPDIRECT3DDEVICE9 device);
	explicit CHitSlash(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& lifeTime);
	virtual ~CHitSlash();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

public:
	_bool GetActive() const { return m_isActive; }
	void SetActive(const _vec3& pos, const _float& size, const _float& lifeTime);

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
	_float m_lifeTime;
	_float m_accTime;
	_float m_offset;
	_float m_size;
	_int m_step;
	_bool m_isActive;

public:
	static CHitSlash* Create(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& lifeTime = 0.5f);

private:
	virtual void Free() override;

};

END
#endif // HitSlash_h__
