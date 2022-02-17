#pragma once
#include "afxwin.h"
#include "Include.h"

// CPathFind ��ȭ �����Դϴ�.
// ����� �ѹ��� �ϳ��� ������..
// �̰� ��θ� ã���ִ� ���̴�.

BEGIN(Tool)
class CObjectTool_Rect;
END
class CPathFind : public CDialog
{
public:
	enum E_PathMODE
	{
		PATHMODE_PATH, PATHMODE_SELECT, PATHMODE_END
	};

	DECLARE_DYNAMIC(CPathFind)

public:
	CPathFind(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathFind();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHFIND };
#endif



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	void 	HorizontalScroll();
public:
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();
	afx_msg void OnDropFiles(HDROP hDropInfo);


private:
	wstring FindPath(wstring strname);
	HRESULT	ClearData();
public:

	// control
	CListBox m_ListBox;

	//	list<IMGPATH*>			m_PathInfoList;
	list<MYFILEPATH*>		m_MyPathInfoList;
	map<wstring, CImage*>		m_MapPngImage;

private:
	CObjectTool_Rect*			m_GameObject_Rect_Tool = nullptr;

private:
	E_PathMODE m_ePathMode;

	CStatic m_Picture;

	CString mStrTxtName;
	CComboBox mCombo1;
	CEdit mEdit2;


public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
};
	
