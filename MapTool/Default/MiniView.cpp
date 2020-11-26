// MiniView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "MiniView.h"
#include "MainFrm.h"

// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{
	m_device = CGraphicDevice::GetInstance()->GetDevice();
	SafeAddRef(m_device);
}

CMiniView::~CMiniView()
{
	SafeRelease(m_device);
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()


// CMiniView �׸����Դϴ�.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
	CGraphicDevice::GetInstance()->RenderBegin();

	_matrix matScale;
	float xScale = 800.f / 32.f;
	float yScale = 1200.f / (32.f * 32.f);
	//float xScale = (float)WINCX / (TILECX * TILEX);
	//float yScale = (float)WINCY * 2.f / (TILECY * TILEY);
	D3DXMatrixScaling(&matScale, xScale, yScale, 0.f);
	//m_terrain->MiniRender();
	CGraphicDevice::GetInstance()->RenderEnd(m_hWnd);
}


// CMiniView �����Դϴ�.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView �޽��� ó�����Դϴ�.


void CMiniView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMainFrame* mainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == mainFrame) return;

	//CMFCToolView* toolView = dynamic_cast<CMFCToolView*>(mainFrame->m_mainSplitter.GetPane(0, 1));
	//if (nullptr == toolView) return;

	//m_terrain = toolView->m_terrain;
	//if (nullptr == m_terrain) return;

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}
