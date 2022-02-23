// MapTool.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "ObjectTool_ToolObject.h"


// MapTool 대화 상자

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MAPTOOL, pParent)
{

}

CMapTool::~CMapTool()
{

}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox_Map);
}

void CMapTool::ListBoxUpdate()
{
	m_ListBox_Map.ResetContent();

	auto maplist =  GetSingle(CSuperToolSIngleton)->Get_GameObjectList(TAG_LAY(Layer_Map));
	if (maplist == nullptr)
		return;

	for (auto map: *maplist)
	{
		m_ListBox_Map.AddString(static_cast<CObjectTool_ToolObject*>(map)->Get_OutputData().strObjectName);
	}

}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// MapTool 메시지 처리기


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	ListBoxUpdate();
	


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMapTool::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
