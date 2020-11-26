#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"

class CMainFrame;
class CMFCToolView;

class CDynamicCamera : public Engine::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 device);
	virtual ~CDynamicCamera();

public:
	HRESULT Ready(const _vec3* eye, const _vec3* at, const _vec3* up,	
				const _float& fovY, const _float& aspect,
				const _float& nearZ, const _float& farZ);

	virtual _int Update(const _float& deltaTime) override;

public:
	void SetPickType(_int type) { m_pickType = type; }

public:
	const _int GetPickType() const { return m_pickType; }

private:
	void KeyInput(const _float& deltaTime);

	void MoveMouse();
	void FixMouse();
	void MousePicking();
	void CalcRay();

	void TerrainPicking();
	void NaviColliderPicking();

public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 device, 
									const _vec3* eye, 
									const _vec3* at, 
									const _vec3* up, 
									const _float& fovY = D3DXToRadian(60.f),
									const _float& aspect = _float(WINCX) / WINCY, 
									const _float& nearZ = 1.f, 
									const _float& farZ = 1000.f);

private:
	_bool m_canClick = false;
	_bool m_isFix = true;
	
	_vec3 m_rayDir = { 0.f, 0.f, 0.f };
	_float m_rayDist = 0.f;

	_int m_pickType = 0;

	CMainFrame* m_mainFrame;
	CMFCToolView* m_toolView;
	HWND m_hWnd;

private:
	virtual void Free() override;
};

#endif
