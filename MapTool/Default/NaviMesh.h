#pragma once

class CNaviCell;

class CNaviMesh
{
	DECLARE_SINGLETON(CNaviMesh)

private:
	explicit CNaviMesh();
	virtual ~CNaviMesh();

public:
	virtual HRESULT Ready();
	virtual _int Update(const _float& deltaTime);
	virtual void Render();

public:
	void AddPos(_vec3 pos);

private:
	void Release();

private:
	vector<CNaviCell*> m_cellList;
	_vec3 m_tmpPos[3];
	_int m_count;
	LPDIRECT3DDEVICE9 m_device;
};

