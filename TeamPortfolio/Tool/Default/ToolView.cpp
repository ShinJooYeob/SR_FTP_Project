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
	// ǥ�� �μ� ����Դϴ�.
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

// CToolView ����/�Ҹ�

CToolView::CToolView()

{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	m_AddPos = _float3(0, 1, 0);
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



//	UnlockWindowUpdate();
	GetSingle(CSuperToolSIngleton)->Update_Tool(0.03f);

	LPDIRECT3DSURFACE9 pBackBuffer = nullptr;
	chain1->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	GetSingle(CSuperToolSIngleton)->Get_Graphics_Device()->SetRenderTarget(0, pBackBuffer);

	// #Tag Tool Renderer
	GetSingle(CSuperToolSIngleton)->Render_Begin();
	
	//������
	GetSingle(CSuperToolSIngleton)->Get_Component_Renderer()->Render_RenderGroup();
	
	GetSingle(CSuperToolSIngleton)->Get_Graphics_Device()->EndScene();
	chain1->Present(NULL, NULL, g_hWnd, NULL, 0);

//	LockWindowUpdate();

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
	// ���콺 �� �÷��� ��
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
	// ������ ��
	else
	{
		if (m_AddPos == UPVEC)
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnLButtonDown(nFlags, point);
	if (m_Nearobj == nullptr)
		return;

	// �ش���ġ�� ��ü�� �ִٸ� ����
	list<CGameObject*>* maplist = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(0, TAG_LAY(Layer_Map));
	if (maplist == nullptr)
		return;
	for (auto obj : *maplist)
	{
		if ((m_PickPos + m_AddPos) == ((CObjectTool_ToolObject*)obj)->Get_Pos())
			return;
	}


	
	// ������ ������Ʈ ����
	GetSingle(CSuperToolSIngleton)->Create_Clone_MapObject(m_PickPos + m_AddPos, TAG_LAY(Layer_Map));
	CMapTool* mapToolView = GetSingle(CSuperToolSIngleton)->GetMapTool();
	if (mapToolView == nullptr)
		return;
	mapToolView->Update_CountText();
}
void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnLButtonUp(nFlags, point);

}



void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{	
	// �̺�Ʈ�� ������ �ɷ� ��ŷ��������..
	// DX ������ ���� ������ ��ǥ�� ���´�.

	// ��ŷ Ŭ���� �������� ����

	GetSingle(CGameInstance)->Update_Transform_ToWorldSpace(point);

	list<CGameObject*>* maplist = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(0, TAG_LAY(Layer_Map));
	if (maplist == nullptr)
		return;

	CTransform* tr =  (CTransform*)GetSingle(CSuperToolSIngleton)->Get_WireCube()->Get_Component(TAG_COM(Com_Transform));
	tr->Scaled(_float3(1.1f, 1.1f, 1.1f));

	// ��ŷ�� ������Ʈ���� �����ͼ� ī�޶�� ���� �ڿ� ���� ����� �Ѱ��� ��ȯ�ϰ� ����.
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
	}
	else
	{
		return;
	}
	
	// ��ŷ�� ������Ʈ�� ��������� ���� ������ Ÿ�� ��ġ ����
	// �븻�� ��� ���� ��� / �ϴ� ���߿�

	//if(m_Nearobj)
	//	m_NextPos = m_Nearobj->Get_Pos();

	// ��ŷ�� ���� ��ġ ���� �ڵ� 
	//_float3* vertex = m_Nearobj->GetPickVertex3();
	//_float3 a = vertex[1] - vertex[0];
	//_float3 b = vertex[2] - vertex[0];
	//_float3 nomalVec = _float3(0, 0, 0);	
	//D3DXVec3Cross(&nomalVec, &a, &b);
	//nomalVec.z *= -1;	
	//D3DXVec3Normalize(&nomalVec, &nomalVec);
	//_float3 AddVec = OneVector(nomalVec);
	//m_NextPos += AddVec;

	// ���� ���� ������Ʈ�� �����ؾ��Ѵ�.
	CObjectTool_ToolObject* selectobj = GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject();
	if (selectobj == nullptr) 
		return;
	GetSingle(CSuperToolSIngleton)->Get_WireCube()->Set_TransformMat(selectobj->Get_Matrix());
	GetSingle(CSuperToolSIngleton)->Get_WireCube()->Set_Pos(m_PickPos + m_AddPos);




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




BOOL CToolView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return true;
}


BOOL CToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.	
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.


	CObjectTool_ToolObject* selectobj = GetSingle(CSuperToolSIngleton)->Get_ViewObject_SelectObject();
	if (selectobj == nullptr)
		return;
	GetSingle(CSuperToolSIngleton)->Get_WireCube()->Set_TransformMat(selectobj->Get_Matrix());
	GetSingle(CSuperToolSIngleton)->Get_WireCube()->Set_Pos(m_PickPos + m_AddPos);

	CScrollView::OnMButtonDown(nFlags, point);
}
