#ifndef Stage_h__
#define Stage_h__

#include "Scene.h"

class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 device);
	virtual ~CStage();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;
		
private:
	HRESULT ReadyEnvironmentLayer(const _tchar* layerTag);
	HRESULT ReadyGameLogicLayer(const _tchar* layerTag);
	HRESULT ReadyUILayer(const _tchar* layerTag);
	HRESULT	ReadyLightInfo();

	HRESULT InitResource(Engine::RESOURCETYPE type);

public:
	static CStage* Create(LPDIRECT3DDEVICE9 device);

private:
	virtual void Free() override;
};

#endif
