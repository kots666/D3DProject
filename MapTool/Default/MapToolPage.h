#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMapToolPage 대화 상자입니다.

class CMapToolPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CMapToolPage)

public:
	CMapToolPage();
	virtual ~CMapToolPage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOLPAGE };
#endif

public:
	void ChangeType();
	void AddItem(const _int& index);
	void ChangeValue(const _int& cellIndex, const _int& vertexIndex, const _float& value, const _int& xyz);

public:
	CTreeCtrl m_treeCtrl;
	_int m_mode;
	_int m_naviIndex;
	_int m_vertexIndex;

	_float m_valueX;
	_float m_valueY;
	_float m_valueZ;

	CEdit m_editCtrlX;
	CEdit m_editCtrlY;
	CEdit m_editCtrlZ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedTerrainPicking();
	afx_msg void OnClickedVertexPicking();
	afx_msg void OnSelectedTreeControl(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnDeltaPosX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaPosY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaPosZ(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnChangeEditX();
	afx_msg void OnChangeEditY();
	afx_msg void OnChangeEditZ();
};
