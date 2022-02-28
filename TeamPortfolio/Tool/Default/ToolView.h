// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once

#define TIMER_UPDATE 1


class CToolDoc;
BEGIN(Tool)
class CObjectTool_ToolObject;
END
class CToolView : public CScrollView
{

protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

	// 특성입니다.
public:
	CToolDoc* GetDocument() const;

	// 작업입니다.
public:

	// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

public:
	void Change_PickVector(bool b);
	void Add_Axis_PickVector();

	_float3 Get_PickPos() { return m_PickPos; }

private:
	_float3 OneVector(_float3 nomalVec);
	
private:
	CObjectTool_ToolObject* m_Nearobj;
	_float3					m_PickPos;
	_float3					m_AddPos;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
{
	return reinterpret_cast<CToolDoc*>(m_pDocument);
}
#endif
