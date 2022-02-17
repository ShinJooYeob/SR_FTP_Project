// Trans_Dialog.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Trans_Dialog.h"
#include "ObjectTool_Rect.h"
#include "afxdialogex.h"


// CTrans_Dialog 대화 상자

IMPLEMENT_DYNAMIC(CTrans_Dialog, CDialog)

CTrans_Dialog::CTrans_Dialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CTrans_Dialog, pParent)
{
	m_bStart = false;
	m_MaxSilderSize = 100;
	m_GameObject_Rect_Tool = nullptr;
}

CTrans_Dialog::~CTrans_Dialog()
{

	Safe_Release(m_GameObject_Rect_Tool);
}

void CTrans_Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_SilderNumber[0]);
	DDX_Control(pDX, IDC_SLIDER2, m_SilderNumber[1]);
	DDX_Control(pDX, IDC_SLIDER3, m_SilderNumber[2]);

	DDX_Control(pDX, IDC_EDIT1, m_InputNumber[0]);
	DDX_Control(pDX, IDC_EDIT2, m_InputNumber[1]);
	DDX_Control(pDX, IDC_EDIT3, m_InputNumber[2]);

	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);

	
	if (!m_bStart)
	{
		m_Radio[0].SetCheck(TRUE);
		m_eTransType = TRANSTYPE_POS;
		m_bStart = true;

	}

}

HRESULT CTrans_Dialog::GetSilderNumber(_uint index)
{

	CString strNum;
	m_InputNumber[index].GetWindowText(strNum);
	m_SilderNumber[index].SetPos(_ttoi(strNum));
	ChnageToTargetGameObject();

	return S_OK;
}

HRESULT CTrans_Dialog::ChnageToTargetGameObject()
{
	// 실제 게임 오브젝트의 크기를 변경.
	if (m_GameObject_Rect_Tool == nullptr)
		return E_FAIL;

	for (int i = 0; i < TRANSTYPE_END; i++)
	{
		if (m_Radio[i].GetCheck())
		{
			m_eTransType = (E_TRANSTYPE)i;
			break;
		}
	}

	// 현재 값 데이터 받아오기
	_float xyz[3] = {};
	for (int i =0; i<3; i++)
	{
		CString str;
		m_InputNumber[i].GetWindowText(str);
		xyz[i] = _ttoi(str);
	}
	
	switch (m_eTransType)
	{
	case E_TRANSTYPE::TRANSTYPE_POS:
		m_GameObject_Rect_Tool->Set_Position(_float3(xyz[0], xyz[1], xyz[2]));
		break;
	case E_TRANSTYPE::TRANSTYPE_ROT:
		//	m_TargetTransForm->Set_MatrixState(CTransform::STATE_POS, xyz);
		break;
	case E_TRANSTYPE::TRANSTYPE_SCALE:
		m_GameObject_Rect_Tool->Set_Scaled(_float3(xyz[0], xyz[1], xyz[2]));
		break;

	}
	return S_OK;
}


BEGIN_MESSAGE_MAP(CTrans_Dialog, CDialog)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT1, &CTrans_Dialog::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CTrans_Dialog::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CTrans_Dialog::OnEnChangeEdit3)
END_MESSAGE_MAP()


// CTrans_Dialog 메시지 처리기




BOOL CTrans_Dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	for (_uint i=0;i<3;i++)
	{
		m_SilderNumber[i].SetRange(0, m_MaxSilderSize);
		m_SilderNumber[i].SetPos(0);
		m_SilderNumber[i].SetLineSize(m_MaxSilderSize/100);
		m_SilderNumber[i].SetPageSize(m_MaxSilderSize/10);

		int pos = m_SilderNumber[i].GetPos();
		CString str;
		str.Format(TEXT("%d"), pos);
		m_InputNumber[i].SetWindowText(str);
	}

	// 랜더링 뷰와 트랜스폼을 넣어준다.
	if (m_GameObject_Rect_Tool == nullptr)
	{
		m_GameObject_Rect_Tool = GetSingle(CSuperToolSIngleton)->GetObjectRect();

		m_GameObject_Rect_Tool->AddRef();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTrans_Dialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// ID 확인 후에 3개의 슬라이더 값을 업데이트한다.

	

	for (_uint i = 0; i < 3; i++)
	{
		if (pScrollBar->GetDlgCtrlID() == m_SilderNumber[i].GetDlgCtrlID())
		{			
			int pos = m_SilderNumber[i].GetPos();
			CString str;
			str.Format(TEXT("%d"), pos);
			m_InputNumber[i].SetWindowText(str);
		}
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

}


void CTrans_Dialog::OnEnChangeEdit1()
{
	GetSilderNumber(0);
}


void CTrans_Dialog::OnEnChangeEdit2()
{
	GetSilderNumber(1);

}


void CTrans_Dialog::OnEnChangeEdit3()
{
	GetSilderNumber(2);
}



BOOL CTrans_Dialog::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.



	return CDialog::DestroyWindow();
}
