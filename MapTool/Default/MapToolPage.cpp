// MapToolPag.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "MapToolPage.h"
#include "afxdialogex.h"
#include "DynamicCamera.h"
#include "NaviMesh.h"
#include "NaviCell.h"

// CMapToolPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapToolPage, CPropertyPage)

CMapToolPage::CMapToolPage()
	: CPropertyPage(IDD_MAPTOOLPAGE)
	, m_pickingMode(0)
	, m_valueX(0)
	, m_valueY(0)
	, m_valueZ(0)
	, m_transMode(0)
{

}

CMapToolPage::~CMapToolPage()
{
}

void CMapToolPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeCtrl);
	DDX_Radio(pDX, IDC_RADIO1, m_pickingMode);
	DDX_Text(pDX, IDC_EDIT2, m_valueX);
	DDX_Text(pDX, IDC_EDIT3, m_valueY);
	DDX_Text(pDX, IDC_EDIT4, m_valueZ);
	DDX_Control(pDX, IDC_EDIT2, m_editCtrlX);
	DDX_Control(pDX, IDC_EDIT3, m_editCtrlY);
	DDX_Control(pDX, IDC_EDIT4, m_editCtrlZ);
	DDX_Radio(pDX, IDC_RADIO3, m_transMode);
}


BEGIN_MESSAGE_MAP(CMapToolPage, CPropertyPage)
	ON_BN_CLICKED(IDC_RADIO1, &CMapToolPage::OnClickedTerrainPicking)
	ON_BN_CLICKED(IDC_RADIO2, &CMapToolPage::OnClickedVertexPicking)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CMapToolPage::OnSelectedTreeControl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CMapToolPage::OnDeltaPosX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CMapToolPage::OnDeltaPosY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CMapToolPage::OnDeltaPosZ)
	ON_EN_CHANGE(IDC_EDIT2, &CMapToolPage::OnChangeEditX)
	ON_EN_CHANGE(IDC_EDIT3, &CMapToolPage::OnChangeEditY)
	ON_EN_CHANGE(IDC_EDIT4, &CMapToolPage::OnChangeEditZ)
	ON_BN_CLICKED(IDC_RADIO3, &CMapToolPage::OnClickedTogetherMode)
	ON_BN_CLICKED(IDC_RADIO4, &CMapToolPage::OnClickedSoloMode)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapToolPage::OnClickedDelete)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapToolPage::OnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON8, &CMapToolPage::OnClickedLoad)
END_MESSAGE_MAP()


// CMapToolPage 메시지 처리기입니다.


void CMapToolPage::ChangePickingModeType()
{
	dynamic_cast<CDynamicCamera*>(Engine::GetCurScene()->GetLayer(L"Environment")->GetGameObject(L"DynamicCamera"))->SetPickType(m_pickingMode);
}

void CMapToolPage::ChangeTransModeType()
{
	dynamic_cast<CDynamicCamera*>(Engine::GetCurScene()->GetLayer(L"Environment")->GetGameObject(L"DynamicCamera"))->SetTransType(m_transMode);
}

void CMapToolPage::AddItem(const _int& index)
{
	HTREEITEM root;

	TCHAR rootName[5] = L"";

	wsprintf(rootName, L"%d", index);

	root = m_treeCtrl.InsertItem(rootName, index, 999, TVI_ROOT, TVI_LAST);

	HTREEITEM child;

	child = m_treeCtrl.InsertItem(L"_0", index, 0, root, TVI_LAST);

	HTREEITEM child2;

	child2 = m_treeCtrl.InsertItem(L"_1", index, 1, root, TVI_LAST);

	HTREEITEM child3;

	child3 = m_treeCtrl.InsertItem(L"_2", index, 2, root, TVI_LAST);
}

void CMapToolPage::ChangeValue(const _int & cellIndex, const _int & vertexIndex, const _float & value, const _int & xyz)
{
	if (0 == m_transMode)
	{
		_vec3* pos = CNaviMesh::GetInstance()->GetPos(cellIndex, vertexIndex);
		if (nullptr == pos) return;

		_vec3 comparePos = *pos;
		_vec3 toChangePos = comparePos;

		switch (xyz)
		{
		case 0: toChangePos.x = value; break;
		case 1: toChangePos.y = value; break;
		case 2: toChangePos.z = value; break;
		}

		vector<CNaviCell*>* cellVec = CNaviMesh::GetInstance()->GetCellList();

		for (auto cell : *cellVec)
		{
			_vec3* vertices = cell->GetAllPos();

			for (_int i = 0; i < 3; ++i)
			{
				if (vertices[i] == comparePos)
				{
					vertices[i] = toChangePos;
				}
			}
		}
	}
	else
	{
		_vec3* pos = CNaviMesh::GetInstance()->GetPos(cellIndex, vertexIndex);
		if (nullptr == pos) return;

		switch (xyz)
		{
		case 0: pos->x = value; break;
		case 1: pos->y = value; break;
		case 2: pos->z = value; break;
		}
	}
}

void CMapToolPage::ChangeEditValue(const _float & valueX, const _float & valueY, const _float & valueZ)
{
	m_valueX = valueX;
	m_valueY = valueY;
	m_valueZ = valueZ;
}

void CMapToolPage::SelectPickedVertex(const _int & cellIndex, const _int & vertexIndex)
{
	UpdateData(TRUE);

	const _vec3* pos = CNaviMesh::GetInstance()->GetPos(cellIndex, vertexIndex);
	if (nullptr == pos) return;

	ChangeEditValue(pos->x, pos->y, pos->z);

	m_cellIndex = cellIndex;
	m_vertexIndex = vertexIndex;

	CNaviMesh::GetInstance()->ResetSelected();
	CNaviMesh::GetInstance()->SetIsSelected(cellIndex, vertexIndex, true);

	UpdateData(FALSE);
}

