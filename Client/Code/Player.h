#ifndef Player_h__
#define Player_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CTrailRect;
class CTexture;
class CDynamicMesh;
class CNaviMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CShader;

END

BEGIN(Client)

class CDynamicCamera;
class CTerrain;
class CSwordTrail;
class CSwordDistort;

class CPlayer : public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 device);
	virtual ~CPlayer();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

public:
	virtual _bool AttackColliderOverlapped(Engine::CGameObject* target) override;
	virtual void HitColliderOverlapped(Engine::CGameObject* causer) override;

private:
	HRESULT AddComponent();
	HRESULT SetUpConstantTable(LPD3DXEFFECT& effect);
	HRESULT LoadCollider();
	void KeyInput(const _float& deltaTime);
	void SetUpOnTerrain();
	_vec3 PickUpOnTerrain();

	void UpdateAnimMatrices();
	void CalcComboTime(const _float& deltaTime);
	void CalcSkillTime(const _float& deltaTime);

	void CalcFrameTime(_float& outTime, const _float& frame);

	void LookAtMouseDirection();

public:
	void DoIdle();
	void DoAttack();
	void DoSkill01();
	void DoSkill02(); // ¿ø°Å¸®

private:
	Engine::CDynamicMesh* m_meshCom = nullptr;
	Engine::CNaviMesh* m_naviMeshCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;
	Engine::CCalculator* m_calcCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	Engine::CTexture* m_trailTexCom = nullptr;
	Engine::CShader* m_shaderCom = nullptr;
	Engine::CTexture* m_normalCom = nullptr;

	CDynamicCamera* m_camera = nullptr;

	CSwordTrail* m_swordTrail = nullptr;
	CSwordDistort* m_swordDistort = nullptr;

	_vec3 m_dir;
	_float m_yRotAngle;
	_float m_animationSpeed;
	_matrix m_yRotMat;
	_matrix m_yScaleRotMat;
	_matrix m_reviseMat;

	_bool m_isSkill = false;
	_bool m_isAttack = false;
	_bool m_isCombo = false;
	_int m_comboIndex = 0;
	_float m_accTime = 0.f;
	_float m_comboTime = 0.f;
	_float m_attackStartTime = 0.f;
	_float m_attackEndTime = 0.f;

	_bool m_isStartAttack = false;
	_bool m_isEndAttack = false;

	list<_float> m_skillStartTimeList;
	list<_float> m_skillEndTimeList;

	_bool m_isStartSkill = false;
	_bool m_isEndSkill = false;

	_int m_state;
	_int m_selSkill = 0;

private:
	void RecordPos();

public:
	static CPlayer*	Create(LPDIRECT3DDEVICE9 device);

private:
	virtual void Free() override;

};

END
#endif
