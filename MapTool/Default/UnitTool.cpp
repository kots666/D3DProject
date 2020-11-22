// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_inputData(_T(""))
	, m_outputData(_T(""))
	, m_name(_T(""))
	, m_atk(0)
	, m_def(0)
	, m_findName(_T(""))
{

}

CUnitTool::~CUnitTool()
{
	for (auto& pair : m_mapUnitData)
	{
		SafeDelete(pair.second);
	}
	m_mapUnitData.clear();
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_inputData);
	DDX_Text(pDX, IDC_EDIT2, m_outputData);
	DDX_Text(pDX, IDC_EDIT3, m_name);
	DDX_Text(pDX, IDC_EDIT4, m_atk);
	DDX_Text(pDX, IDC_EDIT5, m_def);
	DDX_Control(pDX, IDC_LIST2, m_listBox);
	DDX_Control(pDX, IDC_RADIO1, m_jobIndex[0]);
	DDX_Control(pDX, IDC_RADIO2, m_jobIndex[1]);
	DDX_Control(pDX, IDC_RADIO3, m_jobIndex[2]);
	DDX_Control(pDX, IDC_CHECK1, m_itemIndex[0]);
	DDX_Control(pDX, IDC_CHECK2, m_itemIndex[1]);
	DDX_Control(pDX, IDC_CHECK3, m_itemIndex[2]);
	DDX_Text(pDX, IDC_EDIT6, m_findName);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedPush)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnBnClickedAddUnitData)
	ON_LBN_SELCHANGE(IDC_LIST2, &CUnitTool::OnLbnSelChangeClass)
	ON_BN_CLICKED(IDC_BUTTON3, &CUnitTool::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnBnClickedLoad)
	ON_EN_CHANGE(IDC_EDIT6, &CUnitTool::OnEnChangeFindName)
END_MESSAGE_MAP()


// CUnitTool �޽��� ó�����Դϴ�.


void CUnitTool::OnBnClickedPush()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	UpdateData(TRUE); // DoDataExchange�� ȣ�����ִ� �Լ�. TRUE�� ������ �����Ϳ� �����͸� �޾ƿ´�.
	m_outputData = m_inputData;
	UpdateData(FALSE); // FALSE�� ������ �����͸� ��������.
}


void CUnitTool::OnBnClickedAddUnitData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	UNITINFO* newUnit = new UNITINFO;
	newUnit->name = m_name;
	newUnit->atk = m_atk;
	newUnit->def = m_def;

	for (int i = 0; i < 3; ++i)
	{
		if (m_jobIndex[i].GetCheck())
		{
			newUnit->jobIndex = i;
			break;
		}
	}

	newUnit->itemIndex = 0;

	m_listBox.AddString(m_name.GetString());

	m_mapUnitData.emplace(newUnit->name, newUnit);
	UpdateData(FALSE);
}


void CUnitTool::OnLbnSelChangeClass()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	for (int i = 0; i < 3; ++i)
	{
		m_jobIndex[i].SetCheck(0);
		m_itemIndex[i].SetCheck(0);
	}
	CString name = L"";
	int selectIndex = m_listBox.GetCurSel();
	if (LB_ERR == selectIndex) return;

	m_listBox.GetText(selectIndex, name);

	auto& findIter = m_mapUnitData.find(name);
	if (m_mapUnitData.end() == findIter) return;

	m_name = findIter->second->name;
	m_atk = findIter->second->atk;
	m_def = findIter->second->def;
	m_jobIndex[findIter->second->jobIndex].SetCheck(1);

	UpdateData(FALSE);
}


BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_jobIndex[0].SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CUnitTool::OnBnClickedDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	int selectIndex = m_listBox.GetCurSel();

	auto& findIter = m_mapUnitData.find(m_name);

	if (m_mapUnitData.end() == findIter) return;

	SafeDelete(findIter->second);
	m_mapUnitData.erase(m_name);
	m_listBox.DeleteString(selectIndex);

	m_name = L"";
	m_findName = L"";
	m_atk = 0;
	m_def = 0;

	for (int i = 0; i < 3; ++i)
	{
		m_jobIndex[i].SetCheck(0);
		m_itemIndex[i].SetCheck(0);
	}

	UpdateData(FALSE);
}

