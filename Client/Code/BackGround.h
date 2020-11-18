#ifndef BackGround_h__
#define BackGround_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRCTex;
class CTexture;
class CRenderer;

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

private:
	HRESULT AddComponent();

private:
	Engine::CRCTex* m_bufferCom = nullptr;
	Engine::CTexture* m_texCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	
public:
	static CBackGround*	Create(LPDIRECT3DDEVICE9 device);

private:
	virtual void Free() override;
};

END
#endif
