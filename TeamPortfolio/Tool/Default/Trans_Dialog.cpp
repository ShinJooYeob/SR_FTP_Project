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

}

CTrans_Dialog::~CTrans_Dialog()
{


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

	DDX_Control(pDX, IDC_SLIDER1, m_Silder_Rotation[0]);
	DDX_Control(pDX, IDC_SLIDER2, m_Silder_Rotation[1]);
	DDX_Control(pDX, IDC_SLIDER3, m_Silder_Rotation[2]);
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
	CObjectTool_ToolObject* CurrentToolObject = GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject();
	if (CurrentToolObject == nullptr)
		return E_FAIL;

	CurrentToolObject->Set_Position(_float3(newfloat[0], newfloat[1], newfloat[2]));
	_float3 newRot = _float3(D3DXToRadian(newfloat[3]), D3DXToRadian(newfloat[4]), D3DXToRadian(newfloat[5]));

	for (int i=0;i<3;i++)
	{
		m_Silder_Rotation[i].SetPos(newfloat[i + 3]);
	}
	
	CurrentToolObject->Set_Rotation(newRot);
	CurrentToolObject->Set_Scaled(_float3(newfloat[6], newfloat[7], newfloat[8]));

	delete[] newfloat;
	newfloat = nullptr;

	return S_OK;
}





BEGIN_MESSAGE_MAP(CTrans_Dialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CTrans_Dialog::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CTrans_Dialog::OnLbnSelchangeList1)
	ON_WM_HSCROLL()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


HRESULT CTrans_Dialog::ResetTexture()
{
	// 텍스처 리스트 업데이트
	CObjectTool_ToolObject* CurrentToolObject = GetSingle(CSuperToolSIngleton)->Get_ViewObject_Object();

	if (CurrentToolObject)
	{
		CTexture* tex = (CTexture*)CurrentToolObject->Get_Component(TAG_COM(Com_Texture));
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

HRESULT CTrans_Dialog::Set_CurrentUpdate_WorldMat()
{
	// 현재 선택된 값으로 데이터 업데이트
	CObjectTool_ToolObject* CurrentToolObject = GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject();
	if (CurrentToolObject == nullptr)
		return E_FAIL;

	_float3 scale = CurrentToolObject->Get_Scale();
	_float3 pos = CurrentToolObject->Get_Pos();
	

	_tchar buf[32] = L"";

	_itot_s(pos.x, buf, 10);
	m_InputNumber[0].SetWindowText(buf);
	_itot_s(pos.y, buf, 10);
	m_InputNumber[1].SetWindowText(buf);
	_itot_s(pos.z, buf, 10);
	m_InputNumber[2].SetWindowText(buf);

	_itot_s(scale.x, buf, 10);
	m_InputNumber[6].SetWindowText(buf);
	_itot_s(scale.y, buf, 10);
	m_InputNumber[7].SetWindowText(buf);
	_itot_s(scale.z, buf, 10);
	m_InputNumber[8].SetWindowText(buf);

	m_InputNumber[3].SetWindowText(L"0");
	m_InputNumber[4].SetWindowText(L"0");
	m_InputNumber[5].SetWindowText(L"0");

	m_Silder_Rotation[0].SetPos(0);
	m_Silder_Rotation[1].SetPos(0);
	m_Silder_Rotation[2].SetPos(0);


	return S_OK;
}

// CTrans_Dialog 메시지 처리기
BOOL CTrans_Dialog::OnInitDialog()
{
	CDialog::OnInitDialog();


	for (auto& slider : m_Silder_Rotation)
	{
		slider.SetRange(0, 360);
	}

	// 현재 선택된 오브젝트가 있다면 그 값으로 초기화한다.
	CObjectTool_ToolObject* CurrentToolObject = GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject();
	if (CurrentToolObject == nullptr)
	{
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

		// 텍스처 리스트 업데이트
		ResetTexture();
	}
	else
	{
		Set_CurrentUpdate_WorldMat();		
		ResetTexture();
	}
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
	CObjectTool_ToolObject* CurrentToolObject = GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject();
	_uint index = m_TextureListBox.GetCurSel();
	CurrentToolObject->Set_TextureNum_Bind(index);
	
}




void CTrans_Dialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	for (int i=0;i<3;i++)
	{
		if (m_Silder_Rotation[i].GetDlgCtrlID() == pScrollBar->GetDlgCtrlID())
		{
			int iPos = m_Silder_Rotation[i].GetPos();
			CString sPos;
			sPos.Format(_T("%d"), iPos);
			m_InputNumber[i+3].SetWindowText(sPos);
			EditToObjectUpdate(m_InputNumber, 9);
			break;

		}
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CTrans_Dialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//if (nChar == 'C')
	//{
	//	CTransform* trans = (CTransform*)m_GameObject_Rect_Tool->Get_Component(TAG_COM(Com_Transform));
	//	trans->Turn_CCW(_float3(0, 1, 0), 0.3f);

	//}
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
