﻿// MapTool.cpp: 구현 파일
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
	srand(time(NULL));
}

CMapTool::~CMapTool()
{

}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox_Map);
	DDX_Control(pDX, IDC_EDIT1, m_EditBox[0]);
	DDX_Control(pDX, IDC_EDIT2, m_EditBox[1]);
	DDX_Control(pDX, IDC_EDIT3, m_EditBox[2]);
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

void CMapTool::CreateNewMap(_uint x, _uint y, _uint z, E_BUILDINGTYPE type)
{
	// 1. 현재 맵객체는 삭제
	GetSingle(CSuperToolSIngleton)->Update_Select_Render_None(TAG_LAY(Layer_View));
	auto objlist = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(0, TAG_LAY(Layer_Map));
	if (objlist != nullptr)
	{
		for (auto obj : *objlist)
		{
			obj->DIED();
		}
	}
	

	// 2. 내부가 빈 맵 오브젝트 생성
	_uint Length, Floor, Depth;
	Length = x;
	Floor = y;
	Depth = z;

	for (int y = 0; y < Floor; y++)
	{
		for (int z = 0; z < Depth; z++)
		{
			for (int x = 0; x < Length; x++)
			{
				bool isDraw = false;
				switch (type)
				{
				case CMapTool::BUILDINGTYPE_EMPTY:

					// 모서리 판단하기
					if (x == 0 || y == 0 || z == 0)
						isDraw = true;
					if (
						x == Length - 1 ||
						y == Floor - 1 ||
						z == Depth - 1)
						isDraw = true;

					if (isDraw)
					{
						_float3 newfloat = _float3(x, y, z);
						GetSingle(CSuperToolSIngleton)->Create_Clone_MapObject(newfloat, TAG_LAY(Layer_Map));
					}

					break;
				case CMapTool::BUILDINGTYPE_FULL:
					isDraw = true;
					if (isDraw)
					{
						_float3 newfloat = _float3(x, y, z);
						GetSingle(CSuperToolSIngleton)->Create_Clone_MapObject(newfloat, TAG_LAY(Layer_Map));
					}

					break;
				case CMapTool::BUILDINGTYPE_RAND:

					isDraw =  (rand()%100)% 2;
					if (isDraw)
					{
						_float3 newfloat = _float3(x, y, z);
						GetSingle(CSuperToolSIngleton)->Create_Clone_MapObject(newfloat, TAG_LAY(Layer_Map));
					}

					break;
				case CMapTool::BUILDINGTYPE_END:
					break;
				default:
					break;
				}


			}
		}
	}
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapTool::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON13, &CMapTool::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CMapTool::OnBnClickedButton14)
END_MESSAGE_MAP()


// MapTool 메시지 처리기


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	ListBoxUpdate();
	for (auto& box : m_EditBox)
	{
		box.SetWindowText(L"10");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMapTool::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMapTool::OnBnClickedButton2()
{
	// 프리셋 버튼1
	// 내부가 빈 건물
	_uint val[3];
	_uint count = 0;
	for (auto& box:m_EditBox)
	{
		CString text;
		GetDlgItemText(box.GetDlgCtrlID(), text);
		val[count++] = _ttoi(text);

	}
	CreateNewMap(val[0], val[1], val[2], BUILDINGTYPE_EMPTY);
}


void CMapTool::OnBnClickedButton13()
{
	// 프리셋 버튼2
	// 내부가 찬 건물
	_uint val[3];
	_uint count = 0;
	for (auto& box : m_EditBox)
	{
		CString text;
		GetDlgItemText(box.GetDlgCtrlID(), text);
		val[count++] = _ttoi(text);

	}
	CreateNewMap(val[0], val[1], val[2], BUILDINGTYPE_FULL);
}


void CMapTool::OnBnClickedButton14()
{
	// 프리셋 버튼3
	// 랜덤한 건물
	_uint val[3];
	_uint count = 0;
	for (auto& box : m_EditBox)
	{
		CString text;
		GetDlgItemText(box.GetDlgCtrlID(), text);
		val[count++] = _ttoi(text);

	}
	CreateNewMap(val[0], val[1], val[2], BUILDINGTYPE_RAND);
}



void CMapTool::OnBnClickedButton1()
{
	// 맵 세이브
	auto objlist = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(0, TAG_LAY(Layer_Map));
	NULL_CHECK_BREAK(objlist);

	GetSingle(CSuperToolSIngleton)->SaveData_Map(*objlist,this);

}


void CMapTool::OnBnClickedButton3()
{
	// 맵 로드

	// 기존 맵을 삭제하고 새 맵 데이터를 삽입한다.
	GetSingle(CSuperToolSIngleton)->Update_Select_Render_None(TAG_LAY(Layer_View));
	auto objlist = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(0, TAG_LAY(Layer_Map));
	if (objlist != nullptr)
	{
		for (auto obj : *objlist)
		{
			obj->DIED();
		}
	}

	GetSingle(CSuperToolSIngleton)->LoadData_Data(this);
}

