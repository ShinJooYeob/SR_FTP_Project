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

#include "ObjectTool_Rect.h"
#include "Camera_Tool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CToolView

HWND	g_hWnd;
HINSTANCE g_hInstance;

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()

{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CToolView::~CToolView()
{
	// #Tag Tool 소멸자
	Safe_Delete(m_pTerrain);
	Safe_Release(m_pComRenderer);
	Safe_Release(m_pGraphicDevice);
	

	CTextureMgr::GetInstance()->DestroyInstance();
	CDevice::GetInstance()->DestroyInstance();
	CGameInstance::Release_Engine();
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

		/*CGameInstance*		m_pGameInstance = GetSingle(CDevice)->Get_GameInstance();
		if (m_pGameInstance == nullptr)
			return;
	*/

	// #Bug 정적으로 그리기 때문에 업데이트가 필요없고 데이터만 있으면 될듯.
	CDevice::GetInstance()->Get_GameInstance()->Update_Engine_Tool(0.1f);


	// #Tag Tool Renderer
	CDevice::GetInstance()->Render_Begin();

	// 랜더링
	m_pComRenderer->Render_RenderGroup();

	CDevice::GetInstance()->Render_End();
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

	// #Tag Tool 디바이스 초기화
	g_hWnd = m_hWnd;

	if (FAILED(CDevice::GetInstance()->InitDevice()))
	{
		AfxMessageBox(L"Device Init Failed");
		return;
	}
	if (m_pGraphicDevice == nullptr)
	{
		m_pGraphicDevice = CDevice::GetInstance()->Get_Device();
		Safe_AddRef(m_pGraphicDevice);
	}

	//if (FAILED(CTextureMgr::GetInstance()->InsertTexture(TEX_SINGLE, L"../Texture/Cube.png", L"CUBE")))
	//{
	//	AfxMessageBox(L"Cube Texture Insert Failed");
	//	return;
	//}

	//m_pTerrain = new CTerrain;
	//m_pTerrain->Initialize();
	//m_pTerrain->SetMainView(this);

	// #Tag 창 크기 재설정
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

	if (FAILED(Ready_Static_Component_Prototype()))
	{
		FAILED_TOOL
	}

	if (FAILED(Ready_Static_GameObject_Prototype()))
	{
		FAILED_TOOL
	}

	if (FAILED(Ready_GameObject_Layer(TEXT("OBJECTLAYER"))))
	{
		FAILED_TOOL
	}

	if (FAILED(Scene_Change(SCENEID::SCENE_LOBY)))
	{
		FAILED_TOOL
	}
	return;
}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDown(nFlags, point);

	// #Tag 툴창에서 마우스 피킹

	//Invalidate : 호출 시 윈도우 wm_paint와 wm_erasebkgnd 메세지를 발생시킴
	// ondraw 함수를 다시 한 번 호출
	// 인자값이 FALSE일때는 wm_paint만 메시지만 발생
	// 인자값이 true일때 wm_paint와 wm_erasebkgnd 두 메세지를 동시에 발생
	// wm_erasebkgnd 메세지 : 배경을 지우라는 메시지

	Invalidate(FALSE);

	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	//CMainFrame*	pMain = dynamic_cast<CMainFrame*>(GetParentFrame());

	//CMiniView*	pMiniView = dynamic_cast<CMiniView*>(pMain->m_SecondSplitter.GetPane(0, 0));

	//CMyForm*	pMyForm = dynamic_cast<CMyForm*>(pMain->m_SecondSplitter.GetPane(1, 0));

	//CMapTool*	pMapTool = &pMyForm->m_MapTool;

	//m_pTerrain->TileChange(D3DXVECTOR3(point.x + GetScrollPos(0),
	//	point.y + GetScrollPos(1),
	//	0.f), pMapTool->m_iDrawID);

	// pMiniView->Invalidate(FALSE);
	Invalidate(FALSE);
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseMove(nFlags, point);

	// #Tag 툴창에서 마우스 MOVE

	//if (GetAsyncKeyState(VK_LBUTTON))
	//{
	//
	//	Invalidate(FALSE);

	//	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	//	CMiniView*	pMiniView = dynamic_cast<CMiniView*>(pMain->m_SecondSplitter.GetPane(0, 0));
	//	CMyForm*	pMyForm = dynamic_cast<CMyForm*>(pMain->m_SecondSplitter.GetPane(1, 0));
	//	CMapTool*	pMapTool = &pMyForm->m_MapTool;

	//	m_pTerrain->TileChange(D3DXVECTOR3(point.x + GetScrollPos(0), point.y + GetScrollPos(1), 0.f), pMapTool->m_iDrawID);

	//	pMiniView->Invalidate(FALSE);
	//}
}


