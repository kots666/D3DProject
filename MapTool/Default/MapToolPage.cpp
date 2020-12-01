// MapToolPag.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "MapToolPage.h"
#include "afxdialogex.h"
#include "DynamicCamera.h"
#include "NaviMesh.h"

// CMapToolPage ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMapToolPage, CPropertyPage)

CMapToolPage::CMapToolPage()
	: CPropertyPage(IDD_MAPTOOLPAGE)
	, m_mode(0)
	, m_valueX(0)
	, m_valueY(0)
	, m_valueZ(0)
{

}

CMapToolPage::~CMapToolPage()
{
}

void CMapToolPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeCtrl);
	DDX_Radio(pDX, IDC_RADIO1, m_mode);
	DDX_Text(pDX, IDC_EDIT2, m_valueX);
	DDX_Text(pDX, IDC_EDIT3, m_valueY);
	DDX_Text(pDX, IDC_EDIT4, m_valueZ);
	DDX_Control(pDX, IDC_EDIT2, m_editCtrlX);
	DDX_Control(pDX, IDC_EDIT3, m_editCtrlY);
	DDX_Control(pDX, IDC_EDIT4, m_editCtrlZ);
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
END_MESSAGE_MAP()


// CMapToolPage �޽��� ó�����Դϴ�.


void CMapToolPage::ChangeType()
{
	dynamic_cast<CDynamicCamera*>(Engine::GetCurScene()->GetLayer(L"Environment")->GetGameObject(L"DynamicCamera"))->SetPickType(m_mode);
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
	_vec3* pos = CNaviMesh::GetInstance()->GetPos(cellIndex, vertexIndex);
	if (nullptr == pos) return;

	switch (xyz)
	{
	case 0: pos->x = value; break;
	case 1: pos->y = value; break;
	case 2: pos->z = value; break;
	}
}

BOOL CMapToolPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CMapToolPage::OnClickedTerrainPicking()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	m_mode = 0;

	ChangeType();
}


void CMapToolPage::OnClickedVertexPicking()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	m_mode = 1;

	ChangeType();
}


void CMapToolPage::OnSelectedTreeControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	HTREEITEM itemRoot;
	HTREEITEM itemParent;
	HTREEITEM itemCur;

	CString strRoot;
	CString strParent;
	CString strCur;

	itemCur = pNMTreeView->itemNew.hItem;
	itemParent = m_treeCtrl.GetParentItem(itemCur);

	m_treeCtrl.GetItemImage(itemCur, m_naviIndex, m_vertexIndex);

	const _vec3* pos = CNaviMesh::GetInstance()->GetPos(m_naviIndex, m_vertexIndex);

	if (nullptr != pos)
	{
		m_valueX = pos->x;
		m_valueY = pos->y;
		m_valueZ = pos->z;
	}

	UpdateData(FALSE);

	*pResult = 0;
}


void CMapToolPage::OnDeltaPosX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	_float value = 0.1f * pNMUpDown->iDelta;

	m_valueX -= value;

	ChangeValue(m_naviIndex, m_vertexIndex, m_valueX, 0);

	UpdateData(FALSE);

	*pResult = 0;
}


void CMapToolPage::OnDeltaPosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	_float value = 0.1f * pNMUpDown->iDelta;

	m_valueY -= value;

	ChangeValue(m_naviIndex, m_vertexIndex, m_valueY, 1);

	UpdateData(FALSE);

	*pResult = 0;
}


void CMapToolPage::OnDeltaPosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	_float value = 0.1f * pNMUpDown->iDelta;

	m_valueZ -= value;

	ChangeValue(m_naviIndex, m_vertexIndex, m_valueZ, 2);

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

	ChangeValue(m_naviIndex, m_vertexIndex, m_valueX, 0);

	UpdateData(FALSE);
}


void CMapToolPage::OnChangeEditY()
{
	UpdateData(TRUE);

	CString str;

	m_editCtrlY.GetWindowTextW(str);
	_float value = _tstof(str);

	m_valueY = value;

	ChangeValue(m_naviIndex, m_vertexIndex, m_valueY, 1);

	UpdateData(FALSE);
}


void CMapToolPage::OnChangeEditZ()
{
	UpdateData(TRUE);

	CString str;

	m_editCtrlZ.GetWindowTextW(str);
	_float value = _tstof(str);

	m_valueZ = value;

	ChangeValue(m_naviIndex, m_vertexIndex, m_valueZ, 2);

	UpdateData(FALSE);
}
