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

public:
	HRESULT ResetTexture();
	HRESULT Set_CurrentUpdate_WorldMat();


private:

	CTexture *	m_Com_Textures;
	
	// 위치 / 크기정보 Set 
	CEdit m_InputNumber[9];
	
	// 텍스처 리스트 박스
	CListBox m_TextureListBox;
	
	// 슬라이더
	CSliderCtrl m_Silder_Rotation[3];
	
	// 스핀버튼
	CSpinButtonCtrl m_SpinButton[3];

private:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButton1();	
	afx_msg void OnLbnSelchangeList1();
	void	Update_SpinButton(int index,int rot = 90);

public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDeltaposSpin0(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);

};