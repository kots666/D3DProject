// SheetView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "SheetView.h"
#include "SelectSheet.h"

// CSheetView

IMPLEMENT_DYNCREATE(CSheetView, CFormView)

CSheetView::CSheetView() :
	CFormView(IDD_SHEETVIEW),
	m_selectSheet(nullptr)
{

}

CSheetView::~CSheetView()
{
	if (nullptr != m_selectSheet)
	{
		delete m_selectSheet;
		m_selectSheet = nullptr;
	}
}

void CSheetView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSheetView, CFormView)
END_MESSAGE_MAP()


// CSheetView 진단입니다.

#ifdef _DEBUG
void CSheetView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSheetView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSheetView 메시지 처리기입니다.


void CSheetView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_selectSheet = new CSelectSheet;
	m_selectSheet->Create(this, WS_VISIBLE | WS_CHILD);
	m_selectSheet->MoveWindow(0, 0, 480, 600);

	CSize scrollSize = CSize{ 500, 600 };
	SetScrollSizes(MM_TEXT, scrollSize);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
