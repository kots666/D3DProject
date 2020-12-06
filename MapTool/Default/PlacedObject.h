#ifndef PlacedObject_h__
#define PlacedObject_h__

#include "GameObject.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;

END

class CPlacedObject : public Engine::CGameObject
{
private:
	explicit CPlacedObject(LPDIRECT3DDEVICE9 device, _tchar* key, _tchar* tag);
	virtual ~CPlacedObject();

public:
	virtual HRESULT Ready() override;
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render() override;

public:
	void SetPos(const _vec3& pos) { m_pos = pos; }
	void SetPosX(const _float& x) { m_pos.x = x; }
	void SetPosY(const _float& y) { m_pos.y = y; }
	void SetPosZ(const _float& z) { m_pos.z = z; }

	void SetScale(const _vec3& scale) { m_scale = scale; }
	void SetScaleX(const _float& x) { m_scale.x = x; }
	void SetScaleY(const _float& y) { m_scale.y = y; }
	void SetScaleZ(const _float& z) { m_scale.z = z; }

	void SetRotation(const _vec3& rotation) { m_rotation = rotation; }
	void SetRotationX(const _float& x) { m_rotation.x = x; }
	void SetRotationY(const _float& y) { m_rotation.y = y; }
	void SetRotationZ(const _float& z) { m_rotation.z = z; }

public:
	_vec3* GetPosAddr() { return &m_pos; }
	_vec3* GetScaleAddr() { return &m_scale; }
	_vec3* GetRotationAddr() { return &m_rotation; }
	_tchar* GetTag() { return m_tag; }

private:
	HRESULT AddComponent();
	//void SetUpOnTerrain();

private:
	Engine::CStaticMesh* m_meshCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;

	_vec3 m_pos;
	_vec3 m_scale;
	_vec3 m_rotation;

	_tchar* m_key;
	_tchar* m_tag;

public:
	static CPlacedObject* Create(LPDIRECT3DDEVICE9 device, _tchar* key, _tchar* tag);

private:
	virtual void Free() override;

};

#endif // PlacedObject_h__
