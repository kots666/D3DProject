// SelectSheet.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "SelectSheet.h"


// CSelectSheet

IMPLEMENT_DYNAMIC(CSelectSheet, CPropertySheet)

CSelectSheet::CSelectSheet()
{
	AddPage(&m_mapToolPage);
}

CSelectSheet::CSelectSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CSelectSheet::CSelectSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CSelectSheet::~CSelectSheet()
{
}


BEGIN_MESSAGE_MAP(CSelectSheet, CPropertySheet)
END_MESSAGE_MAP()


// CSelectSheet �޽��� ó�����Դϴ�.
