// MiniView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MiniView.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "ObjectTool_ToolObject.h"
#include "Camera_Tool.h"

// CMiniView
#ifdef _DEBUG

#define new DEBUG_NEW

#endif
HWND g_hWnd2 = NULL;

IMPLEMENT_DYNCREATE(CMiniView, CView)
#define  MINITIMER 0

CMiniView::CMiniView()
{
}

CMiniView::~CMiniView()
{

}

BEGIN_MESSAGE_MAP(CMiniView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMiniView 그리기입니다.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.

	CObjectTool_ToolObject* toolObj = GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject();
	if (toolObj == nullptr)
		return;

	if (m_Camera_tool == nullptr)
	{
		m_Camera_tool = static_cast<CCamera_Tool*>(GetSingle(CSuperToolSIngleton)->Get_GameObjectList(TAG_LAY(Layer_Camera_Main))->front());
		if (m_Camera_tool == nullptr)
			return;
	}

	m_Camera_tool->Set_StartPosView();
	toolObj->Set_MiniRender();
	toolObj->Set_Visble(true);


	GetSingle(CSuperToolSIngleton)->Render_Begin(COLOR_GRAY);

	GetSingle(CSuperToolSIngleton)->Get_Component_Renderer()->Render_RenderGroup();
	GetSingle(CSuperToolSIngleton)->Get_Graphics_Device()->EndScene();
	GetSingle(CSuperToolSIngleton)->Render_End(m_hWnd);
	toolObj->Set_Visble(false);
	m_Camera_tool->Set_OriginPosView();






}

// CMiniView 진단입니다.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

// CMiniView 메시지 처리기입니다.

//void CMiniView::CubeRender()
//{
//	
//	LPDIRECT3DDEVICE9 device = GetSingle(CSuperToolSIngleton)->Get_Graphics_Device();
//
//
//	// 큐브 텍스처 초기화
//	LPDIRECT3DCUBETEXTURE9 pCubeMap;
//
//	device->CreateCubeTexture
//	(256, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &pCubeMap, NULL);
//
//
//	// 백버퍼에 랜더 타겟 변경 
//	// LPDIRECT3DSURFACE9 pBackBuffer, pZBuffer;
//
//	for (DWORD i = 0; i < 6; i++)
//	{
//
//		LPDIRECT3DSURFACE9 pFace;
//		pCubeMap->GetCubeMapSurface((D3DCUBEMAP_FACES)i, 0, &pFace);
//		device->SetRenderTarget(0, pFace);
//
//		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//
//		device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 255), 1.0f, 0);
//
//		device->BeginScene();
//
//		GetSingle(CSuperToolSIngleton)->Get_Component_Renderer()->Render_RenderGroup();
//
//		device->EndScene();
//		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//
//		device->Present(NULL, NULL, g_hWnd2, NULL); // 디버그용
//		pFace->Release();
//	}
//
//	TCHAR str[64] = L"";
//	static int i = 0;
//	i++;
//	wsprintf(str, L"DDS/cubemap_MakeDDS_%d.dds", i);
//	D3DXSaveTextureToFile(str, D3DXIFF_DDS, pCubeMap, NULL);
//
//
//
//}

BOOL CMiniView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return false;
}


void CMiniView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	m_Camera_tool = nullptr;
	SetTimer(MINITIMER, 50, NULL);
	g_hWnd2 = m_hWnd;
}


void CMiniView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == MINITIMER)
		Invalidate(TRUE);
	CView::OnTimer(nIDEvent);
}


BOOL CMiniView::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	KillTimer(MINITIMER);

	return CView::DestroyWindow();
}
