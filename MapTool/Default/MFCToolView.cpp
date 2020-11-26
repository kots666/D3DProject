
// MFCToolView.cpp : CMFCToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCTool.h"
#endif

#include "MFCToolDoc.h"
#include "MFCToolView.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "MyFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd;

// CMFCToolView

IMPLEMENT_DYNCREATE(CMFCToolView, CScrollView)

BEGIN_MESSAGE_MAP(CMFCToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCToolView 생성/소멸

CMFCToolView::CMFCToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CMFCToolView::CMFCToolView(const CMFCToolView & rhs)
{
	m_device = rhs.m_device;
	SafeAddRef(m_device);
}

CMFCToolView::~CMFCToolView()
{
	SafeRelease(m_device);
	//SafeDelete(m_terrain);
}

BOOL CMFCToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CMFCToolView 그리기

void CMFCToolView::OnDraw(CDC* /*pDC*/)
{
	CMFCToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CGraphicDevice::GetInstance()->RenderBegin();
	// 이 사이에 그림을 그린다.
	//m_terrain->Render();

	CGraphicDevice::GetInstance()->RenderEnd();
}


// CMFCToolView 인쇄

BOOL CMFCToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFCToolView 진단

#ifdef _DEBUG
void CMFCToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMFCToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMFCToolDoc* CMFCToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCToolDoc)));
	return (CMFCToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCToolView 메시지 처리기


void CMFCToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(32 * 20, (32 >> 1) * 20));

	CMainFrame* mainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	if (nullptr == mainFrame) return;

	RECT rcMain = {};

	// 메인 프레임의 윈도우 크기를 얻어옴 (스크린좌표)
	mainFrame->GetWindowRect(&rcMain);

	// 스크린좌표를 클라이언트 좌표로 변환
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	// View의 크기
	RECT rcView = {};
	GetClientRect(&rcView);

	float gapX = rcMain.right - rcView.right;
	float gapY = rcMain.bottom - rcView.bottom;
	
	mainFrame->SetWindowPos(nullptr, 0, 0, WINCX + gapX, WINCY + gapY, SWP_NOZORDER);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	g_hWnd = m_hWnd;

	Engine::CGraphicDevice* initDevice;

	if (FAILED(CGraphicDevice::GetInstance()->Ready(m_hWnd, WINMODE::MODE_WIN, WINCX, WINCY, &initDevice)))
	{
		//MSG_BOX("Graphic Device Create Failed");
		return;
	}
	SafeAddRef(initDevice);

	m_device = initDevice->GetDevice();
	SafeAddRef(m_device);
	SafeRelease(initDevice);

	if (FAILED(ReadyDirectInput(AfxGetInstanceHandle(), m_hWnd)))
	{
		return;
	}

	/*
	if (FAILED(CTextureManager::GetInstance()->InsertTexture(CTextureManager::TEX_MULTI, L"../Texture/Line/%d.png", L"Tile", L"Line", LINECOUNT)))
		return;

	if (FAILED(CTextureManager::GetInstance()->InsertTexture(CTextureManager::TEX_MULTI, L"../Texture/Map/%d.png", L"FullMap", L"Map", MAPCOUNT)))
		return;
	
	if (nullptr == m_terrain)
	{
		m_terrain = new CTerrain();
		m_terrain->SetView(this);
	}
	*/
}

void CMFCToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	_vec3 pos = { float(point.x) + GetScrollPos(0), float(point.y) + GetScrollPos(1), 0.f };

	CMainFrame* main = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (nullptr == main) return;

	CMiniView* miniView = dynamic_cast<CMiniView*>(main->m_mainSplitter.GetPane(0, 0));
	if (nullptr == miniView) return;
	
	//CMyFormView* myFormView = dynamic_cast<CMyFormView*>(main->m_subSplitter.GetPane(1, 0));
	//if (nullptr == myFormView) return;

	//int tileID = myFormView->m_mapTool.m_drawID;

	//m_terrain->TileChange(m_pos, tileID, 0);

	//InvalidateRect(nullptr, false);

	//miniView->InvalidateRect(nullptr, false);

	CScrollView::OnLButtonDown(nFlags, point);
}
