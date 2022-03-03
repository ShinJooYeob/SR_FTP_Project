// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "MyForm.h"

#include "Renderer.h"

#include "GameObject.h"
#include "ObjectTool_ToolObject.h"
#include "ObjectTool_ToolWire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CToolView

HWND	g_hWnd;
HINSTANCE g_hInstance;

LPDIRECT3DSWAPCHAIN9 chain1 = nullptr;

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_INPUT()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()

{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_AddPos = _float3(0, 1, 0);
}

CToolView::~CToolView()
{

	GetSingle(CSuperToolSIngleton)->DestroyInstance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

#pragma region 복습용
	//pDC->Rectangle(100, 100, 200, 200);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	/*CDevice::GetInstance()->Get_Sprite()->Draw(m_pSingle->Get_Texture()->pTexture,
	nullptr,	// 출력할 이미지 영역에 대한 렉트 구조체 주소값, null인 경우 이미지의 0,0을 기준으로 출력하게됨
	nullptr,	// 출력할 이미지의 중심 축 좌표를 vec3 타입의 주소값, null인 경우 0,0이 중심 좌표가 됨
	nullptr,	// 위치 좌표에 대한 vec3타입의 주소값, null인 경우 스크린 상의 0, 0 좌표 출력
	D3DCOLOR_ARGB(255, 255, 255, 255));*/
	// 출력할 원본 이미지와 섞을 색상 값, 0xffffffff값을 넘겨주면 원본 색상을 유지하는 옵션

	/*const	TEXINFO*		pTextureInfo = CTextureMgr::GetInstance()->Get_Texture(L"Terrain", L"Tile", 2);
	if (nullptr == pTextureInfo)
		return;

	// 이미지의 중심 좌표
	float	fCenterX = pTextureInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTextureInfo->tImgInfo.Height / 2.f;

	D3DXMATRIX	matWorld, matScale, matRot, matTrans;

	// dx 제공하는 모든 행렬함수 최초 작동시 output해당하는 행렬을 반드시 항등행렬로 초기화 작업을 실시한다.

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(45.f));
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);

	matWorld = matScale * matRot * matTrans;

	CDevice::GetInstance()->Get_Sprite()->SetTransform(&matWorld);

	CDevice::GetInstance()->Get_Sprite()->Draw(pTextureInfo->pTexture,
		nullptr,	// 출력할 이미지 영역에 대한 렉트 구조체 주소값, null인 경우 이미지의 0,0을 기준으로 출력하게됨
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),	// 출력할 이미지의 중심 축 좌표를 vec3 타입의 주소값, null인 경우 0,0이 중심 좌표가 됨
		nullptr,	// 위치 좌표에 대한 vec3타입의 주소값, null인 경우 스크린 상의 0, 0 좌표 출력
		D3DCOLOR_ARGB(255, 255, 255, 255));*/
#pragma endregion 복습용



//	UnlockWindowUpdate();
	GetSingle(CSuperToolSIngleton)->Update_Tool(0.03f);

	LPDIRECT3DSURFACE9 pBackBuffer = nullptr;
	chain1->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	GetSingle(CSuperToolSIngleton)->Get_Graphics_Device()->SetRenderTarget(0, pBackBuffer);

	// #ToolTag Tool Renderer
	GetSingle(CSuperToolSIngleton)->Render_Begin(COLOR_BACK_TOOLA);
	
	//랜더링
	GetSingle(CSuperToolSIngleton)->Get_Component_Renderer()->Render_RenderGroup();
	
	GetSingle(CSuperToolSIngleton)->Get_Graphics_Device()->EndScene();
	chain1->Present(NULL, NULL, g_hWnd, NULL, 0);

//	LockWindowUpdate();

}

// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG

// CToolView 메시지 처리기

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// 스크롤 바의 사이즈를 지정하는 함수
	//  MM_TEXT: 픽셀 단위로 사이즈를 조정하겠다는 인자값
	// 2인자 : 사이즈를 표현하는 클래스
	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, (TILECY * TILEY / 2)));


	// AfxGetMainWnd : 현재 메인 윈도우를 반환하는 전역 함수
	// 반환타입이 부모타입이어서 자식 타입으로 형변환 했음
	CMainFrame*	pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWindow{};

	// GetWindowRect : 현재 윈도우의 rect 정보를 얻어오는 함수
	pMainFrm->GetWindowRect(&rcWindow);

	// SetRect : 지정한 인자값 대로 rect정보를 세팅하는 함수
	// 0,0 기준으로 윈도우 렉트 정보를 재조정
	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

	RECT		rcMainView{};

	// 현재 view 창의 rect 정보를 얻어오는 함수
	GetClientRect(&rcMainView);

	float	fRowFrm = float(rcWindow.right - rcMainView.right);
	float	fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	// SetWindowPos : 윈도우 창의 위치 및 크기를 재조정하는 함수
	// 1인자 : 배치할 윈도우의 z순서에 대한 포인터
	// x좌표, y좌표, 가로 크기, 세로 크기
	// SWP_NOZORDER : 현재 z순서를 유지하겠다는 플래그 값

	pMainFrm->SetWindowPos(NULL, 0, 0, int(TOOL_WINCX + fRowFrm), int(TOOL_WINCY + fColFrm), SWP_NOZORDER);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// 오브젝트 / 컴포넌트 프로토타입


	g_hWnd = m_hWnd;

	if (FAILED(GetSingle(CSuperToolSIngleton)->InitDevice()))
	{
		AfxMessageBox(L"Device Init Failed");
		return;
	}


	GetSingle(CSuperToolSIngleton)->Get_Graphics_Device()->GetSwapChain(0, &chain1);
	D3DPRESENT_PARAMETERS param;
	chain1->GetPresentParameters(&param);
