#include "stdafx.h"
#include "NaviMesh.h"
#include "NaviCell.h"

IMPLEMENT_SINGLETON(CNaviMesh)

CNaviMesh::CNaviMesh() :
	m_count(0), m_device(nullptr)
{
	Ready();
}

CNaviMesh::~CNaviMesh()
{
	Release();
}

HRESULT CNaviMesh::Ready()
{
	for (int i = 0; i < 3; ++i)
		m_tmpPos[i] = { 0.f, 0.f, 0.f };

	m_cellList.reserve(50);

	m_device = CGraphicDevice::GetInstance()->GetDevice();
	SafeAddRef(m_device);

	return S_OK;
}

_int CNaviMesh::Update(const _float & deltaTime)
{
	for (auto& elem : m_cellList)
	{
		return elem->Update(deltaTime);
	}
}

void CNaviMesh::Render()
{
	for (auto& elem : m_cellList)
	{
		elem->Render();
	}
}

void CNaviMesh::AddPos(_vec3 pos)
{
	m_tmpPos[m_count++] = pos;

	if (m_count >= 3)
	{
		m_count = 0;

		m_cellList.emplace_back(CNaviCell::Create(m_device, m_tmpPos[0], m_tmpPos[1], m_tmpPos[2]));
	}
}

void CNaviMesh::Release()
{
	for (auto& elem : m_cellList)
	{
		if (nullptr != elem)
		{
			elem->Release();
		}
	}

	m_cellList.clear();
	m_cellList.shrink_to_fit();

	SafeRelease(m_device);
}
