
// MFCToolView.cpp : CMFCToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCToolView ����/�Ҹ�

CMFCToolView::CMFCToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CMFCToolView �׸���

void CMFCToolView::OnDraw(CDC* /*pDC*/)
{
	CMFCToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CGraphicDevice::GetInstance()->RenderBegin();
	// �� ���̿� �׸��� �׸���.
	//m_terrain->Render();

	CGraphicDevice::GetInstance()->RenderEnd();
}


// CMFCToolView �μ�

BOOL CMFCToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFCToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFCToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMFCToolView ����

#ifdef _DEBUG
void CMFCToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMFCToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMFCToolDoc* CMFCToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCToolDoc)));
	return (CMFCToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCToolView �޽��� ó����


void CMFCToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(32 * 20, (32 >> 1) * 20));

	CMainFrame* mainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	if (nullptr == mainFrame) return;

	RECT rcMain = {};

	// ���� �������� ������ ũ�⸦ ���� (��ũ����ǥ)
	mainFrame->GetWindowRect(&rcMain);

	// ��ũ����ǥ�� Ŭ���̾�Ʈ ��ǥ�� ��ȯ
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	// View�� ũ��
	RECT rcView = {};
	GetClientRect(&rcView);

	float gapX = rcMain.right - rcView.right;
	float gapY = rcMain.bottom - rcView.bottom;
	
	mainFrame->SetWindowPos(nullptr, 0, 0, WINCX + gapX, WINCY + gapY, SWP_NOZORDER);

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
