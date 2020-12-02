#include "stdafx.h"
#include "NaviMesh.h"
#include "NaviCell.h"
#include "MainFrm.h"
#include "SheetView.h"

IMPLEMENT_SINGLETON(CNaviMesh)

CNaviMesh::CNaviMesh() :
	m_count(0), m_indexCount(0), m_device(nullptr)
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

void CNaviMesh::AddPos(const _vec3& pos)
{
	m_tmpPos[m_count++] = pos;

	if (m_count >= 3)
	{
		m_count = 0;

		if (0 <= CheckCCW(m_tmpPos)) return;

		m_cellList.emplace_back(CNaviCell::Create(m_device, m_tmpPos[0], m_tmpPos[1], m_tmpPos[2]));

		CMainFrame* main = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		if (nullptr == main) return;

		CSheetView* selectView = dynamic_cast<CSheetView*>(main->m_mainSplitter.GetPane(0, 0));
		if (nullptr == selectView) return;

		selectView->m_selectSheet->m_mapToolPage->AddItem(m_indexCount);

		++m_indexCount;
	}
}

void CNaviMesh::DeleteCell(const _int & index)
{
	--m_indexCount;
	m_cellList[index]->Release();

	m_cellList.erase(m_cellList.begin() + index);
}

void CNaviMesh::DeleteAllCell()
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

	m_count = 0;
	m_indexCount = 0;
}

_vec3 * CNaviMesh::GetPos(const _int & cellIndex, const _int & vertexIndex)
{
	if (0 > cellIndex || cellIndex >= m_cellList.size()) return nullptr;

	return m_cellList[cellIndex]->GetPos(vertexIndex);
}

void CNaviMesh::ResetSelected()
{
	for (auto cell : m_cellList)
	{
		for (_int i = 0; i < 3; ++i)
			cell->SetSelected(i, false);
	}
}

void CNaviMesh::SetIsSelected(const _int & cellIndex, const _int & vertexIndex, const _bool & isSelected)
{
	m_cellList[cellIndex]->SetSelected(vertexIndex, isSelected);
}

_int CNaviMesh::CheckCCW(const _vec3 * posArray)
{
	_int tmpSum = posArray[0].x * posArray[1].z + posArray[1].x * posArray[2].z + posArray[2].x * posArray[0].z;

	tmpSum = tmpSum - posArray[0].z * posArray[1].x - posArray[1].z * posArray[2].x - posArray[2].z * posArray[0].x;

	if (0 < tmpSum) return 1;
	else if (0 > tmpSum) return -1;
	else return 0;
}

void CNaviMesh::Release()
{
	DeleteAllCell();

	SafeRelease(m_device);
}
