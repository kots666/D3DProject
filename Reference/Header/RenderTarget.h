#ifndef RenderTarget_h__
#define RenderTarget_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTarget : public CBase
{
private:
	explicit CRenderTarget(LPDIRECT3DDEVICE9 device);
	virtual ~CRenderTarget();

public:
	HRESULT ReadyRenderTarget(const _uint& width, const _uint& height, D3DFORMAT format, D3DXCOLOR color);
	HRESULT ReadyDebugBuffer(const _float& x, const _float& y, const _float& sizeX, const _float& sizeY);
	
public:
	void Clear();
	void SetUpOnDevice(const _uint& index);	// ������ BeginScene
	void ReleaseOnDevice(const _uint& index);

	void ThrowRenderTargetTexture(LPD3DXEFFECT& effect, const char* constantName);

	void RenderDebugBuffer();

private:
	LPDIRECT3DDEVICE9 m_device;
	LPDIRECT3DTEXTURE9 m_targetTexture;

	LPDIRECT3DSURFACE9 m_targetSurface;			// ���� ����ϰ��� �ϴ� ȭ�� ���
	LPDIRECT3DSURFACE9 m_oldTargetSurface;		// ������ ��ġ�� ��ũ�Ǿ��ִ� ȭ�� �ؽ��� ��Ҹ� �ӽ������� �����ϱ� ���� ���۰���

private:
	D3DXCOLOR m_clearColor;
	LPDIRECT3DVERTEXBUFFER9 m_VB;
	LPDIRECT3DINDEXBUFFER9 m_IB;

public:
	static CRenderTarget* Create(LPDIRECT3DDEVICE9 device, const _uint& width, const _uint& height, D3DFORMAT format, D3DXCOLOR color);
	virtual void Free();

};
END
#endif // RenderTarget_h__
