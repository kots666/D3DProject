#pragma once

// CColliderPage 대화 상자

class CPlacedObject;

class CColliderPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CColliderPage)

public:
	CColliderPage();
	virtual ~CColliderPage();

public:
	virtual BOOL OnInitDialog();
	void Release();

public:
	CListBox m_frameListBox;
	CListBox m_dynamicMeshListBox;
	CListBox m_colliderListBox;

	vector<_matrix*> m_frameVec;
	vector<_tchar*> m_frameNameVec;

	vector<CString*> m_dynamicMeshVec;
	vector<CPlacedObject*> m_spawnedObj;
	vector<_tchar*> m_spawnedName;

	vector<CSphereCollider*> m_colliderVec;
	vector<_tchar*> m_colliderNameVec;
	vector<_tchar*> m_colliderBoneNameVec;

	CEdit m_positionXEdit;
	CEdit m_positionYEdit;
	CEdit m_positionZEdit;

	CEdit m_radiusEdit;

	_vec3 m_position;
	_float m_radius;

	_int m_colliderMode;
	_int m_focusBit;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ColliderPage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnSetfocusEdit1() { m_focusBit |= 0x0001; }
	afx_msg void OnEnSetfocusEdit2() { m_focusBit |= 0x0002; }
	afx_msg void OnEnSetfocusEdit3() { m_focusBit |= 0x0004; }
	afx_msg void OnEnSetfocusEdit4() { m_focusBit |= 0x0008; }
	afx_msg void OnEnKillfocusEdit1() { m_focusBit ^= 0x0001; }
	afx_msg void OnEnKillfocusEdit2() { m_focusBit ^= 0x0002; }
	afx_msg void OnEnKillfocusEdit3() { m_focusBit ^= 0x0004; }
	afx_msg void OnEnKillfocusEdit4() { m_focusBit ^= 0x0008; }

public:
	void PreLoadMeshes();
	void LoadDynamicMeshes(const _tchar* tag, const _tchar* filePath, const _tchar* fileName);
	afx_msg void OnClickedSelect();
	afx_msg void OnSelChangeDynamicList();
	afx_msg void OnSelChangeColliderList();
	void ResetFrameList(const _int& index);
	void SetBoneFrame(D3DXFRAME_EX* frame);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetPosition();
	afx_msg void RadioButtonClick(UINT ID);
	afx_msg void OnClickedAdd();
	afx_msg void OnClickedDelete();
	afx_msg void OnClickedSave();
	afx_msg void OnClickedLoad();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void StringSetByValue();
	void TargetSetByValue();
	
};