void CUnitTool::OnEnChangeFindName()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	for (int i = 0; i < 3; ++i)
	{
		m_jobIndex[i].SetCheck(0);
		m_itemIndex[i].SetCheck(0);
	}
	auto& findIter = m_mapUnitData.find(m_findName);

	if (m_mapUnitData.end() == findIter)
		return;

	int findIndex = m_listBox.FindString(0, m_findName);

	if (LB_ERR == findIndex) return;

	m_listBox.SetCurSel(findIndex);

	m_name = m_findName;
	m_atk = findIter->second->atk;
	m_def = findIter->second->def;
	m_jobIndex[findIter->second->jobIndex].SetCheck(1);

	UpdateData(FALSE);
}

void CUnitTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog dialog(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data File(*.dat) | *.dat||", this);

	TCHAR curPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, curPath);
	PathRemoveFileSpec(curPath);
	PathCombine(curPath, curPath, L"Data");

	dialog.m_ofn.lpstrInitialDir = curPath;

	if (IDOK == dialog.DoModal())
	{
		CString filePath = dialog.GetPathName();

		HANDLE hFile = CreateFile(filePath.GetString(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile) return;

		DWORD bytes = 0;
		DWORD stringBytes = 0;

		for (auto& pair : m_mapUnitData)
		{
			stringBytes = sizeof(TCHAR) * (pair.first.GetLength() + 1);
			WriteFile(hFile, &stringBytes, sizeof(DWORD), &bytes, nullptr);
			WriteFile(hFile, pair.first.GetString(), stringBytes, &bytes, nullptr);
			WriteFile(hFile, &pair.second->atk, sizeof(pair.second->atk), &bytes, nullptr);
			WriteFile(hFile, &pair.second->def, sizeof(pair.second->def), &bytes, nullptr);
			WriteFile(hFile, &pair.second->jobIndex, sizeof(pair.second->jobIndex), &bytes, nullptr);
			WriteFile(hFile, &pair.second->itemIndex, sizeof(pair.second->itemIndex), &bytes, nullptr);
		}

		CloseHandle(hFile);
	}
}


void CUnitTool::OnBnClickedLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	CFileDialog dialog(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data File(*.dat) | *.dat||", this);

	TCHAR curPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, curPath);
	PathRemoveFileSpec(curPath);
	PathCombine(curPath, curPath, L"Data");

	dialog.m_ofn.lpstrInitialDir = curPath;

	if (IDOK == dialog.DoModal())
	{
		CString filePath = dialog.GetPathName();

		HANDLE hFile = CreateFile(filePath.GetString(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile) return;

		DWORD bytes = 0;
		DWORD stringBytes = 0;

		UNITINFO* newUnit = nullptr;

		while (true)
		{
			ReadFile(hFile, &stringBytes, sizeof(DWORD), &bytes, nullptr);

			TCHAR* name = new TCHAR[stringBytes];

			newUnit = new UNITINFO;
			ReadFile(hFile, name, stringBytes, &bytes, nullptr);

			newUnit->name = name;
			SafeDeleteArray(name);

			ReadFile(hFile, &newUnit->atk, sizeof(newUnit->atk), &bytes, nullptr);
			ReadFile(hFile, &newUnit->def, sizeof(newUnit->def), &bytes, nullptr);
			ReadFile(hFile, &newUnit->jobIndex, sizeof(newUnit->jobIndex), &bytes, nullptr);
			ReadFile(hFile, &newUnit->itemIndex, sizeof(newUnit->itemIndex), &bytes, nullptr);

			if (0 == bytes)
			{
				SafeDelete(newUnit);
				break;
			}
			// �ʿ� �߰�
			m_mapUnitData.emplace(newUnit->name, newUnit);

			// ����Ʈ�� �߰�
			for (int i = 0; i < 3; ++i)
			{
				m_jobIndex[i].SetCheck(0);
				m_itemIndex[i].SetCheck(0);
			}

			m_listBox.AddString(newUnit->name);
			m_atk = newUnit->atk;
			m_def = newUnit->def;
			m_jobIndex[newUnit->jobIndex].SetCheck(1);
		}
		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}