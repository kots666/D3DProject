// ObjectToolPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "ObjectToolPage.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "PlacedObject.h"

// CObjectToolPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjectToolPage, CPropertyPage)

CObjectToolPage::CObjectToolPage()
	: CPropertyPage(IDD_OBJECTTOOLPAGE)
	, m_posX(0)
	, m_posY(0)
	, m_posZ(0)
	, m_scaleX(0)
	, m_scaleY(0)
	, m_scaleZ(0)
	, m_rotX(0)
	, m_rotY(0)
	, m_rotZ(0)
{

}

CObjectToolPage::~CObjectToolPage()
{
	for (auto& string : m_loadedMeshVec)
	{
		if (nullptr != string)
		{
			delete string;
			string = nullptr;
		}
	}
}

BOOL CObjectToolPage::OnSetActive()
{
	return CPropertyPage::OnSetActive();
}

BOOL CObjectToolPage::OnKillActive()
{
	return CPropertyPage::OnKillActive();
}

BOOL CObjectToolPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_loadedMeshVec.reserve(100);
	m_placedMeshVec.reserve(100);

	PreLoadStaticMeshes();

	//LoadStaticMeshes();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CObjectToolPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_meshListBox);
	DDX_Control(pDX, IDC_LIST2, m_placedListBox);
	DDX_Text(pDX, IDC_EDIT1, m_posX);
	DDX_Text(pDX, IDC_EDIT4, m_posY);
	DDX_Text(pDX, IDC_EDIT5, m_posZ);
	DDX_Text(pDX, IDC_EDIT7, m_scaleX);
	DDX_Text(pDX, IDC_EDIT8, m_scaleY);
	DDX_Text(pDX, IDC_EDIT9, m_scaleZ);
	DDX_Text(pDX, IDC_EDIT10, m_rotX);
	DDX_Text(pDX, IDC_EDIT11, m_rotY);
	DDX_Text(pDX, IDC_EDIT12, m_rotZ);
	DDX_Control(pDX, IDC_EDIT1, m_posXEdit);
	DDX_Control(pDX, IDC_EDIT4, m_posYEdit);
	DDX_Control(pDX, IDC_EDIT5, m_posZEdit);
	DDX_Control(pDX, IDC_EDIT7, m_scaleXEdit);
	DDX_Control(pDX, IDC_EDIT8, m_scaleYEdit);
	DDX_Control(pDX, IDC_EDIT9, m_scaleZEdit);
	DDX_Control(pDX, IDC_EDIT10, m_rotXEdit);
	DDX_Control(pDX, IDC_EDIT11, m_rotYEdit);
	DDX_Control(pDX, IDC_EDIT12, m_rotZEdit);
}

void CObjectToolPage::PreLoadStaticMeshes()
{
	CString dirPath = _T("../Resource/Mesh/StaticMesh/Blade2/*.X");
	CString relativePath = CFileInfo::DeleteFileName(dirPath);
	//검색 클래스
	CFileFind finder;

	//CFileFind는 파일, 디렉터리가 존재하면 TRUE 를 반환함
	BOOL isWorking = finder.FindFile(dirPath); //

	CString fileName;

	while (isWorking)
	{
		//다음 파일 / 폴더 가 존재하면다면 TRUE 반환
		isWorking = finder.FindNextFile();
		//파일 일때
		if (finder.IsArchived())
		{
			//파일의 이름
			CString EXFileName = finder.GetFileName();
			fileName = finder.GetFileTitle();
			LoadStaticMeshes(fileName, relativePath, EXFileName);
			m_meshListBox.AddString(fileName);
		}
	}

	UpdateData(FALSE);
}

void CObjectToolPage::LoadStaticMeshes(const _tchar* tag, const _tchar* filePath, const _tchar* fileName)
{
	_tchar fullTag[MAX_PATH] = L"Mesh_";
	lstrcat(fullTag, tag);
	CString* str = new CString(fullTag);

	if (FAILED(Engine::ReadyMesh(
		CGraphicDevice::GetInstance()->GetDevice(),
		Engine::RESOURCE_STAGE,
		*str,
		Engine::TYPE_STATIC,
		filePath,
		fileName
	)))
	{
		MessageBox(L"Faild to load static mesh");
		//MSG_BOX("Fail to load static mesh");
		return;
	}

	m_loadedMeshVec.emplace_back(str);
}


