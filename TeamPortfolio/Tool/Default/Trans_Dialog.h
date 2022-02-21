#pragma once
#include "afxwin.h"
#include "Include.h"

// CTrans_Dialog 대화 상자
// 위치조정 툴

BEGIN(Tool)
class CObjectTool_ToolObject;
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

private:
//	const _tchar*	m_LoadFileName = L"Blocks.txt";
//	const _tchar*	m_LoadFileName2 = L"Blocks.txt";

public:
	 
	CObjectTool_ToolObject* m_GameObject_Rect_Tool;

	HRESULT ResetTexture();

private:

	// 위치 / 크기정보 Set 
	CEdit m_InputNumber[9];
	
	// 텍스처 리스트 박스
	CListBox m_TextureListBox;
	
	// 슬라이더
	CSliderCtrl m_Silder_Rotation[3];


	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButton1();
	
	afx_msg void OnLbnSelchangeList1();
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};