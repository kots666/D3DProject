#pragma once

#include "MapToolPage.h"

// CSelectSheet

class CSelectSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSelectSheet)

public:
	CSelectSheet();
	CSelectSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSelectSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSelectSheet();

private:
	CMapToolPage m_mapToolPage;

protected:
	DECLARE_MESSAGE_MAP()
};


