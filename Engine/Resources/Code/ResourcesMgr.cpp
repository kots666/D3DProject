#include "ResourcesMgr.h"
#include "TriCol.h"
#include "RCTex.h"
#include "Texture.h"
#include "TerrainTex.h"
#include "CubeTex.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"

USING(Engine)
IMPLEMENT_SINGLETON(CResourcesMgr)

CResourcesMgr::CResourcesMgr() :
	m_resourceMap(nullptr)
{

}

CResourcesMgr::~CResourcesMgr()
{
	Free();
}

HRESULT CResourcesMgr::Reserve(const _ushort& size)
{
	if (nullptr != m_resourceMap)
		return E_FAIL;

	m_size = size;

	m_resourceMap = new map<const _tchar*, CResources*>[size];

	return S_OK;
}

HRESULT CResourcesMgr::ReadyBuffer(LPDIRECT3DDEVICE9 device, const _ushort& containerIdx, const _tchar* bufferTag, BUFFERID ID, _ulong cntX /*= 1*/, _ulong cntZ /*= 1*/, _ulong interval /*= 1*/)
{
	if (nullptr == m_resourceMap)
	{
		MSG_BOX("Resource Container not Reserved");
		return E_FAIL;
	}

	CResources*	resources = Find(containerIdx, bufferTag);
	if (nullptr != resources) return E_FAIL;

	switch (ID)
	{
	case BUFFER_TRICOL:
		resources = CTriCol::Create(device);
		break;

	case BUFFER_RCTEX:
		resources = CRCTex::Create(device);
		break;

	case BUFFER_TERRAINTEX:
		resources = CTerrainTex::Create(device, cntX, cntZ, interval);
		break;

	case BUFFER_CUBETEX:
		resources = CCubeTex::Create(device);
		break;
	}

	NULL_CHECK_RETURN(resources, E_FAIL);
	
	m_resourceMap[containerIdx].emplace(bufferTag, resources);

	return S_OK;
}

HRESULT CResourcesMgr::ReadyTexture(LPDIRECT3DDEVICE9 device, const _ushort & containerIdx, const _tchar * textureTag, TEXTURETYPE type, const _tchar * filePath, const _uint & cnt)
{
	if (nullptr == m_resourceMap)
	{
		MSG_BOX("Resource Container not Reserved");
		return E_FAIL;
	}

	CResources*	resources = Find(containerIdx, textureTag);
	if (nullptr != resources)
		return E_FAIL;

	resources = CTexture::Create(device, filePath, type, cnt);
	NULL_CHECK_RETURN(resources, E_FAIL);

	m_resourceMap[containerIdx].emplace(textureTag, resources);

	return S_OK;
}

HRESULT CResourcesMgr::ReadyMesh(LPDIRECT3DDEVICE9 device, const _ushort & containerIdx, const _tchar * meshTag, MESHTYPE type, const _tchar * filePath, const _tchar * fileName)
{
	if (nullptr == m_resourceMap)
	{
		MSG_BOX("Resource Container not Reserved");
		return E_FAIL;
	}

	CResources* resources = Find(containerIdx, meshTag);
	if (nullptr != resources)
		return E_FAIL;

	switch (type)
	{
	case TYPE_STATIC:
		resources = CStaticMesh::Create(device, filePath, fileName);
		break;

	case TYPE_DYNAMIC:
		resources = CDynamicMesh::Create(device, filePath, fileName);
		break;

	case TYPE_NAVI:
		break;
	}

	if (nullptr == resources)
		return E_FAIL;

	m_resourceMap[containerIdx].emplace(meshTag, resources);

	return S_OK;
}

void CResourcesMgr::RenderBuffer(const _ushort& containerIdx, const _tchar* bufferTag)
{
	CResources* resources = Find(containerIdx, bufferTag);

	if (nullptr == resources) return;

	dynamic_cast<CVIBuffer*>(resources)->Render();
}

void CResourcesMgr::RenderTexture(const _ushort & containerIdx, const _tchar * textureTag, const _uint & texIndex)
{
	CResources* resources = Find(containerIdx, textureTag);

	if (nullptr == resources) return;

	dynamic_cast<CTexture*>(resources)->RenderTexture(texIndex);
}

CComponent * CResourcesMgr::Clone(const _ushort & containerIndex, const _tchar * resourceTag)
{
	if (nullptr == m_resourceMap) return nullptr;

	CResources* comp = Find(containerIndex, resourceTag);
	if (nullptr == comp)
		return nullptr;

	return comp->Clone();
}

CResources* CResourcesMgr::Find(const _ushort& index, const _tchar* resourcesTag)
{
	auto iter = find_if(m_resourceMap[index].begin(),
		m_resourceMap[index].end(),
		CTagFinder(resourcesTag));

	if (m_resourceMap[index].end() == iter) return nullptr;

	return iter->second;
}

void CResourcesMgr::Free()
{
	for (_uint i = 0; i < m_size; ++i)
	{
		for_each(m_resourceMap[i].begin(), m_resourceMap[i].end(), CDeleteMap());
		m_resourceMap[i].clear();
	}
	SafeDeleteArray(m_resourceMap);
}