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
	enum E_BUILDINGTYPE
	{
		BUILDINGTYPE_EMPTY,
		BUILDINGTYPE_FULL,
		BUILDINGTYPE_RAND,
		BUILDINGTYPE_RAND2,
		BUILDINGTYPE_END
	};

public:
	CMapTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif


public:
	void Update_CountText(int offset=0);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

private:
	void CreateNewMap(_uint  x, _uint  y, _uint z, E_BUILDINGTYPE type);
	void CreateCustomNewMap(_uint  x, _uint  y, _uint z, _uint count, _float3 range = _float3(10,10,10));
	
private:
	// XYZ 입력창
	CEdit m_EditBox[3];
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton_PreSet1();
	afx_msg void OnBnClickedButton_PreSet3();
	afx_msg void OnBnClickedButton_Custom();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonClear();
};
