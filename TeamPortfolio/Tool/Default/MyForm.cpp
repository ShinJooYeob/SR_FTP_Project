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

	list<CGameObject*>* list = nullptr;

	list = GetSingle(CSuperToolSIngleton)->Get_GameObjectList(TAG_LAY(Layer_View));
	if (list == nullptr)
		return E_FAIL;

	for (auto vec : *list)
	{
		m_ListBox_Objects.AddString(static_cast<CObjectTool_ToolObject*>(vec)->GetName());
	}
	// ���� �ϴ� ����
	m_ListBox_Objects.SetCurSel(m_ListBox_Objects.GetCount()-1);

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
	ON_BN_CLICKED(IDC_BUTTON15, &CMyForm::OnBnClickedButton_Delete)
	ON_WM_ERASEBKGND()
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

	m_MouseSelesctObject = nullptr;

	
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
	// ���� ���õ� ������Ʈ 
	GetSingle(CSuperToolSIngleton)->SaveData_Object(GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject(), this);
}


void CMyForm::OnBnClickedButtonLoad()
{
	// ������Ʈ �ε� �ؼ� ���� ����
	GetSingle(CSuperToolSIngleton)->LoadData_Data(this);
}

void CMyForm::OnMapSave()
{
	if (nullptr == m_MapToolDialog.GetSafeHwnd())
		m_MapToolDialog.Create(IDD_MAPTOOL);	// �ش� id �� �´� ���̾�α� ����
	m_MapToolDialog.ShowWindow(SW_SHOW);
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
	//GetSingle(CSuperToolSIngleton)->Get_ViewObject_Object()->Set_ViBuffer_Change();

}

void CMyForm::OnBnClickedButton_CreateObject()
{
	static int num = 0;
	num++;
	TCHAR t[64];
	wsprintf(t,L"new_%d",num);
	wstring filename = t;

	GetSingle(CSuperToolSIngleton)->Create_ToolObject_Button(filename);
}


void CMyForm::OnLbnSelchangeList_ObjectSelect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ������ ������Ʈ�� ������
	int index = m_ListBox_Objects.GetCurSel();

	GetSingle(CSuperToolSIngleton)->Set_ViewObject_Index(index);

	GetSingle(CSuperToolSIngleton)->Update_Select_Render_None(TAG_LAY(Layer_View));
	GetSingle(CSuperToolSIngleton)->Update_Select_Render_Visble(TAG_LAY(Layer_View),
		GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject());
	
	if (GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject() == nullptr)
		return;

	// �ؽ�ó�� ��ġ ������Ʈ
	if (nullptr != m_TransformDialog.GetSafeHwnd())
		m_TransformDialog.Set_CurrentUpdate_WorldMat(); // ���� ������Ʈ �����ͷ� ������Ʈ

	else
	{
		m_TransformDialog.Create(IDD_CTrans_Dialog);	
		m_TransformDialog.Set_CurrentUpdate_WorldMat(); 
	}
	GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject()->Texture_CurrentBind();

}


void CMyForm::OnBnClickedButton_Delete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// Delete Object �׽�Ʈ

	// 1. ���õ� ������Ʈ�� �־���Ѵ�.
	
	CObjectTool_ToolObject* obj = GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject();
	if (obj == nullptr)
		return;
	// 2. ������Ʈ ���� ����� ���� ������Ʈ�� ������Ŀ�� ����
	int index = m_ListBox_Objects.GetCurSel();
	obj->DIED();
	m_ListBox_Objects.DeleteString(index);
	m_ListBox_Objects.SetCurSel(index);
}


BOOL CMyForm::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CFormView::OnEraseBkgnd(pDC);
}
