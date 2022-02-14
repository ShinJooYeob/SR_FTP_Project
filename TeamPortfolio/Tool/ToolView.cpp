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

#include "ObjectTool_Rect.h"

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
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()

{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CToolView::~CToolView()
{
	// #Tag Tool �Ҹ���
	Safe_Delete(m_pTerrain);
	Safe_Release(m_pComRenderer);
	Safe_Release(m_pGraphicDevice);
	

	CTextureMgr::GetInstance()->DestroyInstance();
	CDevice::GetInstance()->DestroyInstance();
	CGameInstance::Release_Engine();
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

	// #Bug �������� �׸��� ������ ������Ʈ�� �ʿ���� �����͸� ������ �ɵ�.
	CDevice::GetInstance()->Get_GameInstance()->Update_Engine_Tool(0.1f);


	// #Tag Tool Renderer
	CDevice::GetInstance()->Render_Begin();

	// ������
	m_pComRenderer->Render_RenderGroup();

	CDevice::GetInstance()->Render_End();
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
	pMainFrm->SetWindowPos(NULL, 0, 0, int(WINCX + fRowFrm), int(WINCY + fColFrm), SWP_NOZORDER);

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	// ������Ʈ / ������Ʈ ������Ÿ��

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnLButtonDown(nFlags, point);

	// #Tag ��â���� ���콺 ��ŷ

	//Invalidate : ȣ�� �� ������ wm_paint�� wm_erasebkgnd �޼����� �߻���Ŵ
	// ondraw �Լ��� �ٽ� �� �� ȣ��
	// ���ڰ��� FALSE�϶��� wm_paint�� �޽����� �߻�
	// ���ڰ��� true�϶� wm_paint�� wm_erasebkgnd �� �޼����� ���ÿ� �߻�
	// wm_erasebkgnd �޼��� : ����� ������ �޽���

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnMouseMove(nFlags, point);

	// #Tag ��â���� ���콺 MOVE

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

// #Tag �� ������ �ʱ�ȭ
HRESULT CToolView::Ready_Static_Component_Prototype()
{
	CGameInstance*		m_pGameInstance = GetSingle(CDevice)->Get_GameInstance();
	if (m_pGameInstance == nullptr)
		return E_FAIL;

	//������ �İ�ü ������Ÿ�� ����
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), m_pComRenderer = CRenderer::Create(m_pGraphicDevice))))
		return E_FAIL;

	Safe_AddRef(m_pComRenderer);

	//�����ε��� ������Ÿ�� ����
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pGraphicDevice))))
		return E_FAIL;

	//Transform ������Ÿ�� ����
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), CTransform::Create(m_pGraphicDevice))))
		return E_FAIL;

	/* ����Ʈ �ؽ�ó ������Ÿ�� ���� */
//	CTexture::TEXTUREDESC TextureDesc{};
//	TextureDesc.szFilePath = TEXT("../Bin/Resources/Textures/Default.jpg");

//	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Texture_Default"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
//		return E_FAIL;

	return S_OK;
}

HRESULT CToolView::Ready_Static_GameObject_Prototype()
{
	if (GetSingle(CGameInstance)->Add_GameObject_Prototype(TEXT("Prototype_GameObject_BackGround"), CObjectTool_Rect::Create(m_pGraphicDevice)))
		return E_FAIL;
	return S_OK;
}
HRESULT CToolView::Ready_GameObject_Layer(const _tchar * layertag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, layertag, TEXT("Prototype_GameObject_BackGround")))
		return E_FAIL;
	return S_OK;
}

HRESULT CToolView::Scene_Change(SCENEID eSceneID)
{

	// ��ü����

	CGameInstance*		m_pGameInstance = GetSingle(CDevice)->Get_GameInstance();

	if (m_pGameInstance == nullptr)
		return E_FAIL;

	return S_OK;
}