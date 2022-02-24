#pragma once
#include "afxwin.h"
#include "Include.h"

// CPathFind 대화 상자입니다.
// 패스 정보 툴

BEGIN(Tool)
class CObjectTool_ToolObject;
END
class CPathFind : public CDialog
{
public:

	DECLARE_DYNAMIC(CPathFind)

public:
	CPathFind(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathFind();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHFIND };
#endif



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
	// 경로와 확장자 저장
	const wstring Extension = L".txt";
	CEdit mEditBox; // 파일이름 박스

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
	