BEGIN_MESSAGE_MAP(CObjectToolPage, CPropertyPage)
	ON_WM_DROPFILES()
	ON_WM_MOUSEWHEEL()
	ON_EN_SETFOCUS(IDC_EDIT1, &CObjectToolPage::OnSetFocusPosX)
	ON_EN_SETFOCUS(IDC_EDIT4, &CObjectToolPage::OnSetFocusPosY)
	ON_EN_SETFOCUS(IDC_EDIT5, &CObjectToolPage::OnSetFocusPosZ)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CObjectToolPage::OnKillFocusPosX)
	ON_EN_KILLFOCUS(IDC_EDIT4, &CObjectToolPage::OnKillFocusPosY)
	ON_EN_KILLFOCUS(IDC_EDIT5, &CObjectToolPage::OnKillFocusPosZ)
	ON_BN_CLICKED(IDC_BUTTON1, &CObjectToolPage::OnClickedPlace)
	ON_EN_SETFOCUS(IDC_EDIT7, &CObjectToolPage::OnSetFocusScaleX)
	ON_EN_SETFOCUS(IDC_EDIT8, &CObjectToolPage::OnSetFocusScaleY)
	ON_EN_SETFOCUS(IDC_EDIT9, &CObjectToolPage::OnSetFocusScaleZ)
	ON_EN_SETFOCUS(IDC_EDIT10, &CObjectToolPage::OnSetFocusRotationX)
	ON_EN_SETFOCUS(IDC_EDIT11, &CObjectToolPage::OnSetFocusRotationY)
	ON_EN_SETFOCUS(IDC_EDIT12, &CObjectToolPage::OnSetFocusRotationZ)
	ON_EN_KILLFOCUS(IDC_EDIT7, &CObjectToolPage::OnKillFocusScaleX)
	ON_EN_KILLFOCUS(IDC_EDIT8, &CObjectToolPage::OnKillFocusScaleY)
	ON_EN_KILLFOCUS(IDC_EDIT9, &CObjectToolPage::OnKillFocusScaleZ)
	ON_EN_KILLFOCUS(IDC_EDIT10, &CObjectToolPage::OnKillFocusRotationX)
	ON_EN_KILLFOCUS(IDC_EDIT11, &CObjectToolPage::OnKillFocusRotationY)
	ON_EN_KILLFOCUS(IDC_EDIT12, &CObjectToolPage::OnKillFocusRotationZ)
	ON_LBN_SELCHANGE(IDC_LIST2, &CObjectToolPage::OnSelChangePlacedList)
	ON_BN_CLICKED(IDC_BUTTON4, &CObjectToolPage::OnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON5, &CObjectToolPage::OnClickedLoad)
END_MESSAGE_MAP()


// CObjectToolPage 메시지 처리기입니다.

void CObjectToolPage::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CPropertyPage::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	int fileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	TCHAR filePath[MAX_PATH] = L"";
	CString relativePath = L"";
	for (int i = 0; i < fileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, filePath, MAX_PATH);
		relativePath = CFileInfo::ConverToRelativePathOnly(filePath);

		CString fileAndExtension = PathFindFileName(filePath);
		TCHAR fileName[MAX_PATH] = L"";
		lstrcpy(fileName, fileAndExtension.GetString());
		PathRemoveExtension(fileName);
		m_meshListBox.AddString(fileName);

		LoadStaticMeshes(fileName, relativePath, fileAndExtension);
	}

	UpdateData(FALSE);
}


