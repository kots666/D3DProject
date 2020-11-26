// MainView.cpp : ���� �����Դϴ�.
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


// CMainView �����Դϴ�.

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


// CMainView �޽��� ó�����Դϴ�.


void CMainView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

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

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}
