#ifndef SkillRock_h__
#define SkillRock_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CShader;
class CTexture;

END

BEGIN(Client)
class CSkillRock : public Engine::CGameObject
{
private:
	explicit CSkillRock(LPDIRECT3DDEVICE9 device, _tchar* name, const _vec3& pos);
	virtual ~CSkillRock();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

private:
	HRESULT AddComponent();
	HRESULT SetUpConstantTable(LPD3DXEFFECT& effect);

private:
	Engine::CStaticMesh* m_meshCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	Engine::CShader* m_shaderCom = nullptr;
	Engine::CTexture* m_normalTexCom = nullptr;

	_tchar* m_name;

	_vec3 m_pos;

public:
	static CSkillRock* Create(LPDIRECT3DDEVICE9 device, _tchar* name, const _vec3& pos);

private:
	virtual void Free() override;

};

END

#endif // SkillRock_h__
