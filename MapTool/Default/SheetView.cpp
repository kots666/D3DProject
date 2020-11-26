// SheetView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "SheetView.h"
#include "SelectSheet.h"

// CSheetView

IMPLEMENT_DYNCREATE(CSheetView, CFormView)

CSheetView::CSheetView()
	: CFormView(IDD_SHEETVIEW)
{

}

CSheetView::~CSheetView()
{
}

void CSheetView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSheetView, CFormView)
END_MESSAGE_MAP()


// CSheetView �����Դϴ�.

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


// CSheetView �޽��� ó�����Դϴ�.


void CSheetView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_selectSheet = new CSelectSheet;
	m_selectSheet->Create(this, WS_VISIBLE | WS_CHILD);
	m_selectSheet->MoveWindow(0, 0, 300, 600);

	CSize scrollSize = CSize{ 300, 600 };
	SetScrollSizes(MM_TEXT, scrollSize);
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}
