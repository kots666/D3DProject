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

private:
	HRESULT AddComponent();

private:
	Engine::CTriCol* m_bufferCom = nullptr;
	Engine::CTransform* m_transCom = nullptr;
	Engine::CRenderer* m_rendererCom = nullptr;
	
	LPD3DXLINE m_line;

	_vec3 m_pos[3];

public:
	static CNaviCell* Create(LPDIRECT3DDEVICE9 device, _vec3 pos1, _vec3 pos2, _vec3 pos3);

public:
	virtual void Free();
};

