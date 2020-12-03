#pragma once


// CObjectToolPage 대화 상자입니다.

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

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECTTOOLPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
