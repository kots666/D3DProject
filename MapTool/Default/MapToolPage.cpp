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
