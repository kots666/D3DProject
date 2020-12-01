// MapToolPag.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "MapToolPage.h"
#include "afxdialogex.h"
#include "DynamicCamera.h"
#include "NaviMesh.h"

// CMapToolPage 대화 상자입니다.

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
}


BEGIN_MESSAGE_MAP(CMapToolPage, CPropertyPage)
	ON_BN_CLICKED(IDC_RADIO1, &CMapToolPage::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMapToolPage::OnBnClickedRadio2)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CMapToolPage::OnTvnSelchangedTree1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CMapToolPage::OnDeltaposSpin3)
END_MESSAGE_MAP()


// CMapToolPage 메시지 처리기입니다.


void CMapToolPage::ChangeType()
{
	dynamic_cast<CDynamicCamera*>(Engine::GetCurScene()->GetLayer(L"Environment")->GetGameObject(L"DynamicCamera"))->SetPickType(m_mode);
}

void CMapToolPage::AddItem(int index)
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

BOOL CMapToolPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMapToolPage::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_mode = 0;

	ChangeType();
}


void CMapToolPage::OnBnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_mode = 1;

	ChangeType();
}


void CMapToolPage::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
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


void CMapToolPage::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	_float value = pNMUpDown->iPos + pNMUpDown->iDelta;

	m_valueX += value;

	UpdateData(FALSE);

	*pResult = 0;
}
