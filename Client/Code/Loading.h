#ifndef Loading_h__
#define Loading_h__

#include "Define.h"
#include "Base.h"

BEGIN(Client)

class CLoading : public CBase 
{
public:
	enum LOADINGID {LOADING_STAGE, LOADING_BOSS, LOADING_END };

private:
	explicit CLoading(LPDIRECT3DDEVICE9 device);
	virtual ~CLoading();

public:
	LOADINGID GetLoadingID() const { return m_loadingID; }
	CRITICAL_SECTION* GetCRT() { return &m_crt; }
	_bool GetIsFinish() const { return m_isFinish; }
	_float  GetPercent() const { return (_float)m_nowLoad / (_float)m_maxLoad; }

public:
	static _uint CALLBACK ThreadMain(void* arg);

public:
	HRESULT Ready(LOADINGID loadID);
	_uint LoadingForStage();
	HRESULT LoadingForDynamicMeshNormalTextures();
	HRESULT LoadingForStaticMeshNormalTextures();
	HRESULT LoadingForStaticMeshes();

private:
	HANDLE m_thread;

	CRITICAL_SECTION m_crt;
	LOADINGID m_loadingID;
	LPDIRECT3DDEVICE9 m_device;
	_bool m_isFinish;

	_uint m_nowLoad = 0;
	_uint m_maxLoad = 185;

public:
	static CLoading* Create(LPDIRECT3DDEVICE9 device, LOADINGID loadID);

private:
	virtual void Free();

};

END

#endif
