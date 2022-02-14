// MySheet.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MySheet.h"


// CMySheet

IMPLEMENT_DYNAMIC(CMySheet, CPropertySheet)

CMySheet::CMySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	CPropertySheet::AddPage(&m_MyPage1);
	CPropertySheet::AddPage(&m_MyPage2);
}

CMySheet::CMySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	CPropertySheet::AddPage(&m_MyPage1);
	CPropertySheet::AddPage(&m_MyPage2);
}

CMySheet::~CMySheet()
{
}


BEGIN_MESSAGE_MAP(CMySheet, CPropertySheet)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMySheet �޽��� ó�����Դϴ�.


void CMySheet::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	ShowWindow(SW_HIDE);

	//CPropertySheet::OnClose();
}
