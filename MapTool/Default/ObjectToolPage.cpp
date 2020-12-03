// ObjectToolPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "ObjectToolPage.h"
#include "afxdialogex.h"


// CObjectToolPage 대화 상자입니다.

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


// CObjectToolPage 메시지 처리기입니다.