BOOL CObjectToolPage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	_float offset = 0.1f;

	if (nFlags & MK_SHIFT)
		offset = 1.f;

	_float value = (_float)(zDelta / 120) * offset;

	if (m_focusbit & 0x0001)
	{
		m_posX += value;

		if (nullptr != m_targetPos)
			m_targetPos->x += value;

		UpdateData(FALSE);
	}
	else if (m_focusbit & 0x0002)
	{
		m_posY += value;

		if (nullptr != m_targetPos)
			m_targetPos->y = m_posY;

		UpdateData(FALSE);
	}
	else if (m_focusbit & 0x0004)
	{
		m_posZ += value;

		if (nullptr != m_targetPos)
			m_targetPos->z = m_posZ;

		UpdateData(FALSE);
	}
	else if (m_focusbit & 0x0010)
	{
		m_scaleX += value;

		if (nullptr != m_targetScale)
			m_targetScale->x = m_scaleX;

		UpdateData(FALSE);
	}
	else if (m_focusbit & 0x0020)
	{
		m_scaleY += value;

		if (nullptr != m_targetScale)
			m_targetScale->y = m_scaleY;

		UpdateData(FALSE);
	}
	else if (m_focusbit & 0x0040)
	{
		m_scaleZ += value;

		if (nullptr != m_targetScale)
			m_targetScale->z = m_scaleZ;

		UpdateData(FALSE);
	}
	else if (m_focusbit & 0x0100)
	{
		m_rotX += value;

		if (nullptr != m_targetRotation)
			m_targetRotation->x = D3DXToRadian(m_rotX);

		UpdateData(FALSE);
	}
	else if (m_focusbit & 0x0200)
	{
		m_rotY += value;

		if (nullptr != m_targetRotation)
			m_targetRotation->y = D3DXToRadian(m_rotY);

		UpdateData(FALSE);
	}
	else if (m_focusbit & 0x0400)
	{
		m_rotZ += value;

		if (nullptr != m_targetRotation)
			m_targetRotation->z = D3DXToRadian(m_rotZ);

		UpdateData(FALSE);
	}

	return CPropertyPage::OnMouseWheel(nFlags, zDelta, pt);
}

void CObjectToolPage::OnClickedPlace()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	_int select = m_meshListBox.GetCurSel();

	if (0 > select || select >= m_loadedMeshVec.size()) return;

	_int size = m_placedMeshVec.size();

	_tchar* key = new _tchar[10];

	wsprintf(key, L"%d", size);

	CPlacedObject* newObj = CPlacedObject::Create(CGraphicDevice::GetInstance()->GetDevice(), key, m_loadedMeshVec[select]->GetBuffer());
	if (nullptr == newObj)
	{
		MessageBox(L"Fail to create PlacedObject");
		return;
	}

	m_placedMeshVec.emplace_back(newObj);

	Engine::GetCurScene()->GetLayer(L"Environment")->AddGameObject(key, newObj);
	m_placedListBox.AddString(*m_loadedMeshVec[select]);

	SetCurSelByPlace(m_placedMeshVec.size() - 1);

	UpdateData(FALSE);
}

void CObjectToolPage::SetCurSelByPlace(const _int & index)
{
	m_placedListBox.SetCurSel(index);

	m_targetPos = m_placedMeshVec[index]->GetPosAddr();
	m_targetScale = m_placedMeshVec[index]->GetScaleAddr();
	m_targetRotation = m_placedMeshVec[index]->GetRotationAddr();

	SetValueByVec();
}

void CObjectToolPage::OnSelChangePlacedList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_int select = m_placedListBox.GetCurSel();

	if (0 > select || select >= m_placedMeshVec.size()) return;

	m_targetPos = m_placedMeshVec[select]->GetPosAddr();
	m_targetScale = m_placedMeshVec[select]->GetScaleAddr();
	m_targetRotation = m_placedMeshVec[select]->GetRotationAddr();

	SetValueByVec();

	UpdateData(FALSE);
}

void CObjectToolPage::SetValueByVec()
{
	m_posX = m_targetPos->x;
	m_posY = m_targetPos->y;
	m_posZ = m_targetPos->z;

	m_scaleX = m_targetScale->x;
	m_scaleY = m_targetScale->y;
	m_scaleZ = m_targetScale->z;

	m_rotX = D3DXToDegree(m_targetRotation->x);
	m_rotY = D3DXToDegree(m_targetRotation->y);
	m_rotZ = D3DXToDegree(m_targetRotation->z);
}


