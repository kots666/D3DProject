#pragma once
#include "afxcmn.h"


// CMapToolPage ��ȭ �����Դϴ�.

class CMapToolPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CMapToolPage)

public:
	CMapToolPage();
	virtual ~CMapToolPage();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOLPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CTreeCtrl m_treeCtrl;
};
