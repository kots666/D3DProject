// MapToolPag.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "MapToolPage.h"
#include "afxdialogex.h"


// CMapToolPage ��ȭ �����Դϴ�.

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


// CMapToolPage �޽��� ó�����Դϴ�.


BOOL CMapToolPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	HTREEITEM root;

	root = m_treeCtrl.InsertItem(L"Root", 0, 0, TVI_ROOT, TVI_LAST);

	HTREEITEM child;

	child = m_treeCtrl.InsertItem(L"Child", 0, 0, root, TVI_LAST);

	HTREEITEM child2;

	child2 = m_treeCtrl.InsertItem(L"Child2", 0, 0, root, TVI_LAST);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
