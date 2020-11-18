#ifndef Export_Resouces_h__
#define Export_Resouces_h__

#include "ResourcesMgr.h"

BEGIN(Engine)

// ResourcesMgr
// Get
inline CComponent* CloneResource(const _ushort& containerIndex, const _tchar* resourceTag);
// Set
// General

inline HRESULT ReserveContainerSize(const _ushort& size);
inline HRESULT ReadyBuffer(LPDIRECT3DDEVICE9 device, const _ushort& containerIdx, const _tchar* bufferTag, BUFFERID ID, _ulong cntX = 1, _ulong cntZ = 1, _ulong interval = 1);
inline HRESULT ReadyTexture(LPDIRECT3DDEVICE9 device, const _ushort& containerIdx, const _tchar* textureTag, TEXTURETYPE type, const _tchar* filePath, const _uint& cnt = 1);
inline HRESULT ReadyMesh(LPDIRECT3DDEVICE9 device, const _ushort & containerIdx, const _tchar * meshTag, MESHTYPE type, const _tchar * filePath, const _tchar * fileName);

inline void RenderBuffer(const _ushort& index, const _tchar* bufferTag);
inline void RenderTexture(const _ushort& index, const _tchar* textureTag, const _uint& texIndex);

// Release Resources
inline void ReleaseResoures();

#include "Export_Resources.inl"

END

#endif
