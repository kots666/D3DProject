#ifndef Player_h__
#define Player_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;

END

BEGIN(Client)

class CTerrain;

class CPlayer : public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 device);
	virtual ~CPlayer();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

private:
	HRESULT AddComponent();
	void KeyInput(const _float& deltaTime);
	void SetUpOnTerrain();
	_vec3 PickUpOnTerrain();

	void UpdateAnimMatrices();

private:
	Engine::CDynamicMesh* m_meshCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;
	Engine::CCalculator* m_calcCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	Engine::CCollider* m_colliderCom = nullptr;

	_vec3 m_dir;
	_float m_speed = 5.f;
	_float m_yRotAngle;
	_matrix m_yRotMat;
	_matrix m_yScaleRotMat;

public:
	static CPlayer*	Create(LPDIRECT3DDEVICE9 device);

private:
	virtual void Free() override;

};

END
#endif
