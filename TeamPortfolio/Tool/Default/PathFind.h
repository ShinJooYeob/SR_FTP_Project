#pragma once
#include "afxwin.h"
#include "Include.h"

// CPathFind ��ȭ �����Դϴ�.
// �н� ���� ��

BEGIN(Tool)
class CObjectTool_ToolObject;
END
class CPathFind : public CDialog
{
public:

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
	wstring GetSaveFilePath();
	HRESULT	ClearPathData();
	HRESULT Update_PathListData();


private:
	// ��ο� Ȯ���� ����
	const wstring Extension = L".txt";
	CEdit mEditBox; // �����̸� �ڽ�

	// control
	CListBox				m_ListBox;
	CComboBox				m_ComboBox_Filetype;
	list<IMGPATH*>			m_PathInfoList;
	E_FILETYPE				m_ePathFileType;


public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
};
	