BOOL CMapToolPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMapToolPage::OnClickedTerrainPicking()
{
	m_pickingMode = 0;
	ChangePickingModeType();
}


void CMapToolPage::OnClickedVertexPicking()
{
	m_pickingMode = 1;
	ChangePickingModeType();
}


void CMapToolPage::OnSelectedTreeControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HTREEITEM itemRoot;
	HTREEITEM itemParent;
	HTREEITEM itemCur;

	CString strRoot;
	CString strParent;
	CString strCur;

	itemCur = pNMTreeView->itemNew.hItem;
	itemParent = m_treeCtrl.GetParentItem(itemCur);

	m_treeCtrl.GetItemImage(itemCur, m_cellIndex, m_vertexIndex);

	const _vec3* pos = CNaviMesh::GetInstance()->GetPos(m_cellIndex, m_vertexIndex);

	if (nullptr != pos)
	{
		CNaviMesh::GetInstance()->ResetSelected();
		CNaviMesh::GetInstance()->SetIsSelected(m_cellIndex, m_vertexIndex, true);
		ChangeEditValue(pos->x, pos->y, pos->z);
	}

	UpdateData(FALSE);

	*pResult = 0;
}


void CMapToolPage::OnDeltaPosX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	_float value = 0.1f * pNMUpDown->iDelta;

	m_valueX -= value;

	ChangeValue(m_cellIndex, m_vertexIndex, m_valueX, 0);

	UpdateData(FALSE);

	*pResult = 0;
}


void CMapToolPage::OnDeltaPosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	_float value = 0.1f * pNMUpDown->iDelta;

	m_valueY -= value;

	ChangeValue(m_cellIndex, m_vertexIndex, m_valueY, 1);

	UpdateData(FALSE);

	*pResult = 0;
}


void CMapToolPage::OnDeltaPosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	_float value = 0.1f * pNMUpDown->iDelta;

	m_valueZ -= value;

	ChangeValue(m_cellIndex, m_vertexIndex, m_valueZ, 2);

	UpdateData(FALSE);

	*pResult = 0;
}


void CMapToolPage::OnChangeEditX()
{
	UpdateData(TRUE);

	CString str;

	m_editCtrlX.GetWindowTextW(str);
	_float value = _tstof(str);

	m_valueX = value;

	ChangeValue(m_cellIndex, m_vertexIndex, m_valueX, 0);

	UpdateData(FALSE);
}


void CMapToolPage::OnChangeEditY()
{
	UpdateData(TRUE);

	CString str;

	m_editCtrlY.GetWindowTextW(str);
	_float value = _tstof(str);

	m_valueY = value;

	ChangeValue(m_cellIndex, m_vertexIndex, m_valueY, 1);

	UpdateData(FALSE);
}


void CMapToolPage::OnChangeEditZ()
{
	UpdateData(TRUE);

	CString str;

	m_editCtrlZ.GetWindowTextW(str);
	_float value = _tstof(str);

	m_valueZ = value;

	ChangeValue(m_cellIndex, m_vertexIndex, m_valueZ, 2);

	UpdateData(FALSE);
}


void CMapToolPage::OnClickedTogetherMode()
{
	m_transMode = 0;
	UpdateData(FALSE);
}


void CMapToolPage::OnClickedSoloMode()
{
	m_transMode = 1;
	UpdateData(FALSE);
}


void CMapToolPage::OnClickedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CNaviMesh::GetInstance()->DeleteCell(m_cellIndex);

	HTREEITEM item = m_treeCtrl.GetSelectedItem();

	m_treeCtrl.DeleteItem(item);

	m_treeCtrl.DeleteAllItems();

	vector<CNaviCell*>* cellVec = CNaviMesh::GetInstance()->GetCellList();

	int index = 0;

	for (auto& cell : *cellVec)
	{
		HTREEITEM root;

		TCHAR rootName[5] = L"";

		wsprintf(rootName, L"%d", index);

		root = m_treeCtrl.InsertItem(rootName, index, 999, TVI_ROOT, TVI_LAST);

		HTREEITEM child;

		child = m_treeCtrl.InsertItem(L"_0", index, 0, root, TVI_LAST);

		HTREEITEM child2;

		child2 = m_treeCtrl.InsertItem(L"_1", index, 1, root, TVI_LAST);

		HTREEITEM child3;

		child3 = m_treeCtrl.InsertItem(L"_2", index, 2, root, TVI_LAST);

		++index;
	}
}


void CMapToolPage::OnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

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

		vector<CNaviCell*>* cellVec = CNaviMesh::GetInstance()->GetCellList();

		for (auto& cell : *cellVec)
		{
			_vec3* posArr = cell->GetAllPos();

			for (_int i = 0; i < 3; ++i)
			{
				WriteFile(hFile, &posArr[i], sizeof(_vec3), &dwByte, nullptr);
			}
		}

		CloseHandle(hFile);
	}
}


void CMapToolPage::OnClickedLoad()
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

		DWORD dwByte = 0;

		CNaviMesh::GetInstance()->DeleteAllCell();
		m_treeCtrl.DeleteAllItems();

		while (true)
		{
			_vec3 pos[3];

			for (_int i = 0; i < 3; ++i)
			{
				ReadFile(hFile, &pos[i], sizeof(_vec3), &dwByte, nullptr);
				CNaviMesh::GetInstance()->AddPos(pos[i]);
			}

			if (0 == dwByte) break;
		}
		CloseHandle(hFile);
	}
	UpdateData(FALSE);
}
