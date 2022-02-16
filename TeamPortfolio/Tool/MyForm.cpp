// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"

// CMyForm

#ifdef _DEBUG

#define new DEBUG_NEW

#endif

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{
}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)

	// 각 버튼 추가
	ON_BN_CLICKED(IDC_BUTTON9, &CMyForm::OnPathFind)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnTransform)
	ON_BN_CLICKED(IDC_BUTTON10, &CMyForm::OnObjectSave)
	ON_BN_CLICKED(IDC_BUTTON11, &CMyForm::OnMapSave)
END_MESSAGE_MAP()

// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

// CMyForm 메시지 처리기입니다.

void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_Font.CreatePointFont(130, L"궁서");

	GetDlgItem(IDC_BUTTON9)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON2)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON10)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON11)->SetFont(&m_Font);

}

// 각 버튼 이벤트 함수

void CMyForm::OnPathFind()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (nullptr == m_PathFind.GetSafeHwnd())
		m_PathFind.Create(IDD_PATHFIND);	// 해당 id 에 맞는 다이얼로그 생성

	m_PathFind.ShowWindow(SW_SHOW);  // 창 모양으로 출력
}

void CMyForm::OnTransform()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (nullptr == m_TransformDialog.GetSafeHwnd())
		m_TransformDialog.Create(IDD_CTrans_Dialog);	// 해당 id 에 맞는 다이얼로그 생성

	m_TransformDialog.ShowWindow(SW_SHOW);  // 창 모양으로 출력

}

void CMyForm::OnObjectSave()
{


}

void CMyForm::OnMapSave()
{

}
