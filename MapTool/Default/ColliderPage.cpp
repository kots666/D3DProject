// ColliderPage.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFCTool.h"
#include "ColliderPage.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "PlacedObject.h"

// CColliderPage 대화 상자

IMPLEMENT_DYNAMIC(CColliderPage, CPropertyPage)

CColliderPage::CColliderPage()
	: CPropertyPage(IDD_ColliderPage)
	, m_colliderMode(0)
	, m_focusBit(0)
{
	
}

CColliderPage::~CColliderPage()
{
	Release();
}

BOOL CColliderPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_dynamicMeshVec.reserve(10);
	PreLoadMeshes();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CColliderPage::Release()
{
	for (auto& mesh : m_dynamicMeshVec)
	{
		if (nullptr != mesh)
		{
			delete mesh;
			mesh = nullptr;
		}
	}

	for (auto& name : m_frameNameVec)
	{
		if (nullptr != name)
		{
			delete name;
			name = nullptr;
		}
	}

	for (auto& name : m_colliderBoneNameVec)
	{
		if (nullptr != name)
		{
			delete name;
			name = nullptr;
		}
	}
}

void CColliderPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_colliderListBox);
	DDX_Control(pDX, IDC_LIST3, m_dynamicMeshListBox);
	DDX_Control(pDX, IDC_EDIT1, m_positionXEdit);
	DDX_Control(pDX, IDC_EDIT2, m_positionYEdit);
	DDX_Control(pDX, IDC_EDIT3, m_positionZEdit);
	DDX_Control(pDX, IDC_EDIT4, m_radiusEdit);
	DDX_Radio(pDX, IDC_RADIO1, m_colliderMode);
	DDX_Control(pDX, IDC_LIST4, m_frameListBox);
}

BEGIN_MESSAGE_MAP(CColliderPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CColliderPage::OnClickedSelect)
	ON_LBN_SELCHANGE(IDC_LIST3, &CColliderPage::OnSelChangeDynamicList)
	ON_LBN_SELCHANGE(IDC_LIST1, &CColliderPage::OnSelChangeColliderList)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, RadioButtonClick)
	ON_BN_CLICKED(IDC_BUTTON2, &CColliderPage::OnClickedAdd)
	ON_BN_CLICKED(IDC_BUTTON8, &CColliderPage::OnClickedDelete)
	ON_BN_CLICKED(IDC_BUTTON3, &CColliderPage::OnClickedSave)
	ON_WM_MOUSEWHEEL()
	ON_EN_SETFOCUS(IDC_EDIT1, &CColliderPage::OnEnSetfocusEdit1)
	ON_EN_SETFOCUS(IDC_EDIT2, &CColliderPage::OnEnSetfocusEdit2)
	ON_EN_SETFOCUS(IDC_EDIT3, &CColliderPage::OnEnSetfocusEdit3)
	ON_EN_SETFOCUS(IDC_EDIT4, &CColliderPage::OnEnSetfocusEdit4)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CColliderPage::OnEnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CColliderPage::OnEnKillfocusEdit2)
	ON_EN_KILLFOCUS(IDC_EDIT3, &CColliderPage::OnEnKillfocusEdit3)
	ON_EN_KILLFOCUS(IDC_EDIT4, &CColliderPage::OnEnKillfocusEdit4)
	ON_BN_CLICKED(IDC_BUTTON10, &CColliderPage::OnClickedLoad)
END_MESSAGE_MAP()


void CColliderPage::PreLoadMeshes()
{
	CString dirPath = _T("../Resource/Mesh/DynamicMesh/*.X");
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
			LoadDynamicMeshes(fileName, relativePath, EXFileName);
			m_dynamicMeshListBox.AddString(fileName);
		}
	}

	//SetHorizontalScroll(m_colliderListBox);

	UpdateData(FALSE);
}

void CColliderPage::LoadDynamicMeshes(const _tchar * tag, const _tchar * filePath, const _tchar * fileName)
{
	_tchar fullTag[MAX_PATH] = L"Mesh_";
	lstrcat(fullTag, tag);
	CString* str = new CString(fullTag);

	if (FAILED(Engine::ReadyMesh(
		CGraphicDevice::GetInstance()->GetDevice(),
		Engine::RESOURCE_STAGE,
		*str,
		Engine::TYPE_DYNAMIC,
		filePath,
		fileName
	)))
	{
		MessageBox(L"Faild to load Dynamic mesh");
		//MSG_BOX("Fail to load static mesh");
		return;
	}

	m_dynamicMeshVec.emplace_back(str);
}


