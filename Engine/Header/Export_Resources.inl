#include "Export_Resources.h"
// ResourcesMgr
// Get
// Set
// General

CComponent * CloneResource(const _ushort & containerIndex, const _tchar * resourceTag)
{
	return CResourcesMgr::GetInstance()->Clone(containerIndex, resourceTag);
}

HRESULT ReserveContainerSize(const _ushort& size)
{
	return CResourcesMgr::GetInstance()->Reserve(size);
}

HRESULT ReadyBuffer(LPDIRECT3DDEVICE9 device, const _ushort& containerIdx, const _tchar* bufferTag, BUFFERID ID, _ulong cntX, _ulong cntZ, _ulong interval)
{
	return CResourcesMgr::GetInstance()->ReadyBuffer(device, containerIdx, bufferTag, ID, cntX, cntZ, interval);
}

HRESULT ReadyTexture(LPDIRECT3DDEVICE9 device, const _ushort& containerIdx, const _tchar* textureTag, TEXTURETYPE type, const _tchar* filePath, const _uint& cnt, const _bool* isBlue)
{
	return CResourcesMgr::GetInstance()->ReadyTexture(device, containerIdx, textureTag, type, filePath, cnt, isBlue);
}

inline HRESULT ReadyMesh(LPDIRECT3DDEVICE9 device, const _ushort & containerIdx, const _tchar * meshTag, MESHTYPE type, const _tchar * filePath, const _tchar * fileName)
{
	return CResourcesMgr::GetInstance()->ReadyMesh(device, containerIdx, meshTag, type, filePath, fileName);
}

void RenderBuffer(const _ushort& index, const _tchar* bufferTag)
{
	CResourcesMgr::GetInstance()->RenderBuffer(index, bufferTag);
}

void RenderTexture(const _ushort& index, const _tchar* textureTag, const _uint& texIndex)
{
	CResourcesMgr::GetInstance()->RenderTexture(index, textureTag, texIndex);
}

// Release Resources
void ReleaseResoures()
{
	CResourcesMgr::DestroyInstance();
}
