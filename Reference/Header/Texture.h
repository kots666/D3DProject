#ifndef Texture_h__
#define Texture_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CResources
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 device);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();


public:
	HRESULT	ReadyTexture(const _tchar* path, TEXTURETYPE type, const _uint& cnt);
	void RenderTexture(const _uint& index = 0);

public:
	void SetTexture(LPD3DXEFFECT& effect, const char* constantName, const _uint& index = 0);

private:
	vector<IDirect3DBaseTexture9*> m_texVec;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 device, const _tchar* path, TEXTURETYPE type, const _uint& cnt);
	CComponent* Clone();
	virtual void Free();
};
END
#endif
