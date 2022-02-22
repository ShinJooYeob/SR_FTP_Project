// MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"

#include "ObjectTool_ToolObject.h"


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

HRESULT CMyForm::Update_ViewListBox()
{
	UpdateData(FALSE);

	m_ListBox_Objects.ResetContent();

	if (GetSingle(CSuperToolSIngleton)->Get_ToolVec_isEmpty())
		return E_FAIL;

	auto Vectors = GetSingle(CSuperToolSIngleton)->Get_ToolVec();
	
	for (auto vec : Vectors)
	{
		m_ListBox_Objects.AddString(vec->Get_ObjectInfo().strObjectName);
	}
	UpdateData(TRUE);

	return S_OK;
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_CheckCameraEnable);
	DDX_Control(pDX, IDC_CHECK2, m_CheckWirframeEnable);
	DDX_Control(pDX, IDC_LIST3, m_ListBox_Objects);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)

	// �� ��ư �߰�
	ON_BN_CLICKED(IDC_BUTTON9, &CMyForm::OnPathFind)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnTransform)
	ON_BN_CLICKED(IDC_BUTTON10, &CMyForm::OnObjectSave)
	ON_BN_CLICKED(IDC_BUTTON11, &CMyForm::OnMapSave)
	ON_BN_CLICKED(IDC_BUTTON12, &CMyForm::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON13, &CMyForm::OnBnClickedButtonCube)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON14, &CMyForm::OnBnClickedButton_CreateObject)
	ON_LBN_SELCHANGE(IDC_LIST3, &CMyForm::OnLbnSelchangeList_ObjectSelect)
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

	m_Font.CreatePointFont(80, L"����");

	GetDlgItem(IDC_BUTTON9)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON2)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON10)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON11)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON12)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON13)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON14)->SetFont(&m_Font);

	Update_ViewListBox();

	
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

//void CMyForm::OnCbnSelchangeCombo2()
//{
	//// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//int a = m_ComboBox.GetCurSel();
	//int b = 0;
	//switch (a)
	//{
	//case 0:
	//	b = 0;
	//	break;

	//case 1:
	//	b = 1;
	//	break;

	//case 2:
	//	b = 2;
	//	break;

	//}
//}

void CMyForm::OnBnClickedButtonCube()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ť�� ������Ʈ�� �ٲٱ�
	GetSingle(CSuperToolSIngleton)->GetObjectRect()->Set_ViBuffer_Change();
}


void CMyForm::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	//_tchar PickMouseText[64] = L"";
	//wsprintf(PickMouseText, L"MousePos (%d,%d)", ptMouse.x, ptMouse.y);

	//SetDlgItemText(IDC_STATIC1, MFCMouseText);
	//SetDlgItemText(IDC_STATIC2, PickMouseText);

	CFormView::OnLButtonDown(nFlags, point);
}


void CMyForm::OnBnClickedButton_CreateObject()
{
	static int num = 0;
	num++;
	TCHAR t[64];
	wsprintf(t,L"new_%d",num);
	GetSingle(CSuperToolSIngleton)->Create_ToolObject_Button(t);

	
}


void CMyForm::OnLbnSelchangeList_ObjectSelect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ������ ������Ʈ�� ������
	int index = m_ListBox_Objects.GetCurSel();

	//GetSingle(CSuperToolSIngleton)->Update_Select_Render_None();
	//GetSingle(CSuperToolSIngleton)->Update_Select_Render_Visble(m_);


	//Update_Select_Render_Visble


}
