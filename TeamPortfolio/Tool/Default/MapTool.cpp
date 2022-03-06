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
	srand(time(NULL));
}

CMapTool::~CMapTool()
{

}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditBox[0]);
	DDX_Control(pDX, IDC_EDIT2, m_EditBox[1]);
	DDX_Control(pDX, IDC_EDIT3, m_EditBox[2]);
}



void CMapTool::Update_CountText(int offset)
{
	auto maplist = GetSingle(CSuperToolSIngleton)->Get_GameObjectList(TAG_LAY(Layer_Map));
	if (maplist == nullptr)
	{
		SetDlgItemText(IDC_STATIC1, L"0");
		return;
	}
	int count = 0;
	for (auto& obj : *maplist)
	{
		if(obj->Get_IsDied() == false)
			count++;
	}

	
	count += offset;
	_tchar countstr[16];
	_itot_s(count, countstr,10);
	SetDlgItemText(IDC_STATIC1, countstr);

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
	Update_CountText();
}

void CMapTool::CreateCustomNewMap(_uint x, _uint y, _uint z, _uint _count, _float3 range )
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


	// 우주맵 생성
	_uint Length, Floor, Depth;
	Length = x;
	Floor = y;
	Depth = z;


	GetSingle(CSuperToolSIngleton)->Create_Clone_MapObject(_float3(0,0,0), TAG_LAY(Layer_Map));
	int halfX = (int)range.x*0.5;
	int halfY = (int)range.y*0.5;
	int halfZ = (int)range.z*0.5;

	for (int count = 0; count < _count; count++)
	{

		int OffsetX = (rand() % (int)range.x) - halfX;
		int OffsetY = (rand() % (int)range.y); // 무조건 위로
		int OffsetZ = (rand() % (int)range.z) - halfZ;
		_uint RandomTex = rand() % 7;

		for (int y = 0; y < Floor; y++)
		{
			for (int z = 0; z < Depth; z++)
			{
				for (int x = 0; x < Length; x++)
				{
					bool isDraw = false;
					isDraw = (rand() % 100) % 2;
					if (isDraw)
					{
						_float3 newfloat = _float3(x+ OffsetX, y+ OffsetY, z+ OffsetZ);
						GetSingle(CSuperToolSIngleton)->Create_Clone_MapObject(newfloat, TAG_LAY(Layer_Map),true, RandomTex);
					}
				}
			}
		}
	}
	Update_CountText();

}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedButton_PreSet1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapTool::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON14, &CMapTool::OnBnClickedButton_PreSet3)
	ON_BN_CLICKED(IDC_BUTTON13, &CMapTool::OnBnClickedButton_Custom)

	ON_BN_CLICKED(IDC_BUTTON4, &CMapTool::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// MapTool 메시지 처리기


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (auto& box : m_EditBox)
	{
		box.SetWindowText(L"5");
	}

	CreateNewMap(1,1,1,BUILDINGTYPE_FULL);

	Update_CountText();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CMapTool::OnBnClickedButton_PreSet1()
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


void CMapTool::OnBnClickedButton_Custom()
{
	// 커스턴 버튼 랜덤 수정용
	// 지금은 막음
	return;
	
	_uint val[3];
	_uint count = 0;
	for (auto& box : m_EditBox)
	{
		CString text;
		GetDlgItemText(box.GetDlgCtrlID(), text);
		val[count++] = _ttoi(text);

	}
	CreateCustomNewMap(val[0], val[1], val[2],80,_float3(30,200,30));
}


void CMapTool::OnBnClickedButton_PreSet3()
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




BOOL CMapTool::PreTranslateMessage(MSG* pMsg)
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

void CMapTool::OnBnClickedButtonClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	auto objlist = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(0, TAG_LAY(Layer_Map));
	NULL_CHECK_BREAK(objlist);
	for (CGameObject* obj: *objlist)
	{
		obj->DIED();
	}
	Update_CountText();	
}
