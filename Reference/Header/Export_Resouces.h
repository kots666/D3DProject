#ifndef Export_Resouces_h__
#define Export_Resouces_h__

#include "ResourcesMgr.h"

BEGIN(Engine)

// ResourcesMgr
// Get
// Set
// General

inline HRESULT Reserve_ContainerSize(const _ushort& size);
inline HRESULT Ready_Buffer(LPDIRECT3DDEVICE9 device, const _ushort& index, const _tchar* bufferTag, BUFFERID ID, _ulong cntX = 1, _ulong cntZ = 1, _ulong interval = 1);
inline void Render_Buffer(const _ushort& index, const _tchar* bufferTag);

// Release Resources
inline void Release_Resoures();

#include "Export_Resouces.inl"

END

#endif
