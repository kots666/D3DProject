#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"

class CMainFrame;
class CMainView;

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
	void SetTransType(_int type) { m_transType = type; }

public:
	const _int GetPickType() const { return m_pickType; }

private:
	void KeyInput(const _float& deltaTime);

	void MoveMouse();
	void FixMouse();
	void MousePicking();
	void CalcRay();

	void TerrainPicking();
	void FindNearlyPoint(_vec3* outPos);
	void NaviColliderPicking();
	_bool IsCollideToSphere(const _vec3& rayPos, const _vec3& rayDir, const _vec3& centerPos, const _float& radius);

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
	
	_vec3 m_rayPos = { 0.f, 0.f, 0.f };
	_vec3 m_rayDir = { 0.f, 0.f, 0.f };

	_int m_pickType = 0;
	_int m_transType = 0;

	_float m_speed = 20.f;

	CMainFrame* m_mainFrame;
	CMainView* m_toolView;
	HWND m_hWnd;

private:
	virtual void Free() override;
};

#endif
