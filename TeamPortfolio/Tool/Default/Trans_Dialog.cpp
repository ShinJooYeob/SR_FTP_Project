// Trans_Dialog.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Trans_Dialog.h"
#include "TextureLayer.h"
#include "ObjectTool_ToolObject.h"
#include "afxdialogex.h"


// CTrans_Dialog 대화 상자

IMPLEMENT_DYNAMIC(CTrans_Dialog, CDialog)

CTrans_Dialog::CTrans_Dialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CTrans_Dialog, pParent)
{
	m_Com_Textures = nullptr;
}

CTrans_Dialog::~CTrans_Dialog()
{

	if (m_Com_Textures)
		Safe_Release(m_Com_Textures);
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
	DDX_Control(pDX, IDC_SPIN1, m_SpinButton[0]);
	DDX_Control(pDX, IDC_SPIN2, m_SpinButton[1]);
	DDX_Control(pDX, IDC_SPIN3, m_SpinButton[2]);
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

	for (int i = 0; i < 3; i++)
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
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CTrans_Dialog::OnDeltaposSpin0)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CTrans_Dialog::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CTrans_Dialog::OnDeltaposSpin2)

END_MESSAGE_MAP()


HRESULT CTrans_Dialog::ResetTexture()
{
	// 텍스처 리스트 업데이트
	
	// 모든 텍스처 데이터 업데이트
	if (m_Com_Textures == nullptr)
	{
		CObjectTool_ToolObject* CurrentToolObject = GetSingle(CSuperToolSIngleton)->Get_ViewObject_Object();
		if (CurrentToolObject == nullptr)
			return E_FAIL;

		m_Com_Textures = static_cast<CTexture*>(CurrentToolObject->Get_Component(TAG_COM(Com_Texture)));
		NULL_CHECK_BREAK(m_Com_Textures);
		m_Com_Textures->AddRef();
	}

	if (m_Com_Textures)
	{
		auto iterbegin= m_Com_Textures->Get_SaveTextureMap().begin();
		auto iterend = m_Com_Textures->Get_SaveTextureMap().end();
		m_TextureListBox.ResetContent();
		// 전체 TextureMap 순회

		for (auto iter = iterbegin;iter!=iterend ;iter++)
		{
			wstring wstr = (iter->first);
			CTextureLayer* layer= (iter->second);
			wstring wstr2 = L"_%d";
			wstr += wstr2;
			// 레이어 개수 만큼 업데이트
			for (int i=0; i <= layer->Get_TextureNum();i++)
			{
				TCHAR fomatText[64];
				wsprintf(fomatText, wstr.c_str(), i);
				m_TextureListBox.AddString(fomatText);
			}
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
	ResetTexture();
	EditToObjectUpdate(m_InputNumber,9);
}


void CTrans_Dialog::OnLbnSelchangeList1()
{
	// 리스트 박스 선택시 이벤트

	CObjectTool_ToolObject* CurrentToolObject = GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject();
	NULL_CHECK(CurrentToolObject);

	// 해당 스테이트키 + 넘버를 해석해서 현재 텍스쳐에 넣는다.

	if (m_Com_Textures)
	{
		// 선택한 리스트의 키 _ 번호 획득
		CString cstr;
		_uint index = m_TextureListBox.GetCurSel();
		m_TextureListBox.GetText(index, cstr);
		int _index = cstr.Find(L"_", 0);
		CString KeyCStr = cstr.Left(_index);
		CString NumCStr = cstr.Right(1);
		int keyindex = _ttoi(NumCStr);

		// 키와 번호를 추출해서 오브젝트에 적용한다.
		CurrentToolObject->Set_StateKey_TextureNum_Bind(KeyCStr, keyindex);
	}
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

BOOL CTrans_Dialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN)
	{
		//이스케이프키일 경우 함수 종료
		if (pMsg->wParam == VK_DOWN || pMsg->wParam == VK_UP ||
			pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT)
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);

}

void CTrans_Dialog::Update_SpinButton(int index, int rot)
{

	int CurRot = m_Silder_Rotation[index].GetPos();
	int moveRot = CurRot + rot;
	if (moveRot > 360 || moveRot < 0)
		return;

	m_Silder_Rotation[index].SetPos(moveRot);

	CString sPos;
	sPos.Format(_T("%d"), moveRot);
	m_InputNumber[index + 3].SetWindowText(sPos);
	EditToObjectUpdate(m_InputNumber, 9);

}


void CTrans_Dialog::OnDeltaposSpin0(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 90도 씩 회전 하도록 처리
	int delta = pNMUpDown->iDelta;
	if (delta>0)
	{
		Update_SpinButton(0, -90);
	}
	else
	{
		Update_SpinButton(0, 90);

	}
	
	*pResult = 0;
}

void CTrans_Dialog::OnDeltaposSpin1(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int delta = pNMUpDown->iDelta;
	if (delta > 0)
	{
		Update_SpinButton(1, -90);

	}
	else
	{
		Update_SpinButton(1, 90);

	}
	*pResult = 0;

}

void CTrans_Dialog::OnDeltaposSpin2(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int delta = pNMUpDown->iDelta;
	if (delta > 0)
	{
		Update_SpinButton(2, -90);

	}
	else
	{
		Update_SpinButton(2, 90);

	}
	*pResult = 0;

}
