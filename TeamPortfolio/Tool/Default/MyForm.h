#pragma once

#include "PathFind.h"
#include "Trans_Dialog.h"
#include "MapTool.h"
#include "CubeMapDialog.h"

// CMyForm 폼 뷰입니다.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	CMapTool		m_MapToolDialog;
	CCubeMapDialog	m_CubeMapToolDialog;


	CButton m_CheckCameraEnable;
	CButton m_CheckWirframeEnable;

	HRESULT Update_ViewListBox();

	HRESULT Update_PickPos();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CEdit m_GetCubeID;



public:

	virtual void OnInitialUpdate();

	CListBox m_ListBox_Objects;
	
	afx_msg void OnPathFind();
	afx_msg void OnTransform();
	afx_msg void OnObjectSave();
	afx_msg void OnMapSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonCube();
	afx_msg void OnBnClickedButton_CreateObject();
	afx_msg void OnLbnSelchangeList_ObjectSelect();
	afx_msg void OnBnClickedButton_Delete();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonCubeMap();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnEnChangeCubeID();
};
