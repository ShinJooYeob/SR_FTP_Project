#pragma once


#include "afxwin.h"


// CCubeMapDialog 대화 상자

class CCubeMapDialog : public CDialog
{
	DECLARE_DYNAMIC(CCubeMapDialog)

public:
	CCubeMapDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCubeMapDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYCUBEMAP };
#endif
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	
	LPD3DXSPRITE m_pComSprite;
	LPDIRECT3DTEXTURE9 m_pCom_Tex;

	void CubeMapMake();
	HRESULT CreateTexute(wstring path);

public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
};
