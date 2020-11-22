#pragma once
#include "afxwin.h"


// CUnitTool 대화 상자입니다.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPush();
	afx_msg void OnBnClickedAddUnitData();
	afx_msg void OnLbnSelChangeClass();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnEnChangeFindName();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();

public:
	CString m_inputData;
	CString m_outputData;

	// UnitData
	CString m_name;
	int m_atk;
	int m_def;
	CListBox m_listBox;

	map<CString, UNITINFO*> m_mapUnitData;
	
	CButton m_jobIndex[3];
	CButton m_itemIndex[3];
	CString m_findName;
};
