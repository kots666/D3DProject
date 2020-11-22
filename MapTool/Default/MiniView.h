#pragma once

class CTerrain;

// CMiniView ���Դϴ�.

class CMiniView : public CView
{
	DECLARE_DYNCREATE(CMiniView)

public:
	CTerrain* m_terrain;
	LPDIRECT3DDEVICE9 m_device;

protected:
	CMiniView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMiniView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


