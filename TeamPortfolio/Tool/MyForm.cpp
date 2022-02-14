// MyForm.cpp : ���� �����Դϴ�.
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
	: CFormView(IDD_MYFORM), m_PopUp(L"�׳�")
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

	// �� ��ư �߰�
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnUnitTool)
	ON_BN_CLICKED(IDC_BUTTON7, &CMyForm::OnMapTool)
	ON_BN_CLICKED(IDC_BUTTON8, &CMyForm::OnProperty)
	ON_BN_CLICKED(IDC_BUTTON9, &CMyForm::OnPathFind)
END_MESSAGE_MAP()

// CMyForm �����Դϴ�.

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

// CMyForm �޽��� ó�����Դϴ�.

void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	m_Font.CreatePointFont(180, L"�ü�");

	GetDlgItem(IDC_BUTTON1)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON7)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON8)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON9)->SetFont(&m_Font);

	if (nullptr == m_PopUp.GetSafeHwnd())
		m_PopUp.Create(0, WS_OVERLAPPEDWINDOW);
}

// �� ��ư �̺�Ʈ �Լ�
void CMyForm::OnUnitTool()
{
	// GetSafeHwnd : ���� ���̾�α��� ������ �ڵ��� ��ȯ

	if (nullptr == m_UnitTool.GetSafeHwnd())
		m_UnitTool.Create(IDD_UNITTOOL);	// �ش� id �� �´� ���̾�α� ����

	m_UnitTool.ShowWindow(SW_SHOW);  // â ������� ���
}

void CMyForm::OnMapTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (nullptr == m_MapTool.GetSafeHwnd())
		m_MapTool.Create(IDD_MAPTOOL);	// �ش� id �� �´� ���̾�α� ����

	m_MapTool.ShowWindow(SW_SHOW);  // â ������� ���
}

void CMyForm::OnProperty()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_PopUp.ShowWindow(SW_SHOW);  // â ������� ���
}

void CMyForm::OnPathFind()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (nullptr == m_PathFind.GetSafeHwnd())
		m_PathFind.Create(IDD_PATHFIND);	// �ش� id �� �´� ���̾�α� ����

	m_PathFind.ShowWindow(SW_SHOW);  // â ������� ���
}