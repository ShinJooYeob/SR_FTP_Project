#pragma once

#include "PathFind.h"
#include "Trans_Dialog.h"

// CMyForm �� ���Դϴ�.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	CFont			m_Font;
	CPathFind		m_PathFind;
	CTrans_Dialog	m_TransformDialog;

	CButton m_CheckCameraEnable;
	CButton m_CheckWirframeEnable;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()



public:

	virtual void OnInitialUpdate();

	afx_msg void OnPathFind();
	afx_msg void OnTransform();
	afx_msg void OnObjectSave();
	afx_msg void OnMapSave();
	CComboBox m_ComboBox;
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButtonCube();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
