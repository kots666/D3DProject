#pragma once

// CMainView �� ���Դϴ�.

class CMainView : public CFormView
{
	DECLARE_DYNCREATE(CMainView)

protected:
	CMainView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMainView();

public:
	LPDIRECT3DDEVICE9 m_device;

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINVIEW };
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


