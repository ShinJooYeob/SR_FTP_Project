
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CToolView

HWND	g_hWnd;
IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
	
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
	// #Tag 인스턴스 
//	CTextureMgr::GetInstance()->DestroyInstance();
//	CDevice::GetInstance()->DestroyInstance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//pDC->Rectangle(100, 100, 200, 200);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	/*CDevice::GetInstance()->Get_Sprite()->Draw(m_pSingle->Get_Texture()->pTexture, 
												nullptr,	// 출력할 이미지 영역에 대한 렉트 구조체 주소값, null인 경우 이미지의 0,0을 기준으로 출력하게됨
												nullptr,	// 출력할 이미지의 중심 축 좌표를 vec3 타입의 주소값, null인 경우 0,0이 중심 좌표가 됨
												nullptr,	// 위치 좌표에 대한 vec3타입의 주소값, null인 경우 스크린 상의 0, 0 좌표 출력
												D3DCOLOR_ARGB(255, 255, 255, 255));*/
									          	// 출력할 원본 이미지와 섞을 색상 값, 0xffffffff값을 넘겨주면 원본 색상을 유지하는 옵션	

	

}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기



void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	g_hWnd = m_hWnd;


	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
