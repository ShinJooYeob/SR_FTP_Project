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
	for (auto obj : m_StageMapList)
		Safe_Release(obj);
	m_StageMapList.clear();
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox_Map);
}

void CMapTool::ListBoxUpdate()
{
	m_ListBox_Map.ResetContent();
	if (m_StageMapList.empty())
		return;

	for (auto map: m_StageMapList)
	{
		m_ListBox_Map.AddString(map->Get_OutputData().strObjectName);
	}

}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// MapTool 메시지 처리기


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

 // #STOP 잠시

	//for (auto obj : m_StageMapList)
	//	Safe_Release(obj);
	//m_StageMapList.clear();
	//m_ListBox_Map.ResetContent();

	//// 0,0,0 위치에 디폴트 오브젝트 생성

	//m_StageMapList.clear();
	//
	//GetSingle(CSuperToolSIngleton)->Create_New_ToolObject()->Clone();

	//ListBoxUpdate();
	


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMapTool::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
