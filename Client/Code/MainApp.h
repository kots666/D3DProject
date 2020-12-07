#ifndef MainApp_h__
#define MainApp_h__

#include "Define.h"
#include "Base.h"

BEGIN(Engine)

class CGraphicDevice;
class CManagement;

END

class CMainApp : public CBase
{
// private 생성 소멸
private:
	explicit CMainApp();
	virtual ~CMainApp();

// public 함수 변수
public:
	HRESULT Ready();
	_int Update(const _float& deltaTime);
	_int LateUpdate(const _float& deltaTime);
	void Render();

// protected 함수 변수

// private 함수 변수
private:
	HRESULT SetUpDefaultSetting(LPDIRECT3DDEVICE9* outerDevice);
	HRESULT ReadyScene(LPDIRECT3DDEVICE9 device, Engine::CManagement** outerManagement);

private:
	Engine::CGraphicDevice* m_deviceClass;
	Engine::CManagement* m_managementClass;
	LPDIRECT3DDEVICE9 m_device;

	_float m_accTime = 0.f;
	_ulong m_frame = 0;
	_tchar m_fps[256];

// 생성 소멸 함수
public:
	static CMainApp* Create();
	virtual void Free();
};

#endif