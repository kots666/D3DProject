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
	HRESULT	ReadyTexture(const _tchar* path, TEXTURETYPE type, const _uint& cnt, const _bool* isBlue);
	void RenderTexture(const _uint& index = 0);

public:
	_uint GetSize() { return m_texVec.size(); }
	_bool IsBlue(const _int& index);

	void SetTexture(LPD3DXEFFECT& effect, const char* constantName, const _uint& index = 0);

private:
	vector<IDirect3DBaseTexture9*> m_texVec;
	vector<_bool> m_blueVec;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 device, const _tchar* path, TEXTURETYPE type, const _uint& cnt, const _bool* isBlue = nullptr);
	CComponent* Clone();
	virtual void Free();
};
END
#endif
