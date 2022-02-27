// MiniView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MiniView.h"
#include "MainFrm.h"
#include "ToolView.h"

// CMiniView
#ifdef _DEBUG

#define new DEBUG_NEW

#endif

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{
}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CMiniView 그리기입니다.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.

	GetSingle(CSuperToolSIngleton)->Render_Begin();

	GetSingle(CSuperToolSIngleton)->Render_End(m_hWnd);

}

// CMiniView 진단입니다.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

// CMiniView 메시지 처리기입니다.

BOOL CMiniView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return false;
}
