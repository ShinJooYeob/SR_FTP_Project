// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once

#include "Device.h"
#include "TextureMgr.h"
#include "Terrain.h"

class CToolDoc;
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

	CTerrain*			Get_Terrain(void) { return m_pTerrain; }

public:
	LPDIRECT3DDEVICE9	m_pGraphicDevice = nullptr;

	CTerrain*			m_pTerrain = nullptr;
	CRenderer*			m_pComRenderer = nullptr;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:

	HRESULT Ready_Static_Component_Prototype();
	HRESULT Ready_Static_GameObject_Prototype();
	HRESULT Ready_GameObject_Layer(const _tchar* layertag);

	HRESULT Scene_Change(SCENEID id);
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
{
	return reinterpret_cast<CToolDoc*>(m_pDocument);
}
#endif
