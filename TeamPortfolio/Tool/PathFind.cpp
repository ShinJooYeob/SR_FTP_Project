// PathFind.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathFind.h"
#include "afxdialogex.h"
#include "FileInfo.h"


// CPathFind ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathFind, CDialog)

CPathFind::CPathFind(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHFIND, pParent)
{

}

CPathFind::~CPathFind()
{
	for_each(m_PathInfoList.begin(), m_PathInfoList.end(), Safe_Delete<IMGPATH*>);
	m_PathInfoList.clear();
}

void CPathFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CPathFind, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPathFind::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathFind::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON7, &CPathFind::OnLoadData)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CPathFind �޽��� ó�����Դϴ�.


void CPathFind::OnListBox()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CPathFind::OnSaveData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	wofstream		fout;
	fout.open(L"../Data/ImgPath.txt");

	// ���� ���� ����
	if (!fout.fail())
	{
		for (auto& iter : m_PathInfoList)
		{
			fout << iter->wstrObjKey << L"|" << iter->wstrStateKey << L"|" << iter->iCount << L"|" << iter->wstrPath << endl;
		}

		fout.close();		// close �Լ��� ���� ����(��ü Ÿ���̾ �Ҹ� ������ �˾Ƽ� ������ ���� ���� �Ҹ� ����)
	}

	// �������� �⺻ ���α׷��� ��������ִ� �Լ�
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);

}


void CPathFind::OnLoadData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	wifstream		fin;
	fin.open(L"../Data/ImgPath.txt");

	if (!fin.fail())
	{
		TCHAR	szObjKey[MAX_STR] = L"";
		TCHAR	szStateKey[MAX_STR] = L"";
		TCHAR	szCount[MAX_STR] = L"";
		TCHAR	szPath[MAX_PATH] = L"";

		wstring	wstrCombined = L"";

		m_ListBox.ResetContent();

		while (true)
		{
			// '|' ������ ��� ���ڿ� �Է� ó��

			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			wstrCombined = wstring(szObjKey) + L"|" + szStateKey + L"|" + szCount + L"|" + szPath;
			m_ListBox.AddString(wstrCombined.c_str());
		}

		fin.close();		// close �Լ��� ���� ����(��ü Ÿ���̾ �Ҹ� ������ �˾Ƽ� ������ ���� ���� �Ҹ� ����)
	}

	UpdateData(FALSE);

	// �������� �⺻ ���α׷��� ��������ִ� �Լ�
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}


void CPathFind::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	UpdateData(TRUE);
	CDialog::OnDropFiles(hDropInfo);

	TCHAR	szFilePath[MAX_PATH] = L"";

	// DragQueryFile : ��ӵ� ������ ������ ������ �Լ�
	// 0xffffffff(-1) : �� ��° ���ڰ��� -1�� �����ϸ� ��ӵ� ���� ������ ��ȯ�ϴ� �ɼ�
	int iFileCnt = DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFileName[64] = L"";

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		CFileInfo::DirInfoExtraction(szFilePath, m_PathInfoList);
	}
	m_ListBox.ResetContent();

	wstring	wstrCombined = L"";
	TCHAR	szBuf[MIN_STR] = L"";

	// /, \, |, \\, || : ������(token)

	for (auto& iter : m_PathInfoList)
	{
		// ������ wstr�� ��ȯ
		// ���� 10���� ���ڿ��� ��ȯ�ϰڴٴ� �ǹ�
		_itow_s(iter->iCount, szBuf, 10);

		wstrCombined = iter->wstrObjKey + L"|" + iter->wstrStateKey + L"|" + szBuf + L"|" + iter->wstrPath;
		m_ListBox.AddString(wstrCombined.c_str());
	}

	UpdateData(FALSE);

}
