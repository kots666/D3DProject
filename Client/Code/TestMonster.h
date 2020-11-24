#ifndef TestMonster_h__
#define TestMonster_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRCTex;
class CTexture;
class CTransform;

END

BEGIN(Client)

class CTestPlayer;

class CTestMonster : public Engine::CGameObject
{
private:
	explicit CTestMonster(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* player);
	virtual ~CTestMonster(void);

public:
	virtual HRESULT Ready(void) override;
	virtual _int Update(const _float& fTimeDelta) override;
	virtual void Render(void) override;

private:
	HRESULT AddComponent(void);
	void Chase(const _float& fTimeDelta);

private:
	Engine::CRCTex*				m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_transCom = nullptr;
	CGameObject* m_player = nullptr;
	CTestPlayer* player = nullptr;

	_float m_fSpeed = 3.f;


public:
	static CTestMonster*	Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* player);

private:
	virtual void Free(void) override;

};

END
#endif // TestMonster_h__
