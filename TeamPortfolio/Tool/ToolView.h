// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once

#include "Device.h"

#define TIMER_UPDATE 1


class CToolDoc;
BEGIN(Tool)
class CObjectTool_Rect;
END
class CToolView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

	// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

	// �۾��Դϴ�.
public:

	// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	

public:
	CObjectTool_Rect* GetTargetObject()
	{
		return m_GameObject_Rect_Tool;
	}

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:

	HRESULT Ready_Static_Component_Prototype();
	HRESULT Ready_Static_GameObject_Prototype();
	HRESULT Ready_GameObject_Layer(const _tchar* layertag);

	HRESULT Scene_Change(SCENEID id);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDevice = nullptr;
	CObjectTool_Rect*			m_GameObject_Rect_Tool = nullptr;
	CRenderer*					m_pComRenderer = nullptr;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
{
	return reinterpret_cast<CToolDoc*>(m_pDocument);
}
#endif
