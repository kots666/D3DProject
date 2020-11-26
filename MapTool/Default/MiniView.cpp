// MiniView.cpp : 구현 파일입니다.
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


// CMiniView 그리기입니다.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
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


// CMiniView 진단입니다.

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


// CMiniView 메시지 처리기입니다.


void CMiniView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMainFrame* mainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == mainFrame) return;

	//CMFCToolView* toolView = dynamic_cast<CMFCToolView*>(mainFrame->m_mainSplitter.GetPane(0, 1));
	//if (nullptr == toolView) return;

	//m_terrain = toolView->m_terrain;
	//if (nullptr == m_terrain) return;

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
