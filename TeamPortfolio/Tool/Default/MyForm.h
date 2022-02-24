#pragma once

#include "PathFind.h"
#include "Trans_Dialog.h"
#include "MapTool.h"

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
	CMapTool		m_MapToolDialog;

	CButton m_CheckCameraEnable;
	CButton m_CheckWirframeEnable;

	HRESULT Update_ViewListBox();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	CObjectTool_ToolObject* m_MouseSelesctObject;

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
};
