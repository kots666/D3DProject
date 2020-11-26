// MainView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "MainView.h"
#include "MainFrm.h"

// CMainView

IMPLEMENT_DYNCREATE(CMainView, CFormView)

CMainView::CMainView()
	: CFormView(IDD_MAINVIEW)
{

}

CMainView::~CMainView()
{
}

void CMainView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainView, CFormView)
END_MESSAGE_MAP()


// CMainView 진단입니다.

#ifdef _DEBUG
void CMainView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainView 메시지 처리기입니다.


void CMainView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

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
		SafeRelease(initDevice);
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

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
