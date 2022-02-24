// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CToolView

HWND	g_hWnd;
HINSTANCE g_hInstance;

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_INPUT()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()

{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView()
{

	GetSingle(CSuperToolSIngleton)->DestroyInstance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView �׸���

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

#pragma region ������
	//pDC->Rectangle(100, 100, 200, 200);

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	/*CDevice::GetInstance()->Get_Sprite()->Draw(m_pSingle->Get_Texture()->pTexture,
	nullptr,	// ����� �̹��� ������ ���� ��Ʈ ����ü �ּҰ�, null�� ��� �̹����� 0,0�� �������� ����ϰԵ�
	nullptr,	// ����� �̹����� �߽� �� ��ǥ�� vec3 Ÿ���� �ּҰ�, null�� ��� 0,0�� �߽� ��ǥ�� ��
	nullptr,	// ��ġ ��ǥ�� ���� vec3Ÿ���� �ּҰ�, null�� ��� ��ũ�� ���� 0, 0 ��ǥ ���
	D3DCOLOR_ARGB(255, 255, 255, 255));*/
	// ����� ���� �̹����� ���� ���� ��, 0xffffffff���� �Ѱ��ָ� ���� ������ �����ϴ� �ɼ�

	/*const	TEXINFO*		pTextureInfo = CTextureMgr::GetInstance()->Get_Texture(L"Terrain", L"Tile", 2);
	if (nullptr == pTextureInfo)
		return;

	// �̹����� �߽� ��ǥ
	float	fCenterX = pTextureInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTextureInfo->tImgInfo.Height / 2.f;

	D3DXMATRIX	matWorld, matScale, matRot, matTrans;

	// dx �����ϴ� ��� ����Լ� ���� �۵��� output�ش��ϴ� ����� �ݵ�� �׵���ķ� �ʱ�ȭ �۾��� �ǽ��Ѵ�.

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(45.f));
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);

	matWorld = matScale * matRot * matTrans;

	CDevice::GetInstance()->Get_Sprite()->SetTransform(&matWorld);

	CDevice::GetInstance()->Get_Sprite()->Draw(pTextureInfo->pTexture,
		nullptr,	// ����� �̹��� ������ ���� ��Ʈ ����ü �ּҰ�, null�� ��� �̹����� 0,0�� �������� ����ϰԵ�
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),	// ����� �̹����� �߽� �� ��ǥ�� vec3 Ÿ���� �ּҰ�, null�� ��� 0,0�� �߽� ��ǥ�� ��
		nullptr,	// ��ġ ��ǥ�� ���� vec3Ÿ���� �ּҰ�, null�� ��� ��ũ�� ���� 0, 0 ��ǥ ���
		D3DCOLOR_ARGB(255, 255, 255, 255));*/
#pragma endregion ������

		/*CGameInstance*		m_pGameInstance = GetSingle(CDevice)->Get_GameInstance();
		if (m_pGameInstance == nullptr)
			return;
	*/

	GetSingle(CSuperToolSIngleton)->Update_Tool(0.03f);

	// #Tag Tool Renderer
	GetSingle(CSuperToolSIngleton)->Render_Begin();
	
	//������
	GetSingle(CSuperToolSIngleton)->Get_Component_Renderer()->Render_RenderGroup();
	GetSingle(CSuperToolSIngleton)->Render_End(m_hWnd);


}

// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG

// CToolView �޽��� ó����

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// ��ũ�� ���� ����� �����ϴ� �Լ�
	//  MM_TEXT: �ȼ� ������ ����� �����ϰڴٴ� ���ڰ�
	// 2���� : ����� ǥ���ϴ� Ŭ����
	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, (TILECY * TILEY / 2)));

	// #Tag Tool ����̽� �ʱ�ȭ
	



	// #Tag â ũ�� �缳��
		// AfxGetMainWnd : ���� ���� �����츦 ��ȯ�ϴ� ���� �Լ�
		// ��ȯŸ���� �θ�Ÿ���̾ �ڽ� Ÿ������ ����ȯ ����
	CMainFrame*	pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWindow{};

	// GetWindowRect : ���� �������� rect ������ ������ �Լ�
	pMainFrm->GetWindowRect(&rcWindow);

	// SetRect : ������ ���ڰ� ��� rect������ �����ϴ� �Լ�
	// 0,0 �������� ������ ��Ʈ ������ ������
	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

	RECT		rcMainView{};

	// ���� view â�� rect ������ ������ �Լ�
	GetClientRect(&rcMainView);

	float	fRowFrm = float(rcWindow.right - rcMainView.right);
	float	fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	// SetWindowPos : ������ â�� ��ġ �� ũ�⸦ �������ϴ� �Լ�
	// 1���� : ��ġ�� �������� z������ ���� ������
	// x��ǥ, y��ǥ, ���� ũ��, ���� ũ��
	// SWP_NOZORDER : ���� z������ �����ϰڴٴ� �÷��� ��

	pMainFrm->SetWindowPos(NULL, 0, 0, int(TOOL_WINCX + fRowFrm), int(TOOL_WINCY + fColFrm), SWP_NOZORDER);

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	// ������Ʈ / ������Ʈ ������Ÿ��


	g_hWnd = m_hWnd;

	if (FAILED(GetSingle(CSuperToolSIngleton)->InitDevice()))
	{
		AfxMessageBox(L"Device Init Failed");
		return;
	}

	SetTimer(TIMER_UPDATE, 30, NULL);

	


	return;
}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnLButtonDown(nFlags, point);

	// #Bug ��â���� ���콺 ��ŷ

	GetSingle(CGameInstance)->Update_Transform_ToWorldSpace(point);
	
	list<CGameObject*>* list =  GetSingle(CGameInstance)->Get_ObjectList_from_Layer(0, TAG_LAY(Layer_Map));
	if (list == nullptr)
		return;
	for (CGameObject* obj : *list)
	{
		if (static_cast<CObjectTool_ToolObject*>(obj)->PickObject())
			int A = 0;
	}

	


}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{	
	// �̺�Ʈ�� ������ �ɷ� ��ŷ��������..
	// DX ������ ���� ������ ��ǥ�� ���´�.
	

	// ��ŷ Ŭ���� �������� ����


}

void CToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if (nIDEvent == TIMER_UPDATE)
	{
		Invalidate(TRUE);
	}

	CScrollView::OnTimer(nIDEvent);
}


BOOL CToolView::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	KillTimer(TIMER_UPDATE);

	return CScrollView::DestroyWindow();
}