void CColliderPage::OnClickedSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int dynamicSelect = m_dynamicMeshListBox.GetCurSel();

	if (0 > dynamicSelect || dynamicSelect >= m_dynamicMeshVec.size())
		return;

	_tchar* key = new _tchar[10];

	//lstrcpy(m_dynamicName, m_dynamicMeshVec[dynamicSelect]->GetBuffer());

	wsprintf(key, L"%d", placedIndex);
	CPlacedObject* newObj = CPlacedObject::Create(CGraphicDevice::GetInstance()->GetDevice(), key, m_dynamicMeshVec[dynamicSelect]->GetBuffer(), true);
	if (nullptr == newObj)
	{
		MessageBox(L"Fail to create Dynamic PlacedObject");
		return;
	}

	m_spawnedObj.emplace_back(newObj);

	Engine::GetCurScene()->GetLayer(L"Environment")->AddGameObject(key, newObj);
	ResetFrameList(placedIndex);
	++placedIndex;
}


void CColliderPage::OnSelChangeDynamicList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_colliderListBox.SetCurSel(-1);
}


void CColliderPage::OnSelChangeColliderList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_dynamicMeshListBox.SetCurSel(-1);

	_int selected = m_colliderListBox.GetCurSel();

	if (0 > selected || selected >= m_colliderVec.size()) return;

	m_position = m_colliderVec[selected]->GetOffset();
	m_radius = m_colliderVec[selected]->GetLocalRadius();

	StringSetByValue();
}

void CColliderPage::ResetFrameList(const _int & index)
{
	m_frameListBox.ResetContent();
	m_frameVec.clear();
	m_frameVec.shrink_to_fit();
	
	for (auto name : m_frameNameVec)
	{
		if (nullptr != name)
		{
			delete name;
			name = nullptr;
		}
	}

	m_frameNameVec.clear();
	m_frameNameVec.shrink_to_fit();

	m_frameVec.reserve(100);
	m_frameNameVec.reserve(100);

	_int newIndex = m_spawnedObj.size() - 1;

	D3DXFRAME_EX* root = (D3DXFRAME_EX*)(m_spawnedObj[newIndex]->GetCloneFrame()->pFrameFirstChild);

	SetBoneFrame(root);

	UpdateData(FALSE);
}

void CColliderPage::SetBoneFrame(D3DXFRAME_EX * frame)
{
	if (nullptr == frame) return;

	_tchar* tmpName = new _tchar[50];
	ZeroMemory(tmpName, sizeof(_tchar) * 50);

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, frame->Name, strlen(frame->Name), tmpName, 50);
	CString boneName = tmpName;

	m_frameListBox.AddString(boneName);
	m_frameVec.emplace_back(&frame->combinedTransformationMatrix);
	m_frameNameVec.emplace_back(tmpName);

	if (nullptr != frame->pFrameSibling)
		SetBoneFrame((D3DXFRAME_EX*)frame->pFrameSibling);

	if (nullptr != frame->pFrameFirstChild)
		SetBoneFrame((D3DXFRAME_EX*)frame->pFrameFirstChild);
}

BOOL CColliderPage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			SetPosition();
		}
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CColliderPage::SetPosition()
{
	UpdateData(TRUE);

	CString strValue;
	m_positionXEdit.GetWindowTextW(strValue);
	float xPos = _tstof(strValue);

	m_positionYEdit.GetWindowTextW(strValue);
	float yPos = _tstof(strValue);

	m_positionZEdit.GetWindowTextW(strValue);
	float zPos = _tstof(strValue);

	m_radiusEdit.GetWindowTextW(strValue);
	float radius = _tstof(strValue);

	m_position = _vec3{ xPos, yPos, zPos };
	m_radius = radius;

	StringSetByValue();
	TargetSetByValue();

	UpdateData(FALSE);
}

void CColliderPage::RadioButtonClick(UINT ID)
{
	UpdateData(TRUE);

	if (IDC_RADIO1 == ID)
		m_colliderMode = 0;
	else
		m_colliderMode = 1;

	UpdateData(FALSE);
}


void CColliderPage::OnClickedAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	_int selectedBone = m_frameListBox.GetCurSel();

	if (0 > selectedBone || selectedBone >= m_frameVec.size()) return;

	/*_int selectedMesh = m_dynamicMeshListBox.GetCurSel();

	if (0 > selectedMesh || selectedMesh >= m_spawnedObj.size()) return;*/

	_int num = m_colliderVec.size();
	CString collNum;
	collNum.Format(_T("%d"), num);

	_tchar* name = new _tchar[50];

	swprintf(name, sizeof(_tchar) * 50, L"Com_SphereCollider%d", num);

	CSphereCollider* newSphere = CSphereCollider::Create(CGraphicDevice::GetInstance()->GetDevice(), name, m_frameVec[selectedBone], m_position, m_radius);

	m_spawnedObj[0]->AddSphereCollider(newSphere, name);
	m_colliderVec.emplace_back(newSphere);
	m_colliderListBox.AddString(collNum);
	m_colliderNameVec.emplace_back(name);

	_tchar* boneName = new _tchar[50];
	ZeroMemory(boneName, sizeof(_tchar) * 50);

	lstrcpy(boneName, m_frameNameVec[selectedBone]);

	m_colliderBoneNameVec.emplace_back(boneName);

	UpdateData(FALSE);
}


