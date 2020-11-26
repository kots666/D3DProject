#pragma once


// CMapToolPage 대화 상자입니다.

class CMapToolPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CMapToolPage)

public:
	CMapToolPage();
	virtual ~CMapToolPage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOLPAG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
