#pragma once
#include "afxwin.h"
#include "Include.h"

// CTrans_Dialog 대화 상자
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
	HRESULT GetSilderNumber(_uint index);
	HRESULT ChnageToTargetGameObject();


private:
	bool	m_bStart;
	_uint	m_MaxSilderSize;
	E_TRANSTYPE m_eTransType;

public:
	   
	CObjectTool_Rect* m_GameObject_Rect_Tool;


	// 위치 / 크기정보 Set 
	CSliderCtrl m_SilderNumber[3];
	CEdit m_InputNumber[3];

	// 위치 크기 회전 선택
	CButton m_Radio[3];



	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();

	virtual BOOL DestroyWindow();
};