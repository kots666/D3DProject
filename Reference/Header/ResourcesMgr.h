#ifndef ResourcesMgr_h__
#define ResourcesMgr_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CResourcesMgr : public CBase 
{
	DECLARE_SINGLETON(CResourcesMgr)

private:
	explicit CResourcesMgr();
	virtual ~CResourcesMgr();

public:
	HRESULT	Reserve(const _ushort& size);
	HRESULT	ReadyBuffer(LPDIRECT3DDEVICE9 device,
				const _ushort& containerIdx,
				const _tchar* bufferTag, 
				BUFFERID ID, 
				_ulong cntX = 1, 
				_ulong cntZ = 1, 
				_ulong interval = 1);
	HRESULT ReadyTexture(LPDIRECT3DDEVICE9 device,
		const _ushort& containerIdx,
		const _tchar* textureTag,
		TEXTURETYPE type,
		const _tchar* filePath,
		const _uint& cnt = 1);
	HRESULT ReadyMesh(LPDIRECT3DDEVICE9 device,
		const _ushort& containerIdx,
		const _tchar* meshTag,
		MESHTYPE type,
		const _tchar* filePath,
		const _tchar* fileName);

	void RenderBuffer(const _ushort& containerIdx, const _tchar* bufferTag);
	void RenderTexture(const _ushort& containerIdx, const _tchar* textureTag, const _uint& texIndex);

	CComponent* Clone(const _ushort& containerIndex, const _tchar* resourceTag);

private:
	CResources* Find(const _ushort& index, const _tchar* resourcesTag);

private:
	map<const _tchar*, CResources*>* m_resourceMap;
	_ushort m_size = 0;

public:
	virtual void Free();
};

END

#endif
