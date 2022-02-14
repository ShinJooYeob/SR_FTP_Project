#pragma once

#include "Include.h"
#include "afxwin.h"

// CUnitTool ��ȭ �����Դϴ�.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUnitTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButton1();
	afx_msg void OnAddCharacter();
	afx_msg void OnListBox();

public:
	// value
	CString m_strTemp;
	CString m_strResult;
	CString m_strName;
	int m_iAttack;
	int m_iDef;
	CString m_strFindName;


	// control
	CListBox m_ListBox;
	CButton m_Radio[3];
	CButton m_Check[3];
	CButton m_BitMap;

	// general
	map<CString, UNITDATA*>			m_mapUnitData;
	bool							m_bStart = false;

	afx_msg void OnDelete();
	afx_msg void OnSearch();
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();
};


// ����, ����, �ҷ����� ��ư ����� �����ؿ���

// 1. ���� : ����Ʈ �ڽ����� ������ ���� ������ ����
// 2. ���� : ���� ����Ʈ �ڽ��� ����� ������ �޸��� ���·� ����
// 3. �ҷ����� : ����� ������ �������� �����͸� �о�鿩�ͼ� ����Ʈ �ڽ��� �ٽ� ä������ ��.