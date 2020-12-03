#pragma once

#include "MapToolPage.h"
#include "ObjectToolPage.h"

// CSelectSheet

class CSelectSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSelectSheet)

public:
	CSelectSheet();
	CSelectSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSelectSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSelectSheet();

public:
	CMapToolPage* m_mapToolPage;
	CObjectToolPage* m_objToolPage;

protected:
	DECLARE_MESSAGE_MAP()
};


