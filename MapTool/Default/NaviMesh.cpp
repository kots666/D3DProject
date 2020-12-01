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

void CNaviMesh::AddPos(_vec3 pos)
{
	m_tmpPos[m_count++] = pos;

	if (m_count >= 3)
	{
		m_count = 0;

		m_cellList.emplace_back(CNaviCell::Create(m_device, m_tmpPos[0], m_tmpPos[1], m_tmpPos[2]));

		CMainFrame* main = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		if (nullptr == main) return;

		CSheetView* selectView = dynamic_cast<CSheetView*>(main->m_mainSplitter.GetPane(0, 0));
		if (nullptr == selectView) return;

		selectView->m_selectSheet->m_mapToolPage->AddItem(m_indexCount);

		++m_indexCount;

		//CMapToolPage::GetActiveWindow()->Get AddItem()
	}
}

_vec3 * CNaviMesh::GetPos(const _int & cellIndex, const _int & vertexIndex)
{
	if (0 > cellIndex || cellIndex >= m_cellList.size()) return nullptr;

	return m_cellList[cellIndex]->GetPos(vertexIndex);
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
