// SelectSheet.cpp : ���� �����Դϴ�.
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

	m_objToolPage = new CObjectToolPage;
	AddPage(m_objToolPage);

	m_colliderPage = new CColliderPage;
	AddPage(m_colliderPage);
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

	if (nullptr != m_objToolPage)
	{
		delete m_objToolPage;
		m_objToolPage = nullptr;
	}

	if (nullptr != m_colliderPage)
	{
		delete m_colliderPage;
		m_colliderPage = nullptr;
	}
}


BEGIN_MESSAGE_MAP(CSelectSheet, CPropertySheet)
END_MESSAGE_MAP()


// CSelectSheet �޽��� ó�����Դϴ�.
