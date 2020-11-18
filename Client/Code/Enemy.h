#ifndef Enemy_h__
#define Enemy_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRCTex;
class CTexture;
class CTransform;
class CRenderer;
class CCalculator;

END

BEGIN(Client)

class CTerrain;

class CEnemy : public Engine::CGameObject
{
private:
	explicit CEnemy(LPDIRECT3DDEVICE9 device);
	virtual ~CEnemy();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

	void SetBillboardMatrix();

private:
	HRESULT AddComponent();

private:
	Engine::CRCTex* m_bufferCom = nullptr;
	Engine::CTexture* m_texCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;
	Engine::CCalculator* m_calcCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;

public:
	static CEnemy*	Create(LPDIRECT3DDEVICE9 device);

private:
	virtual void Free() override;

};

END
#endif