void CColliderPage::OnClickedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (0 >= m_spawnedObj.size()) return;

	_int select = m_colliderListBox.GetCurSel();

	if (0 > select || select >= m_colliderVec.size()) return;

	/*_int objNum = m_dynamicMeshListBox.GetCurSel();

	if (0 > objNum || objNum >= m_dynamicMeshVec.size()) return;*/

	m_spawnedObj[0]->DeleteSphereCollider(m_colliderVec[select], m_colliderNameVec[select]);

	m_colliderVec.erase(m_colliderVec.begin() + select);
	m_colliderNameVec.erase(m_colliderNameVec.begin() + select);
	m_colliderListBox.DeleteString(select);

	UpdateData(FALSE);
}


void CColliderPage::OnClickedSave()
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

		_int cnt = 0;

		for (auto& coll : m_colliderVec)
		{
			_vec3 offset = coll->GetOffset();
			_float radius = coll->GetLocalRadius();

			WriteFile(hFile, m_colliderBoneNameVec[cnt], sizeof(_tchar) * 50, &dwByte, nullptr);
			WriteFile(hFile, &offset, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, &radius, sizeof(_float), &dwByte, nullptr);

			++cnt;
		}
		CloseHandle(hFile);
	}
}


BOOL CColliderPage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	_float valueOffset = 0.1f;

	if (nFlags & MK_SHIFT)
		valueOffset = 1.f;

	_float value = (_float)(zDelta / 120) * valueOffset;

	if (m_focusBit & 0x0001)
	{
		m_position.x += value;
	}
	else if (m_focusBit & 0x0002)
	{
		m_position.y += value;
	}
	else if (m_focusBit & 0x0004)
	{
		m_position.z += value;
	}
	else if (m_focusBit & 0x0008)
	{
		m_radius += value;
	}

	StringSetByValue();
	TargetSetByValue();

	return CPropertyPage::OnMouseWheel(nFlags, zDelta, pt);
}

void CColliderPage::StringSetByValue()
{
	UpdateData(TRUE);

	CString xPosStr;
	CString yPosStr;
	CString zPosStr;
	CString radiusStr;
	
	xPosStr.Format(L"%f", m_position.x);
	m_positionXEdit.SetWindowTextW(xPosStr);

	yPosStr.Format(L"%f", m_position.y);
	m_positionYEdit.SetWindowTextW(yPosStr);

	zPosStr.Format(L"%f", m_position.z);
	m_positionZEdit.SetWindowTextW(zPosStr);

	radiusStr.Format(L"%f", m_radius);
	m_radiusEdit.SetWindowTextW(radiusStr);

	UpdateData(FALSE);
}

void CColliderPage::TargetSetByValue()
{
	UpdateData(TRUE);

	_int sel = m_colliderListBox.GetCurSel();

	if (0 > sel || sel >= m_colliderVec.size()) return;

	m_colliderVec[sel]->SetOffset(m_position);
	m_colliderVec[sel]->SetLocalRadius(m_radius);

	UpdateData(FALSE);
}


void CColliderPage::OnClickedLoad()
{
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

		_int cnt = 0;

		while (true)
		{
			_tchar* boneName = new _tchar[50];
			ZeroMemory(boneName, sizeof(_tchar) * 50);

			_vec3 offset;
			_float radius;

			ReadFile(hFile, boneName, sizeof(_tchar) * 50, &dwByte, nullptr);
			ReadFile(hFile, &offset, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &radius, sizeof(_float), &dwByte, nullptr);

			if (0 == dwByte)
			{
				delete boneName;
				return;
			}

			char tmpName[50] = "";
			WideCharToMultiByte(CP_ACP, 0, boneName, 50, tmpName, 50, NULL, NULL);

			D3DXFRAME_EX* findBone = const_cast<D3DXFRAME_EX*>(m_spawnedObj[0]->FindFrame(tmpName));

			CString collNum;
			collNum.Format(_T("%d"), cnt);

			_tchar* name = new _tchar[50];

			swprintf(name, sizeof(_tchar) * 50, L"Com_SphereCollider%d", cnt);

			CSphereCollider* newSphere = CSphereCollider::Create(CGraphicDevice::GetInstance()->GetDevice(), name, &(findBone->combinedTransformationMatrix), offset, radius);

			m_spawnedObj[0]->AddSphereCollider(newSphere, name);
			m_colliderVec.emplace_back(newSphere);
			m_colliderListBox.AddString(collNum);
			m_colliderNameVec.emplace_back(name);

			m_colliderBoneNameVec.emplace_back(boneName);

			++cnt;
		}
		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}
