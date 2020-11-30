// MapToolPag.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "MapToolPage.h"
#include "afxdialogex.h"


// CMapToolPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapToolPage, CPropertyPage)

CMapToolPage::CMapToolPage()
	: CPropertyPage(IDD_MAPTOOLPAGE)
{

}

CMapToolPage::~CMapToolPage()
{
}

void CMapToolPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeCtrl);
}


BEGIN_MESSAGE_MAP(CMapToolPage, CPropertyPage)
END_MESSAGE_MAP()


// CMapToolPage 메시지 처리기입니다.


BOOL CMapToolPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	HTREEITEM root;

	root = m_treeCtrl.InsertItem(L"Root", 0, 0, TVI_ROOT, TVI_LAST);

	HTREEITEM child;

	child = m_treeCtrl.InsertItem(L"Child", 0, 0, root, TVI_LAST);

	HTREEITEM child2;

	child2 = m_treeCtrl.InsertItem(L"Child2", 0, 0, root, TVI_LAST);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
