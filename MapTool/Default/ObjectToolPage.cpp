// ObjectToolPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "ObjectToolPage.h"
#include "afxdialogex.h"


// CObjectToolPage ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CObjectToolPage, CPropertyPage)

CObjectToolPage::CObjectToolPage()
	: CPropertyPage(IDD_OBJECTTOOLPAGE)
{

}

CObjectToolPage::~CObjectToolPage()
{
}

BOOL CObjectToolPage::OnSetActive()
{
	return CPropertyPage::OnSetActive();
}

BOOL CObjectToolPage::OnKillActive()
{
	return CPropertyPage::OnKillActive();
}

void CObjectToolPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CObjectToolPage, CPropertyPage)
END_MESSAGE_MAP()


// CObjectToolPage �޽��� ó�����Դϴ�.
