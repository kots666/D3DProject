#pragma once

#include "SelectSheet.h"

// CSheetView �� ���Դϴ�.

class CSheetView : public CFormView
{
	DECLARE_DYNCREATE(CSheetView)

protected:
	CSheetView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CSheetView();

public:
	CSelectSheet* m_selectSheet;

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHEETVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


