#include "Mesh.h"

USING(Engine)

CMesh::CMesh(LPDIRECT3DDEVICE9 device) :
	CResources(device), m_isClone(false)
{

}

CMesh::CMesh(const CMesh& rhs) :
	CResources(rhs), m_isClone(true)
{

}

CMesh::~CMesh()
{

}

void CMesh::Free()
{
	CResources::Free();
}

