#ifndef TestPlayer_h__
#define TestPlayer_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRCTex;
class CTexture;
class CTransform;

END

BEGIN(Client)
class CTestPlayer : public Engine::CGameObject
{
private:
	explicit CTestPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestPlayer(void);

public:
	virtual HRESULT Ready(void) override;
	virtual _int Update(const _float& fTimeDelta) override;
	virtual void Render(void) override;

private:
	HRESULT AddComponent(void);
	void Key_Input(const _float& fTimeDelta);

public:
	Engine::CRCTex*				m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_transCom = nullptr;

public:
	static CTestPlayer*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

END
#endif // TestPlayer_h__
