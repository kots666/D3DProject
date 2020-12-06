// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "MapTool.h"
#include "FileInfo.h"
#include "afxdialogex.h"


// CMapTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
{
	m_drawID = 0;
}

CMapTool::CMapTool(const CMapTool & rhs)
{
	m_drawID = rhs.m_drawID;
}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
	DDX_Control(pDX, IDC_LIST1, m_listBox);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangePicture)
END_MESSAGE_MAP()


// CMapTool �޽��� ó�����Դϴ�.


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	int fileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	TCHAR filePath[MAX_PATH] = L"";
	CString relativePath = L"";
	for (int i = 0; i < fileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, filePath, MAX_PATH);
		relativePath = CFileInfo::ConvertToRelativePath(filePath);

		CString fileAndExtension = PathFindFileName(filePath);
		TCHAR fileName[MAX_PATH] = L"";
		lstrcpy(fileName, fileAndExtension.GetString());
		PathRemoveExtension(fileName);
		m_listBox.AddString(fileName);
	}

	HorizontalScroll();

	UpdateData(FALSE);
}

void CMapTool::HorizontalScroll()
{
	CString name;
	CSize size;
	int cx = 0;

	CDC* DC = m_listBox.GetDC();

	for (int i = 0; i < m_listBox.GetCount(); ++i)
	{
		m_listBox.GetText(i, name);
		size = DC->GetTextExtent(name);
		
		if (cx < size.cx) cx = size.cx;
	}

	m_listBox.ReleaseDC(DC);

	if (m_listBox.GetHorizontalExtent() < cx) m_listBox.SetHorizontalExtent(cx);
}


void CMapTool::OnLbnSelchangePicture()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CString fileName = L"";

	int selectIndex = m_listBox.GetCurSel();
	m_listBox.GetText(selectIndex, fileName);

	int i = 0;

	for (; i < fileName.GetLength(); ++i)
	{
		if (isdigit(fileName[i])) break;
	}

	fileName.Delete(0, i);
	m_drawID = _ttoi(fileName.GetString());

	CGraphicDevice::GetInstance()->RenderBegin();

	/*
	const TEXINFO* texInfo = CTextureManager::GetInstance()->GetTexInfo(L"Tile", L"Line", m_drawID);
	if (nullptr == texInfo) return;

	float centerX = texInfo->imageInfo.Width >> 1;
	float centerY = texInfo->imageInfo.Height >> 1;

	_matrix matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, float(WINCX) / 32.f, float(WINCY) / 32.f, 1.f);
	//D3DXMatrixScaling(&matScale, float(WINCX) / float(TILECX), float(WINCY) / float(TILECY), 1.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1), 0.f);

	matWorld = matScale * matTrans;

	CGraphicDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::GetInstance()->GetSprite()->Draw(texInfo->texture, nullptr, &_vec3(centerX, centerY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	*/

	CGraphicDevice::GetInstance()->RenderEnd(m_picture.GetSafeHwnd());

	UpdateData(FALSE);
}


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	/*for (int i = 0; i < LINECOUNT; ++i)
	{
		TCHAR buffer[10] = L"";
		wsprintf(buffer, L"%d", i);

		m_meshListBox.AddString(buffer);
	}*/

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
