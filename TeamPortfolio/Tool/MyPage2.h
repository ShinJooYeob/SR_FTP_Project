#pragma once


// CMyPage2 ��ȭ �����Դϴ�.

class CMyPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CMyPage2)

public:
	CMyPage2();
	virtual ~CMyPage2();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYPAGE2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
