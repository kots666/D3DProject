#pragma once
#include "MapTool.h"

// CMyFormView �� ���Դϴ�.

class CMyFormView : public CFormView
{
	DECLARE_DYNCREATE(CMyFormView)

protected:
	CMyFormView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyFormView();
public:
	//CUnitTool m_unitTool;
	CMapTool m_mapTool;

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORMVIEW };
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
	afx_msg void OnBnClickedUnitTool();
	afx_msg void OnBnClickedMapTool();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
};