//	GetSingle(CSuperToolSIngleton)->Get_Graphics_Device()->CreateAdditionalSwapChain(&param, &chain2);


	SetTimer(TIMER_UPDATE, 50, NULL);

	m_Nearobj = nullptr;


	return;
}

void CToolView::Change_PickVector(bool b)
{
	// 마우스 휠 올렸을 때
	if (b)
	{
		if (m_AddPos == UPVEC)
		{
			m_AddPos = RIGHTVEC;
			return;
		}
		if (m_AddPos == RIGHTVEC)
		{
			m_AddPos = DOWNVEC;
			return;
		}
		if (m_AddPos == DOWNVEC)
		{
			m_AddPos = LEFTVEC;
			return;
		}
		if (m_AddPos == LEFTVEC)
		{
			m_AddPos = FRONTVEC;
			return;
		}
		if (m_AddPos == FRONTVEC)
		{
			m_AddPos = BACKVEC;
			return;
		}
		if (m_AddPos == BACKVEC)
		{
			m_AddPos = UPVEC;
			return;
		}

	}
	// 내렸을 때
	else
	{
		if (m_AddPos == UPVEC)
		{
			m_AddPos = BACKVEC;
			return;
		}
		if (m_AddPos == BACKVEC)
		{
			m_AddPos = FRONTVEC;
			return;
		}
		if (m_AddPos == FRONTVEC)
		{
			m_AddPos = LEFTVEC;
			return;
		}

		if (m_AddPos == LEFTVEC)
		{
			m_AddPos = DOWNVEC;
			return;
		}
		if (m_AddPos == DOWNVEC)
		{
			m_AddPos = RIGHTVEC;
			return;
		}
		if (m_AddPos == RIGHTVEC)
		{
			m_AddPos = UPVEC;
			return;
		}
	}
}

void CToolView::Add_Axis_PickVector()
{
}


_float3 CToolView::OneVector(_float3 nomalVec)
{
	bool bMinX= false;
	bool bMinY= false;
	bool bMinZ = false;
	if (nomalVec.x < 0)
		bMinX = true;
	if (nomalVec.y < 0)
		bMinY = true;
	if (nomalVec.z < 0)
		bMinZ = true;
	
	_float3 newNomal(abs(nomalVec.x), abs(nomalVec.y), abs(nomalVec.z));
	float max = newNomal.x > newNomal.y ? newNomal.x : newNomal.y;
	max = max > newNomal.z ? max : newNomal.z;

	if (newNomal.x == max)
	{
		if (bMinX)
			return _float3(-1, 0, 0);
		else
			return _float3(1, 0, 0);

	}
	if (newNomal.y == max)
	{
		if (bMinY)
			return _float3(0, -1, 0);
		else
			return _float3(0, 1, 0);
	}
	if (newNomal.z == max)
	{
		if (bMinZ)
			return _float3(0, 0, -1);
		else
			return _float3(0, 0, 1);
	}

}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDown(nFlags, point);
	if (m_Nearobj == nullptr)
		return;

	// 해당위치에 객체가 있다면 리턴
	list<CGameObject*>* maplist = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(0, TAG_LAY(Layer_Map));
	if (maplist == nullptr)
		return;
	for (auto obj : *maplist)
	{
		if ((m_PickPos + m_AddPos) == ((CObjectTool_ToolObject*)obj)->Get_Pos())
			return;
	}


	
	// 선택한 오브젝트 생성
	GetSingle(CSuperToolSIngleton)->Create_Clone_MapObject(m_PickPos + m_AddPos, TAG_LAY(Layer_Map));
	CMapTool* mapToolView = GetSingle(CSuperToolSIngleton)->GetMapTool();
	if (mapToolView == nullptr)
		return;
	mapToolView->Update_CountText();
}
void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnRButtonDown(nFlags, point);
	if (m_Nearobj == nullptr)
		return;

	m_Nearobj->DIED();

	CMapTool* map = GetSingle(CSuperToolSIngleton)->GetMapTool();
	if (map == nullptr)
		return;
	map->Update_CountText(-1);

}


void CToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonUp(nFlags, point);

}



void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{	
	// 이벤트로 들어오는 걸로 피킹수행하자..
	// DX 랜더된 곳의 윈도우 좌표가 들어온다.

	// 피킹 클래스 툴용으로 수정

	GetSingle(CGameInstance)->Update_Transform_ToWorldSpace(point);

	list<CGameObject*>* maplist = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(0, TAG_LAY(Layer_Map));
	if (maplist == nullptr)
		return;

	CTransform* tr =  (CTransform*)GetSingle(CSuperToolSIngleton)->Get_WireCube()->Get_Component(TAG_COM(Com_Transform));
	tr->Scaled(_float3(1.1f, 1.1f, 1.1f));

	// 피킹된 오브젝트들을 가져와서 카메라와 비교한 뒤에 가장 가까운 한개만 반환하게 하자.
	list<CObjectTool_ToolObject*> picklist;


	for (CGameObject* obj : *maplist)
	{
		CObjectTool_ToolObject* toolobj = static_cast<CObjectTool_ToolObject*>(obj);
		if (toolobj->PickObject())
		{
			picklist.push_back(toolobj);
			toolobj->Set_MyCamDistance();
		}
	}

	if (picklist.empty())
		return;

	CObjectTool_ToolObject* newpickobj = picklist.front();

	float CurDistance = newpickobj->Get_CamDistance();
	for (CObjectTool_ToolObject* obj : picklist)
	{
		if (CurDistance > obj->Get_CamDistance())
		{
			CurDistance = obj->Get_CamDistance();
			newpickobj = obj;
		}
		
	}

	if (m_Nearobj != newpickobj)
	{
		m_Nearobj = newpickobj;
		m_PickPos = m_Nearobj->Get_Pos();
		GetSingle(CSuperToolSIngleton)->GetMyButtonView()->Update_PickPos();

	}
	else
	{
		return;
	}
	
	// 피킹된 오브젝트의 면기준으로 다음 생성될 타일 위치 결정
	// 노말이 없어서 직접 계산 / 일단 나중에

	//if(m_Nearobj)
	//	m_NextPos = m_Nearobj->Get_Pos();

	// 피킹시 다음 위치 결정 코드 
	//_float3* vertex = m_Nearobj->GetPickVertex3();
	//_float3 a = vertex[1] - vertex[0];
	//_float3 b = vertex[2] - vertex[0];
	//_float3 nomalVec = _float3(0, 0, 0);	
	//D3DXVec3Cross(&nomalVec, &a, &b);
	//nomalVec.z *= -1;	
	//D3DXVec3Normalize(&nomalVec, &nomalVec);
	//_float3 AddVec = OneVector(nomalVec);
	//m_NextPos += AddVec;

	// 현재 선택 오브젝트를 복사해야한다.
	CObjectTool_ToolObject* selectobj = GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject();
	if (selectobj == nullptr) 
		return;
	GetSingle(CSuperToolSIngleton)->Get_WireCube()->Set_TransformMat(selectobj->Get_Matrix());
	GetSingle(CSuperToolSIngleton)->Get_WireCube()->Set_Pos(m_PickPos + m_AddPos);
	

	

	
}

void CToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (nIDEvent == TIMER_UPDATE)
	{
		Invalidate(TRUE);
	}

	CScrollView::OnTimer(nIDEvent);
}


BOOL CToolView::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	KillTimer(TIMER_UPDATE);

	return CScrollView::DestroyWindow();
}




BOOL CToolView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;
}


BOOL CToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.	
	if (zDelta > 0)
	{
		Change_PickVector(TRUE);
	}
	else 
	{
		Change_PickVector(FALSE);

	}
	CObjectTool_ToolObject* selectobj = GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject();
	if (selectobj == nullptr)
		return FALSE;
	GetSingle(CSuperToolSIngleton)->Get_WireCube()->Set_TransformMat(selectobj->Get_Matrix());
	GetSingle(CSuperToolSIngleton)->Get_WireCube()->Set_Pos(m_PickPos + m_AddPos);

	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);


}


void CToolView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	CObjectTool_ToolObject* selectobj = GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject();
	if (selectobj == nullptr)
		return;
	GetSingle(CSuperToolSIngleton)->Get_WireCube()->Set_TransformMat(selectobj->Get_Matrix());
	GetSingle(CSuperToolSIngleton)->Get_WireCube()->Set_Pos(m_PickPos + m_AddPos);

	CScrollView::OnMButtonDown(nFlags, point);
}
