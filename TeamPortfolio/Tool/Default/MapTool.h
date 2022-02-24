#pragma once


// MapTool 대화 상자
#include "afxwin.h"
#include "Include.h"

BEGIN(Tool)
class CObjectTool_ToolObject;
END

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
private:
	void ListBoxUpdate();
	void CreateNewMap(_uint  x, _uint  y, _uint z);


	CListBox m_ListBox_Map;
	
	// XYZ 입력창
	CEdit m_EditBox[3];

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButton2();
};
