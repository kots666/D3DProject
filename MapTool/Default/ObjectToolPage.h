#pragma once


// CObjectToolPage ��ȭ �����Դϴ�.

class CObjectToolPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CObjectToolPage)

public:
	CObjectToolPage();
	virtual ~CObjectToolPage();

public:
	virtual BOOL OnSetActive() override;
	virtual BOOL OnKillActive() override;

public:

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECTTOOLPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