HRESULT CToolView::Ready_Static_Component_Prototype()
{
	// #Tag 툴 프로토 컴포넌트 초기화
	CGameInstance*		m_pGameInstance = GetSingle(CDevice)->Get_GameInstance();
	if (m_pGameInstance == nullptr)
		return E_FAIL;

	//렌더러 컴객체 프로토타입 생성
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), m_pComRenderer = CRenderer::Create(m_pGraphicDevice))))
		return E_FAIL;

	Safe_AddRef(m_pComRenderer);

	//버퍼인덱스 프로토타입 생성
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pGraphicDevice))))
		return E_FAIL;

	//Transform 프로토타입 생성
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), CTransform::Create(m_pGraphicDevice))))
		return E_FAIL;

	/* 디폴트 텍스처 프로토타입 생성 */
//	CTexture::TEXTUREDESC TextureDesc{};
//	TextureDesc.szFilePath = TEXT("../Bin/Resources/Textures/Default.jpg");

//	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Texture_Default"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
//		return E_FAIL;

	return S_OK;
}

HRESULT CToolView::Ready_Static_GameObject_Prototype()
{
	// #Tag 툴 프로토 오브젝트 초기화
	// 임의 객체 생성
	if (GetSingle(CGameInstance)->Add_GameObject_Prototype(TEXT("Prototype_GameObject_BackGround"), CObjectTool_Rect::Create(m_pGraphicDevice)))
		return E_FAIL;

	// 카메라 생성
	if (GetSingle(CGameInstance)->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Camera"), CCamera_Tool::Create(m_pGraphicDevice)))
		return E_FAIL;

	return S_OK;
}
HRESULT CToolView::Ready_GameObject_Layer(const _tchar * layertag)
{
	// #Tag 툴 클론 오브젝트 생성
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, layertag, TEXT("Prototype_GameObject_BackGround")))
		return E_FAIL;

	// 카메라

	CCamera::CAMERADESC CameraDesc;
	CameraDesc.vWorldRotAxis = _float3(0, 0, 0);
	CameraDesc.vEye = _float3(0, 0, -5.f);
	CameraDesc.vAt = _float3(0, 0, 0);
	CameraDesc.vAxisY = _float3(0, 1, 0);

	CameraDesc.fFovy = D3DXToRadian(90);
	CameraDesc.fAspect = _float(TOOL_WINCX) / TOOL_WINCY;
	CameraDesc.fNear = 1.0f;
	CameraDesc.fFar = 100.0f;

	CameraDesc.TransformDesc.fMovePerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);


	//if (FAILED(m_pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Camera_Main), CCamera_Main::Create(m_pGraphicDevice, &CameraDesc))))
		
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, layertag, TEXT("Prototype_GameObject_Camera"), &CameraDesc))
		return E_FAIL;

	return S_OK;
}

HRESULT CToolView::Scene_Change(SCENEID eSceneID)
{

	// 씬체인지

	CGameInstance*		m_pGameInstance = GetSingle(CDevice)->Get_GameInstance();

	if (m_pGameInstance == nullptr)
		return E_FAIL;

	return S_OK;
}