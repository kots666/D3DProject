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

public:
	void ChangeType();
	void AddItem(int index);

public:
	CTreeCtrl m_treeCtrl;
	_int m_mode;
	_int m_naviIndex;
	_int m_vertexIndex;

	_float m_valueX;
	_float m_valueY;
	_float m_valueZ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
};