BOOL CObjectToolPage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (nullptr == m_targetPos) return CPropertyPage::PreTranslateMessage(pMsg);
		if (nullptr == m_targetScale) return CPropertyPage::PreTranslateMessage(pMsg);
		if (nullptr == m_targetRotation) return CPropertyPage::PreTranslateMessage(pMsg);

		if (pMsg->hwnd == m_posXEdit.m_hWnd)
			ChangeValueByEnter(m_posXEdit, m_posX, m_targetPos->x);
		else if (pMsg->hwnd == m_posYEdit.m_hWnd)
			ChangeValueByEnter(m_posYEdit, m_posY, m_targetPos->y);
		else if (pMsg->hwnd == m_posZEdit.m_hWnd)
			ChangeValueByEnter(m_posZEdit, m_posZ, m_targetPos->z);
		else if (pMsg->hwnd == m_scaleXEdit.m_hWnd)
			ChangeValueByEnter(m_scaleXEdit, m_scaleX, m_targetScale->x);
		else if (pMsg->hwnd == m_scaleYEdit.m_hWnd)
			ChangeValueByEnter(m_scaleYEdit, m_scaleY, m_targetScale->y);
		else if (pMsg->hwnd == m_scaleZEdit.m_hWnd)
			ChangeValueByEnter(m_scaleZEdit, m_scaleZ, m_targetScale->z);
		else if (pMsg->hwnd == m_rotXEdit.m_hWnd)
		{
			ChangeValueByEnter(m_rotXEdit, m_rotX, m_targetRotation->x);

			m_targetRotation->x = D3DXToRadian(m_rotX);
			UpdateData(FALSE);
		}
		else if (pMsg->hwnd == m_rotYEdit.m_hWnd)
		{
			ChangeValueByEnter(m_rotYEdit, m_rotY, m_targetRotation->y);

			m_targetRotation->y = D3DXToRadian(m_rotY);
			UpdateData(FALSE);
		}
		else if (pMsg->hwnd == m_rotZEdit.m_hWnd)
		{
			ChangeValueByEnter(m_rotZEdit, m_rotZ, m_targetRotation->z);

			m_targetRotation->z = D3DXToRadian(m_rotZ);
			UpdateData(FALSE);
		}
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CObjectToolPage::ChangeValueByEnter(const CEdit& edit, _float & value, _float & vecValue)
{
	CString strValue;
	edit.GetWindowTextW(strValue);

	_float changed = _tstof(strValue);

	value = changed;
	vecValue = changed;

	UpdateData(FALSE);
}


void CObjectToolPage::OnClickedSave()
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

		for (auto& obj : m_placedMeshVec)
		{
			_tchar fileTag[30] = L"";
			const _tchar* tmpTag = obj->GetTag();

			lstrcpy(fileTag, tmpTag);

			WriteFile(hFile, fileTag, sizeof(fileTag), &dwByte, nullptr);
			WriteFile(hFile, obj->GetPosAddr(), sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, obj->GetScaleAddr(), sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, obj->GetRotationAddr(), sizeof(_vec3), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}
}


void CObjectToolPage::OnClickedLoad()
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

		while (true)
		{
			_tchar fileTag[30] = L"";

			_vec3 pos;
			_vec3 scale;
			_vec3 rotation;

			ReadFile(hFile, &fileTag, sizeof(fileTag), &dwByte, nullptr);
			ReadFile(hFile, &pos, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &scale, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &rotation, sizeof(_vec3), &dwByte, nullptr);

			if (0 == dwByte) break;

			_int index = FindTagIndexFromVec(fileTag);

			_int size = m_placedMeshVec.size();

			_tchar* key = new _tchar[10];

			wsprintf(key, L"%d", size);

			CPlacedObject* newObj = CPlacedObject::Create(CGraphicDevice::GetInstance()->GetDevice(), key, m_loadedMeshVec[index]->GetBuffer());
			newObj->SetPos(pos);
			newObj->SetScale(scale);
			newObj->SetRotation(rotation);

			Engine::GetCurScene()->GetLayer(L"Environment")->AddGameObject(key, newObj);
			m_placedMeshVec.emplace_back(newObj);
			m_placedListBox.AddString(m_loadedMeshVec[index]->GetBuffer());
		}
		CloseHandle(hFile);
	}
	UpdateData(FALSE);
}

_int CObjectToolPage::FindTagIndexFromVec(const _tchar * tag)
{
	_int ret = -1;

	_int index = 0;
	for (auto& str : m_loadedMeshVec)
	{
		if(0 == lstrcmp(str->GetBuffer(), tag))
		{
			ret = index;
		}
		++index;
	}
	return ret;
}