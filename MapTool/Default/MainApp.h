#ifndef MainApp_h__
#define MainApp_h__

#include "Base.h"

BEGIN(Engine)

class CGraphicDevice;
class CManagement;

END

class CMainApp : public CBase
{
// private ���� �Ҹ�
private:
	explicit CMainApp();
	virtual ~CMainApp();

// public �Լ� ����
public:
	HRESULT Ready();
	_int Update(const _float& deltaTime);
	void Render();

// protected �Լ� ����

// private �Լ� ����
private:
	HRESULT SetUpDefaultSetting(LPDIRECT3DDEVICE9* outerDevice);
	HRESULT ReadyScene(LPDIRECT3DDEVICE9 device, Engine::CManagement** outerManagement);

private:
	Engine::CGraphicDevice* m_deviceClass;
	Engine::CManagement* m_managementClass;
	LPDIRECT3DDEVICE9 m_device;

// ���� �Ҹ� �Լ�
public:
	static CMainApp* Create();
	virtual void Free();
};

#endif