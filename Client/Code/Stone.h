#ifndef Stone_h__
#define Stone_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;

END

BEGIN(Client)
class CStone : public Engine::CGameObject
{
private:
	explicit CStone(LPDIRECT3DDEVICE9 device);
	virtual ~CStone();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

private:
	HRESULT AddComponent();
	void SetUpOnTerrain();
	_bool CollideToObject(const _tchar* layerTag, const _tchar* objTag);

private:
	Engine::CStaticMesh* m_meshCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	Engine::CCalculator* m_calcCom = nullptr;
	Engine::CCollider* m_colliderCom = nullptr;
	_bool m_isColl = false;

public:
	static CStone* Create(LPDIRECT3DDEVICE9 device);

private:
	virtual void Free() override;

};

END
#endif // Stone_h__
