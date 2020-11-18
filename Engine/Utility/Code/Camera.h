#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 device);
	virtual ~CCamera();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;

protected:
	_matrix	m_matView, m_matProj;
	_vec3 m_eye, m_at, m_up;
	_float m_fovY, m_aspect, m_nearZ, m_farZ;

protected:
	virtual void Free();

};

END

#endif
