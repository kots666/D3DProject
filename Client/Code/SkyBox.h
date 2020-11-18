#ifndef SkyBox_h__
#define SkyBox_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CCubeTex;
class CTexture;
class CRenderer;
class CTransform;

END

BEGIN(Client)
class CSkyBox : public Engine::CGameObject
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 device);
	virtual ~CSkyBox();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

private:
	HRESULT AddComponent();

private:
	Engine::CCubeTex* m_bufferCom = nullptr;
	Engine::CTexture* m_texCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;

public:
	static CSkyBox*	Create(LPDIRECT3DDEVICE9 device);

private:
	virtual void Free() override;

};

END

#endif