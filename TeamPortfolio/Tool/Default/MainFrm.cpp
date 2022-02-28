// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"
#include "MyForm.h"
#include "ToolView.h"
#include "MiniView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

// Ư�� ������ �޼����� �߻��� ��� ȣ��Ǵ� �Լ����� ��Ƴ��� ��ó����
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

// CMainFrame �޽��� ó����

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// �� ��ü�� �����ϴ� ������ ȣ��(����ڰ� �� ������ �����ϰų� �߰� ������ ����)

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	// CreateStatic  : ���� â�� ����� ���� �Լ�
	// 1���� : �θ� �������� �ּ�
	// 2���� : ���� ��
	// 3���� : ���� ��
	// 4���� : â ��Ÿ��
	// 5���� : �ڽ�â�� id ��

	/*m_MainSplitter.CreateStatic(this, 1, 2);

	m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyForm), CSize(300, WINCY), pContext);
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY), pContext);*/

	/*m_MainSplitter.CreateStatic(this, 2, 2);

	m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(300, 300), pContext);
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);
	m_MainSplitter.CreateView(1, 0, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);
	m_MainSplitter.CreateView(1, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY), pContext);*/

	// #Tag â�ɰ���

	m_MainSplitter.CreateStatic(this, 1, 2);

	m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyForm), CSize(300, TOOL_WINCY), pContext);
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(TOOL_WINCX, TOOL_WINCY), pContext);



	// WS_CHILD : �ڽ� â�̶�� �ǹ�
	// WS_VISIBLE : ���� �� �ٷ� ȭ�鿡 ǥ���ϰڴٴ� �ǹ�
	// IdFromRowCol : ������ ��� ���� �ش��ϴ� â�� id�� ��ȯ�ϴ� �Լ�

	// m_SecondSplitter.CreateStatic(&m_MainSplitter, 2, 1, WS_CHILD | WS_VISIBLE, m_MainSplitter.IdFromRowCol(0, 0));
	// 
	// m_SecondSplitter.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(300, 100), pContext);
	// m_SecondSplitter.CreateView(1, 0, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);
	// 
	// // �� ��ȣ, ���� ũ�� ����, ��� ������ �ּ� ũ��
	// m_MainSplitter.SetColumnInfo(0, 300, 10);

	return TRUE; //CFrameWnd::OnCreateClient(lpcs, pContext);
}