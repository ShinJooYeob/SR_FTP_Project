#pragma once
#include "afxwin.h"
#include "Include.h"

// CTrans_Dialog 대화 상자
// 위치조정 툴

BEGIN(Tool)
class CObjectTool_Rect;
END
class CTrans_Dialog : public CDialog
{
	DECLARE_DYNAMIC(CTrans_Dialog)

public:
	CTrans_Dialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTrans_Dialog();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CTrans_Dialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	// Edit에 값에 따라 
	HRESULT EditToObjectUpdate(CEdit* edit,_uint count=9);

public:
	   
	CObjectTool_Rect* m_GameObject_Rect_Tool;

	// 위치 / 크기정보 Set 
	CEdit m_InputNumber[9];

	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButton1();
};