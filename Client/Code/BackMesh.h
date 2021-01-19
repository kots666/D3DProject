#ifndef BackMesh_h__
#define BackMesh_h__

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
class CBackMesh : public Engine::CGameObject
{
private:
	explicit CBackMesh(LPDIRECT3DDEVICE9 device, _tchar* key, _tchar* name, const _vec3& pos, const _vec3& scale, const _vec3& rot);
	virtual ~CBackMesh();

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
	_tchar* m_key;

	_vec3 m_pos;
	_vec3 m_scale;
	_vec3 m_rotation;

public:
	static CBackMesh* Create(LPDIRECT3DDEVICE9 device, _tchar* key, _tchar* name, const _vec3& pos, const _vec3& scale, const _vec3& rot);

private:
	virtual void Free() override;

};

END

#endif // BackMesh_h__
