#ifndef SkyBox_h__
#define SkyBox_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CStaticMesh;
class CRenderer;
class CTransform;
class CShader;

END

BEGIN(Client)
class CSkyBox : public Engine::CGameObject
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 device);
	virtual ~CSkyBox();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual _int LateUpdate(const _float& deltaTime) override;
	virtual void Render() override;

private:
	HRESULT AddComponent();
	HRESULT SetUpConstantTable(LPD3DXEFFECT& effect);

private:
	Engine::CStaticMesh* m_meshCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;
	Engine::CShader* m_shaderCom = nullptr;

public:
	static CSkyBox*	Create(LPDIRECT3DDEVICE9 device);

private:
	virtual void Free() override;

};

END

#endif