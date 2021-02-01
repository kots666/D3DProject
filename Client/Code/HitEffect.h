#ifndef HitEffect_h__
#define HitEffect_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRCTex;
class CTexture;
class CRenderer;
class CTransform;

END

BEGIN(Client)
class CHitEffect : public Engine::CGameObject
{
private:
	explicit CHitEffect(LPDIRECT3DDEVICE9 device);
	explicit CHitEffect(LPDIRECT3DDEVICE9 device,
		const _vec4& color,
		const _tchar* texName,
		const _int& uCnt,
		const _int& vCnt,
		const _bool& isRandRot,
		const _int& passIndex,
		const _float& lifeTime);
	explicit CHitEffect(LPDIRECT3DDEVICE9 device, const _vec3& pos, const _float& lifeTime);
	virtual ~CHitEffect();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

public:
	_bool GetActive() const { return m_isActive; }
	void SetActive(const _vec3& pos, const _float& xSize, const _float& ySize, const _float& lifeTime);

private:
	HRESULT AddComponent();
	HRESULT SetUpConstantTable(LPD3DXEFFECT& effect);

	void RandRotMatrix();

private:
	Engine::CRCTex* m_bufferCom = nullptr;
	Engine::CTexture* m_textureCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	Engine::CTransform* m_transformCom = nullptr;
	Engine::CShader* m_shaderCom = nullptr;

private:
	_matrix m_rotMat;
	_vec4 m_color;
	_vec3 m_pos;
	const _tchar* m_textureName;

	_float m_lifeTime;
	_float m_accTime;
	_float m_size[3];

	_float m_uOffset;
	_float m_vOffset;

	_int m_step;
	_int m_uStep;
	_int m_vStep;

	_int m_uCnt;
	_int m_vCnt;

	_int m_passIndex;

	_bool m_isActive;
	_bool m_isRandomRotation;

public:
	static CHitEffect* Create(LPDIRECT3DDEVICE9 device,
		const _vec4& color,
		const _tchar* texName,
		const _int& uCnt,
		const _int& vCnt,
		const _bool& isRandRot = false,
		const _int& passIndex = 0,
		const _float& lifeTime = 0.5f);

private:
	virtual void Free() override;

};

END
#endif // HitEffect_h__
