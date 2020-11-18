#include "Texture.h"

USING(Engine)

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 device)
	: CResources(device)
{

}

Engine::CTexture::CTexture(const CTexture& rhs) : CResources(rhs)
{
	_uint size = rhs.m_texVec.size();
	m_texVec.reserve(size);

	m_texVec = rhs.m_texVec;

	for (_uint i = 0; i < size; ++i)
		SafeAddRef(m_texVec[i]);
}

Engine::CTexture::~CTexture()
{

}

HRESULT Engine::CTexture::ReadyTexture(const _tchar* path, TEXTURETYPE type, const _uint& cnt)
{
	m_texVec.reserve(cnt);

	LPDIRECT3DBASETEXTURE9 texture = nullptr;
	
	for (_uint i = 0; i < cnt; ++i)
	{
		_tchar fileName[256] = L"";

		wsprintf(fileName, path, i);

		switch (type)
		{
		case TEX_NORMAL:
			if (FAILED(D3DXCreateTextureFromFile(m_device, fileName, (LPDIRECT3DTEXTURE9*)&texture)))
				return E_FAIL;
			break;

		case TEX_CUBE:
			if (FAILED(D3DXCreateCubeTextureFromFile(m_device, fileName, (LPDIRECT3DCUBETEXTURE9*)&texture)))
				return E_FAIL;
			break;
		}

		m_texVec.push_back(texture);
	}

	return S_OK;
}

void Engine::CTexture::RenderTexture(const _uint& index /*= 0*/)
{
	if (m_texVec.size() < index)
		return;

	m_device->SetTexture(0, m_texVec[index]);
}

Engine::CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 device, const _tchar* path, TEXTURETYPE type, const _uint& cnt)
{
	CTexture* instance = new CTexture(device);

	if (FAILED(instance->ReadyTexture(path, type, cnt)))
		SafeRelease(instance);

	return instance;
}

CComponent * CTexture::Clone()
{
	return new CTexture(*this);
}

void Engine::CTexture::Free()
{
	CResources::Free();

	for_each(m_texVec.begin(), m_texVec.end(), CDeleteObj());

	m_texVec.clear();
	m_texVec.shrink_to_fit();
}

