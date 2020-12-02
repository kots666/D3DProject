#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CTriCol;

END

class CNaviCell : public Engine::CGameObject
{
private:
	explicit CNaviCell(LPDIRECT3DDEVICE9 device, _vec3 pos1, _vec3 pos2, _vec3 pos3);
	virtual ~CNaviCell();

public:
	virtual HRESULT Ready();
	virtual _int Update(const _float& deltaTime) override;
	virtual void Render();

public:
	_vec3* GetPos(const _int& vertexIndex);
	_vec3* GetAllPos() { return m_pos; }

public:
	void SetSelected(const _int& index, const _bool& isSelected) { m_isSelected[index] = isSelected; }

private:
	HRESULT AddComponent();

private:
	Engine::CTriCol* m_bufferCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	
	LPD3DXLINE m_line;

	_vec3 m_pos[3];
	LPD3DXMESH m_sphere[3];
	_bool m_isSelected[3];

public:
	static CNaviCell* Create(LPDIRECT3DDEVICE9 device, _vec3 pos1, _vec3 pos2, _vec3 pos3);

public:
	virtual void Free();
};

