#ifndef Logo_h__
#define Logo_h__

#include "Define.h"
#include "Scene.h"

BEGIN(Client)

class CLoading;
class CBackGround;

class CLogo : public Engine::CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 device);
	virtual ~CLogo();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

private:
	HRESULT ReadyEnvironmentLayer(const _tchar* layerTag);
	HRESULT ReadyGameLogicLayer(const _tchar* layerTag);
	HRESULT ReadyUILayer(const _tchar* layerTag);

	HRESULT InitResource(Engine::RESOURCETYPE type);

private:
	CLoading* m_loading;
	CBackGround* m_backGround;

public:
	static CLogo* Create(LPDIRECT3DDEVICE9 device);

private:
	virtual void Free() override;
};

END
#endif
