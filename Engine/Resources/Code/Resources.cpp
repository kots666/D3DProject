#include "Resources.h"

USING(Engine)

CResources::CResources(LPDIRECT3DDEVICE9 device)
	: m_device(device)
{
	SafeAddRef(device);
}

CResources::CResources(const CResources& rhs)
	: m_device(rhs.m_device)
{
	SafeAddRef(m_device);
}

CResources::~CResources()
{
}

void CResources::Free()
{
	SafeRelease(m_device);
}

