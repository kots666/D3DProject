#pragma once

// CObjectToolPage 대화 상자입니다.

class CPlacedObject;

class CObjectToolPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CObjectToolPage)

public:
	CObjectToolPage();
	virtual ~CObjectToolPage();

public:
	virtual BOOL OnSetActive() override;
	virtual BOOL OnKillActive() override;
	virtual BOOL OnInitDialog();

public:
	CListBox m_meshListBox;
	CListBox m_placedListBox;

	_uint m_meshListBoxIndex = 0;
	_uint m_placedListBoxIndex = 0;

	_vec3* m_targetPos = nullptr;
	_vec3* m_targetScale = nullptr;
	_vec3* m_targetRotation = nullptr;

	_float m_posX = 0.f;
	_float m_posY = 0.f;
	_float m_posZ = 0.f;

	_float m_scaleX = 0.f;
	_float m_scaleY = 0.f;
	_float m_scaleZ = 0.f;

	_float m_rotX = 0.f;
	_float m_rotY = 0.f;
	_float m_rotZ = 0.f;

	_int m_focusbit = 0;

	vector<CString*> m_loadedMeshVec;
	vector<CPlacedObject*> m_placedMeshVec;

	CEdit m_posXEdit;
	CEdit m_posYEdit;
	CEdit m_posZEdit;
	CEdit m_scaleXEdit;
	CEdit m_scaleYEdit;
	CEdit m_scaleZEdit;
	CEdit m_rotXEdit;
	CEdit m_rotYEdit;
	CEdit m_rotZEdit;

public:
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECTTOOLPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSetFocusPosX() { m_focusbit |= 0x0001; }
	afx_msg void OnSetFocusPosY() { m_focusbit |= 0x0002; }
	afx_msg void OnSetFocusPosZ() { m_focusbit |= 0x0004; }
	afx_msg void OnSetFocusScaleX() { m_focusbit |= 0x0010; }
	afx_msg void OnSetFocusScaleY() { m_focusbit |= 0x0020; }
	afx_msg void OnSetFocusScaleZ() { m_focusbit |= 0x0040; }
	afx_msg void OnSetFocusRotationX() { m_focusbit |= 0x0100; }
	afx_msg void OnSetFocusRotationY() { m_focusbit |= 0x0200; }
	afx_msg void OnSetFocusRotationZ() { m_focusbit |= 0x0400; }

	afx_msg void OnKillFocusPosX() { m_focusbit ^= 0x0001; }
	afx_msg void OnKillFocusPosY() { m_focusbit ^= 0x0002; }
	afx_msg void OnKillFocusPosZ() { m_focusbit ^= 0x0004; }
	afx_msg void OnKillFocusScaleX() { m_focusbit ^= 0x0010; }
	afx_msg void OnKillFocusScaleY() { m_focusbit ^= 0x0020; }
	afx_msg void OnKillFocusScaleZ() { m_focusbit ^= 0x0040; }
	afx_msg void OnKillFocusRotationX() { m_focusbit ^= 0x0100; }
	afx_msg void OnKillFocusRotationY() { m_focusbit ^= 0x0200; }
	afx_msg void OnKillFocusRotationZ() { m_focusbit ^= 0x0400; }

public:
	void PreLoadStaticMeshes();
	void LoadStaticMeshes(const _tchar* tag, const _tchar* filePath, const _tchar* fileName);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnClickedPlace();
	void SetCurSelByPlace(const _int& index);
	afx_msg void OnSelChangePlacedList();
	void SetValueByVec();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ChangeValueByEnter(const CEdit& edit, _float& value, _float& vecValue);
	afx_msg void OnClickedSave();
	afx_msg void OnClickedLoad();
	_int FindTagIndexFromVec(const _tchar* tag);
	afx_msg void OnClickedDelete();
	void SetHorizontalScroll(CListBox& listBox);
};
