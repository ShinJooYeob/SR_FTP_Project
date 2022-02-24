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
		m_ListBox_Map.AddString(static_cast<CObjectTool_ToolObject*>(map)->GetName());
	}

}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedButton2)
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


void CMapTool::OnBnClickedButton2()
{
	// 프리셋 버튼
	GetSingle(CSuperToolSIngleton)->Update_Select_Render_None(TAG_LAY(Layer_View));
	auto objlist = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(0,TAG_LAY(Layer_Map));
	if (objlist != nullptr)
	{
		for (auto obj : *objlist)
		{
			obj->DIED();
		}
	}
	

	// 내부가 빈 오브젝트 생성

	// 맵 오브젝트 추가
	int Length, Floor, Depth;
	Length = 5;
	Floor = 20;
	Depth = 5;



	for (int y = 0; y < Floor; y++)
	{
		for (int z = 0; z < Depth; z++)
		{
			for (int x = 0; x < Length; x++)
			{
				bool isDraw = false;
				// int newX = x;
				// int newY = y;
				// int newZ = z;
				
				// 모서리 판단하기
				if (x == 0 || y == 0 || z == 0)
					isDraw = true;
				if(x==Length-1||y== Floor -1||z== Depth -1)
					isDraw = true;

				if (isDraw)
				{
					_float3 newfloat = _float3(x, y, z);
					GetSingle(CSuperToolSIngleton)->Create_New_MapObject(newfloat, TAG_LAY(Layer_Map));

				}

			}
		}
	}

	
	auto mapList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(0, TAG_LAY(Layer_Map));
	
	// #TODO 가운데로 카메라 변경


}
