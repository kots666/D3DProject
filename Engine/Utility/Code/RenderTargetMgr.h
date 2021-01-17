#ifndef RenderTargetMgr_h__
#define RenderTargetMgr_h__

#include "Engine_Define.h"
#include "Base.h"

#include "RenderTarget.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTargetMgr : public CBase
{
	DECLARE_SINGLETON(CRenderTargetMgr)

private:
	explicit CRenderTargetMgr();
	virtual ~CRenderTargetMgr();

public:
	HRESULT ReadyRenderTarget(LPDIRECT3DDEVICE9 device, 
		const _tchar* targetTag, const _uint& width,
		const _uint& height, 
		D3DFORMAT format, 
		D3DXCOLOR color);

	HRESULT ReadyMRT(const _tchar* MRTTag, const _tchar* targetTag);

	HRESULT BeginMRT(const _tchar* MRTTag);
	HRESULT EndMRT(const _tchar* MRTTag);

	HRESULT ReadyDebugBuffer(const _tchar* targetTag, const _float& x, const _float& y, const _float& sizeX, const _float& sizeY);
	void RenderDebugBuffer(const _tchar* MRTTag);

	void ThrowRenderTargetTexture(LPD3DXEFFECT& effect, const _tchar* targetTag, const char* constantName);

private:
	CRenderTarget* FindRenderTarget(const _tchar* targetTag);
	list<CRenderTarget*>* FindMRT(const _tchar* MRTTag);
	_bool m_isRenderDebugBuffer;

private:
	map<const _tchar*, CRenderTarget*> m_RenderTargetMap;
	map<const _tchar*, list<CRenderTarget*>> m_MRTMap;

private:
	virtual void Free();
};

END
#endif // RenderTargetMgr_h__
