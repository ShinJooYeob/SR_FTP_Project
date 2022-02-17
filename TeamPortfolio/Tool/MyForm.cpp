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
	: CFormView(IDD_MYFORM)
{
}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_ComboBox);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)

	// �� ��ư �߰�
	ON_BN_CLICKED(IDC_BUTTON9, &CMyForm::OnPathFind)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnTransform)
	ON_BN_CLICKED(IDC_BUTTON10, &CMyForm::OnObjectSave)
	ON_BN_CLICKED(IDC_BUTTON11, &CMyForm::OnMapSave)
	ON_BN_CLICKED(IDC_BUTTON12, &CMyForm::OnBnClickedButtonLoad)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CMyForm::OnCbnSelchangeCombo2)
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

	m_Font.CreatePointFont(130, L"�ü�");

	GetDlgItem(IDC_BUTTON9)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON2)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON10)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON11)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON12)->SetFont(&m_Font);

}

// �� ��ư �̺�Ʈ �Լ�

void CMyForm::OnPathFind()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (nullptr == m_PathFind.GetSafeHwnd())
		m_PathFind.Create(IDD_PATHFIND);	// �ش� id �� �´� ���̾�α� ����

	m_PathFind.ShowWindow(SW_SHOW);  // â ������� ���
}

void CMyForm::OnTransform()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (nullptr == m_TransformDialog.GetSafeHwnd())
		m_TransformDialog.Create(IDD_CTrans_Dialog);	// �ش� id �� �´� ���̾�α� ����

	m_TransformDialog.ShowWindow(SW_SHOW);  // â ������� ���

}

void CMyForm::OnObjectSave()
{
	GetSingle(CSuperToolSIngleton)->SaveData_Object(GetSingle(CSuperToolSIngleton)->GetObjectRect(), this);
}

void CMyForm::OnMapSave()
{

}


void CMyForm::OnBnClickedButtonLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetSingle(CSuperToolSIngleton)->LoadData_Object(this);
}

void CMyForm::OnCbnSelchangeCombo2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int a = m_ComboBox.GetCurSel();
	int b = 0;
	switch (a)
	{
	case 0:
		b = 0;
		break;

	case 1:
		b = 1;
		break;

	case 2:
		b = 2;
		break;

	}
}



