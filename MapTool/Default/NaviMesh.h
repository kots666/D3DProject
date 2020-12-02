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
	void AddPos(const _vec3& pos);
	void DeleteCell(const _int& index);
	void DeleteAllCell();
	
public:
	_vec3* GetPos(const _int& cellIndex, const _int& vertexIndex);
	vector<CNaviCell*>* GetCellList() { return &m_cellList; }

public:
	void ResetSelected();
	void SetIsSelected(const _int& cellIndex, const _int& vertexIndex, const _bool& isSelected);
	
public:
	_int CheckCCW(const _vec3* posArray);

private:
	void Release();

private:
	vector<CNaviCell*> m_cellList;
	_vec3 m_tmpPos[3];
	_int m_count;
	_int m_indexCount;
	LPDIRECT3DDEVICE9 m_device;
};

