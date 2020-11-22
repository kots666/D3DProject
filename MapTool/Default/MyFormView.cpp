// MyFormView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "MyFormView.h"

// CMyFormView

IMPLEMENT_DYNCREATE(CMyFormView, CFormView)

CMyFormView::CMyFormView()
	: CFormView(IDD_MYFORMVIEW)
{

}

CMyFormView::~CMyFormView()
{
}

void CMyFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyFormView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyFormView::OnBnClickedUnitTool)
	ON_BN_CLICKED(IDC_BUTTON6, &CMyFormView::OnBnClickedMapTool)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyFormView::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON7, &CMyFormView::OnBnClickedLoad)
END_MESSAGE_MAP()


// CMyFormView 진단입니다.

#ifdef _DEBUG
void CMyFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyFormView 메시지 처리기입니다.


void CMyFormView::OnBnClickedUnitTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//if (nullptr == m_unitTool.GetSafeHwnd()) m_unitTool.Create(IDD_UNITTOOL);
	//m_unitTool.ShowWindow(SW_SHOW);
}


void CMyFormView::OnBnClickedMapTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (nullptr == m_mapTool.GetSafeHwnd()) m_mapTool.Create(IDD_MAPTOOL);
	m_mapTool.ShowWindow(SW_SHOW);
}


void CMyFormView::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data File(*.dat) | *.dat||", this);
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Data");
	Dlg.m_ofn.lpstrInitialDir = szDataPath;

	if (IDOK == Dlg.DoModal())
	{
		CString strPath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strPath.GetString(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;

		/*const list<CLine*>* lineList = CLineManager::GetInstance()->GetList();

		for (auto& line : *lineList)
		{
			_vec3 startPos = line->GetStartPos();
			_vec3 endPos = line->GetEndPos();
			int lineID = line->GetID();

			WriteFile(hFile, &startPos, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, &endPos, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, &lineID, sizeof(int), &dwByte, nullptr);
		}*/
		CloseHandle(hFile);
	}
}


void CMyFormView::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data File(*.dat) | *.dat||", this);
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Data");
	Dlg.m_ofn.lpstrInitialDir = szDataPath;
	if (IDOK == Dlg.DoModal())
	{
		CString strPath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strPath.GetString(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		/*DWORD dwByte = 0;
		while (true)
		{
			_vec3 startPos, endPos;
			int lineID;

			ReadFile(hFile, &startPos, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &endPos, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &lineID, sizeof(int), &dwByte, nullptr);

			if (0 == dwByte)
			{
				break;
			}

			CLineManager::GetInstance()->AddLine(new CLine(startPos, endPos, lineID));
		}*/
		CloseHandle(hFile);
	}
	UpdateData(FALSE);
}
