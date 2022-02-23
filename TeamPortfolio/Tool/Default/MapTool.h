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


	// 스테이지들의 맵을 저장한다.
	list<CObjectTool_ToolObject*> m_StageMapList;
	CListBox m_ListBox_Map;

	// 스테이지의 

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeList1();
};
