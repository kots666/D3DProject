// SelectSheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "SelectSheet.h"


// CSelectSheet

IMPLEMENT_DYNAMIC(CSelectSheet, CPropertySheet)

CSelectSheet::CSelectSheet()
{
	m_mapToolPage = new CMapToolPage;
	AddPage(m_mapToolPage);
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
	if (nullptr != m_mapToolPage)
	{
		delete m_mapToolPage;
		m_mapToolPage = nullptr;
	}
}


BEGIN_MESSAGE_MAP(CSelectSheet, CPropertySheet)
END_MESSAGE_MAP()


// CSelectSheet 메시지 처리기입니다.
