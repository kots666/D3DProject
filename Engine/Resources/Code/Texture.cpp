#include "Texture.h"

USING(Engine)

CTexture::CTexture(LPDIRECT3DDEVICE9 device) :
	CResources(device)
{

}

CTexture::CTexture(const CTexture& rhs) :
	CResources(rhs)
{
	_uint size = rhs.m_texVec.size();
	m_texVec.reserve(size);

	m_texVec = rhs.m_texVec;

	for (_uint i = 0; i < size; ++i)
		SafeAddRef(m_texVec[i]);

	_uint boolSize = rhs.m_blueVec.size();
	m_blueVec.reserve(boolSize);

	m_blueVec = rhs.m_blueVec;
}

CTexture::~CTexture()
{

}

HRESULT CTexture::ReadyTexture(const _tchar* path, TEXTURETYPE type, const _uint& cnt, const _bool* isBlue)
{
	m_texVec.reserve(cnt);
	m_blueVec.reserve(cnt);

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

		if (nullptr == isBlue)
			m_blueVec.emplace_back(true);
		else
			m_blueVec.emplace_back(isBlue[i]);
	}

	return S_OK;
}

void CTexture::RenderTexture(const _uint& index /*= 0*/)
{
	if (m_texVec.size() < index)
		return;

	m_device->SetTexture(0, m_texVec[index]);
}

_bool CTexture::IsBlue(const _int & index)
{
	if (index >= m_blueVec.size()) return true;

	return m_blueVec[index];
}

void CTexture::SetTexture(LPD3DXEFFECT & effect, const char * constantName, const _uint & index)
{
	if (m_texVec.size() < index)
		return;

	effect->SetTexture(constantName, m_texVec[index]);
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 device, const _tchar* path, TEXTURETYPE type, const _uint& cnt, const _bool* isBlue)
{
	CTexture* instance = new CTexture(device);

	if (FAILED(instance->ReadyTexture(path, type, cnt, isBlue)))
		SafeRelease(instance);

	return instance;
}

CComponent * CTexture::Clone()
{
	return new CTexture(*this);
}

void CTexture::Free()
{
	CResources::Free();

	for_each(m_texVec.begin(), m_texVec.end(), CDeleteObj());

	m_texVec.clear();
	m_texVec.shrink_to_fit();
}

