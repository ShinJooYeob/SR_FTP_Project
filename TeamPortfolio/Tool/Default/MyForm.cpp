// MyForm.cpp : 구현 파일입니다.
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
	// 가장 하단 선택
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

	// 각 버튼 추가
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

	m_Font.CreatePointFont(80, L"바탕");

	GetDlgItem(IDC_BUTTON9)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON2)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON10)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON11)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON12)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON13)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON14)->SetFont(&m_Font);

	m_MouseSelesctObject = nullptr;

	
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
	// 현재 선택된 오브젝트 
	GetSingle(CSuperToolSIngleton)->SaveData_Object(GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject(), this);
}


void CMyForm::OnBnClickedButtonLoad()
{
	// 오브젝트 로드 해서 새로 생성
	GetSingle(CSuperToolSIngleton)->LoadData_Data(this);
}

void CMyForm::OnMapSave()
{
	if (nullptr == m_MapToolDialog.GetSafeHwnd())
		m_MapToolDialog.Create(IDD_MAPTOOL);	// 해당 id 에 맞는 다이얼로그 생성
	m_MapToolDialog.ShowWindow(SW_SHOW);
}


//void CMyForm::OnCbnSelchangeCombo2()
//{
	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 큐브 컴포넌트로 바꾸기
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 선택한 오브젝트만 렌더링
	int index = m_ListBox_Objects.GetCurSel();

	GetSingle(CSuperToolSIngleton)->Set_ViewObject_Index(index);

	GetSingle(CSuperToolSIngleton)->Update_Select_Render_None(TAG_LAY(Layer_View));
	GetSingle(CSuperToolSIngleton)->Update_Select_Render_Visble(TAG_LAY(Layer_View),
		GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject());
	
	if (GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject() == nullptr)
		return;

	// 텍스처와 위치 업데이트
	if (nullptr != m_TransformDialog.GetSafeHwnd())
		m_TransformDialog.Set_CurrentUpdate_WorldMat(); // 현재 오브젝트 데이터로 업데이트

	else
	{
		m_TransformDialog.Create(IDD_CTrans_Dialog);	
		m_TransformDialog.Set_CurrentUpdate_WorldMat(); 
	}
	GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject()->Texture_CurrentBind();

}


void CMyForm::OnBnClickedButton_Delete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Delete Object 테스트

	// 1. 선택된 오브젝트가 있어야한다.
	
	CObjectTool_ToolObject* obj = GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject();
	if (obj == nullptr)
		return;
	// 2. 오브젝트 삭제 명령후 다음 오브젝트로 선택포커스 변경
	int index = m_ListBox_Objects.GetCurSel();
	obj->DIED();
	m_ListBox_Objects.DeleteString(index);
	m_ListBox_Objects.SetCurSel(index);
}


BOOL CMyForm::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CFormView::OnEraseBkgnd(pDC);
}
