// MiniView.cpp : ���� �����Դϴ�.
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

// CMiniView �׸����Դϴ�.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

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

// CMiniView �����Դϴ�.

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

// CMiniView �޽��� ó�����Դϴ�.

//void CMiniView::CubeRender()
//{
//	
//	LPDIRECT3DDEVICE9 device = GetSingle(CSuperToolSIngleton)->Get_Graphics_Device();
//
//
//	// ť�� �ؽ�ó �ʱ�ȭ
//	LPDIRECT3DCUBETEXTURE9 pCubeMap;
//
//	device->CreateCubeTexture
//	(256, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &pCubeMap, NULL);
//
//
//	// ����ۿ� ���� Ÿ�� ���� 
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
//		device->Present(NULL, NULL, g_hWnd2, NULL); // ����׿�
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nIDEvent == MINITIMER)
		Invalidate(TRUE);
	CView::OnTimer(nIDEvent);
}


BOOL CMiniView::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	KillTimer(MINITIMER);

	return CView::DestroyWindow();
}
