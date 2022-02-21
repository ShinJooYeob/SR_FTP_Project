// Trans_Dialog.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Trans_Dialog.h"
#include "ObjectTool_ToolObject.h"
#include "afxdialogex.h"


// CTrans_Dialog 대화 상자

IMPLEMENT_DYNAMIC(CTrans_Dialog, CDialog)

CTrans_Dialog::CTrans_Dialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CTrans_Dialog, pParent)
{

	m_GameObject_Rect_Tool = nullptr;
}

CTrans_Dialog::~CTrans_Dialog()
{

	Safe_Release(m_GameObject_Rect_Tool);

}

void CTrans_Dialog::DoDataExchange(CDataExchange* pDX)
{
	// 창 체인지 될떄만 업데이트 된다.
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT1, m_InputNumber[0]);
	DDX_Control(pDX, IDC_EDIT2, m_InputNumber[1]);
	DDX_Control(pDX, IDC_EDIT3, m_InputNumber[2]);
	DDX_Control(pDX, IDC_EDIT4, m_InputNumber[3]);
	DDX_Control(pDX, IDC_EDIT5, m_InputNumber[4]);
	DDX_Control(pDX, IDC_EDIT6, m_InputNumber[5]);
	DDX_Control(pDX, IDC_EDIT7, m_InputNumber[6]);
	DDX_Control(pDX, IDC_EDIT8, m_InputNumber[7]);
	DDX_Control(pDX, IDC_EDIT9, m_InputNumber[8]);


	DDX_Control(pDX, IDC_LIST1, m_TextureListBox);
}

HRESULT CTrans_Dialog::EditToObjectUpdate(CEdit* edit,_uint editCount)
{
	// Edit값 체크
	CString strfloat;
	_float* newfloat = new _float[editCount];

	for (int i=0; i<editCount; i++)
	{
		GetDlgItemText(edit[i].GetDlgCtrlID(), strfloat);
		newfloat[i] = _wtof(strfloat);
	}

	// Edit 값을 현재 오브젝트와 동기화.

	// 실제 게임 오브젝트의 크기를 변경.
	if (m_GameObject_Rect_Tool == nullptr)
		return E_FAIL;

	m_GameObject_Rect_Tool->Set_Position(_float3(newfloat[0], newfloat[1], newfloat[2]));
	m_GameObject_Rect_Tool->Set_Scaled(_float3(newfloat[6], newfloat[7], newfloat[8]));


	return S_OK;
}





BEGIN_MESSAGE_MAP(CTrans_Dialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CTrans_Dialog::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CTrans_Dialog::OnLbnSelchangeList1)
END_MESSAGE_MAP()


HRESULT CTrans_Dialog::ResetTexture()
{
	// 텍스처 릭스트 업데이트
	if (m_GameObject_Rect_Tool)
	{
		CTexture* tex = (CTexture*)m_GameObject_Rect_Tool->Get_Component(TAG_COM(Com_Texture));
		m_TextureListBox.ResetContent();
		int size = tex->CurrentTextureLayerSize()+1;

		for (_uint i = 0; i < size; i++)
		{
			TCHAR fomatText[64];
			wsprintf(fomatText, L"Block_%d", i);
			m_TextureListBox.AddString(fomatText);
		}


	}
	return S_OK;
}

// CTrans_Dialog 메시지 처리기
BOOL CTrans_Dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// 공통 오브젝트 사용
	if (m_GameObject_Rect_Tool == nullptr)
	{
		m_GameObject_Rect_Tool = GetSingle(CSuperToolSIngleton)->GetObjectRect();
		m_GameObject_Rect_Tool->AddRef();
	}

	// 값 초기화
	_tchar buf[10] = L"";
	for (auto&input : m_InputNumber)
	{
		_itot_s(0, buf, 10);
		input.SetWindowText(buf);
	}
	_itot_s(1, buf, 10);
	m_InputNumber[6].SetWindowText(buf);
	m_InputNumber[7].SetWindowText(buf);
	m_InputNumber[8].SetWindowText(buf);


	//	#TODO:  내부 블록 텍스처로 리스트 박스 업데이트.
	ResetTexture();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CTrans_Dialog::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.



	return CDialog::DestroyWindow();
}


void CTrans_Dialog::OnBnClickedButton1()
{
	// 현재 정보가 오브젝트에 적용된다.
	EditToObjectUpdate(m_InputNumber,9);

}


void CTrans_Dialog::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTexture* tex = (CTexture*)m_GameObject_Rect_Tool->Get_Component(TAG_COM(Com_Texture));
	_uint index = m_TextureListBox.GetCurSel();

	tex->Bind_Texture(index);

}
