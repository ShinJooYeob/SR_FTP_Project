// PathFind.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathFind.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "ObjectTool_Rect.h"

// CPathFind ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathFind, CDialog)

CPathFind::CPathFind(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHFIND, pParent)
{
}

CPathFind::~CPathFind()
{

	Safe_Release(m_GameObject_Rect_Tool);

	for_each(m_MyPathInfoList.begin(), m_MyPathInfoList.end(), Safe_Delete<MYFILEPATH*>);
	m_MyPathInfoList.clear();

	for (auto& iter : m_MapPngImage)
	{
		iter.second->Destroy();
		Safe_Delete(iter.second);
	}
	m_MapPngImage.clear();

}



void CPathFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}

BEGIN_MESSAGE_MAP(CPathFind, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPathFind::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathFind::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON7, &CPathFind::OnLoadData)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

// CPathFind �޽��� ó�����Դϴ�.

void CPathFind::OnLbnSelchangeList1()
{
	if (m_GameObject_Rect_Tool == nullptr)
		return;

	// �ؽ�ó ���� ���ý� �������� �ϱ�

	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : ���� ����Ʈ�ڿ��� ���õ� ����� �ε��� ���� ��ȯ
	int iSelect = m_ListBox.GetCurSel();

	// GetText �ε��� ���� �ش��ϴ� ���ڿ��� ���ͼ� 2���ڿ��� �־��ش�.
	m_ListBox.GetText(iSelect, strFindName);
	wstring wStrFilename = strFindName.GetString();
	wstring fullpath = FindPath(wStrFilename);


	auto		iter = m_MapPngImage.find(wStrFilename);

	if (iter == m_MapPngImage.end())
		return;

	m_Picture.SetBitmap(*(iter->second));

	MYFILEPATH path;
	path.wFileName = wStrFilename;
	path.wstrFullPath = fullpath;
	m_GameObject_Rect_Tool->Set_Texture(path);

	//int i = 0;

	//for (; i < strFindName.GetLength(); ++i)
	//{
	//	//	isdigit : ���� ������ �Ѱܹ��� ���� ���ڰ� �������� �ƴϸ� �Ϲ� �������� �Ǻ��ϴ� �Լ�
	//	// ���ڶ�� �Ǹ�� ������ ��� 0�� �ƴ� ���� ��ȯ
	//	if (0 != isdigit(strFindName[i]))
	//		break;
	//}

	//// ù ������ ��ġ���� �� ��° ������ ��ġ(ī��Ʈ) ��ŭ ���ڸ� �����ϴ� �Լ�
	//strFindName.Delete(0, i);

	//// _tstoi : ���ڸ� ���������� �ٲ��ִ� �Լ�, ����� �̸��� �Լ��� �ſ� ������ �����ؼ� ã�ƺ� ��
	//m_iDrawID = _tstoi(strFindName);

	UpdateData(FALSE);


}

void CPathFind::OnSaveData()
{
	// �����ư�� ������ �ش� ��� txt ���Ϸ� ����ȴ�.
	wstring strImgPath = L"../Data/ImgPath.txt";
	wofstream		fout;
	fout.open(strImgPath);

	// ���� ���� ����
	if (!fout.fail())
	{
		for (auto& iter : m_MyPathInfoList)
		{
			// |���ڴ� ���� ��ȣ
			fout << iter->wFileName << L"|" << iter->wstrFullPath << endl;
		}

		fout.close();		// close �Լ��� ���� ����(��ü Ÿ���̾ �Ҹ� ������ �˾Ƽ� ������ ���� ���� �Ҹ� ����)
	}

	// �������� �⺻ ���α׷��� ��������ִ� �Լ�
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}

void CPathFind::OnLoadData()
{
	ClearData();

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	wstring strImgPath = L"../Data/ImgPath.txt";

	UpdateData(TRUE);

	// �ش� ����� ���� Ž��
	wifstream		fin;
	fin.open(strImgPath);

	if (!fin.fail())
	{
		TCHAR	wFolderName1[MAX_STR] = L"";
		//TCHAR	wFolderName2[MAX_STR] = L"";
		//TCHAR	wFolderName3[MAX_STR] = L"";
		//TCHAR	wFolderName4[MAX_PATH] = L"";
		//TCHAR	wFolderName5[MAX_PATH] = L"";
		TCHAR	wFullpath[MAX_PATH] = L"";

		wstring	wstrCombined = L"";

		m_ListBox.ResetContent();

		while (true)
		{
			// '|' ������ ��� ���ڿ� �Է� ó��

			fin.getline(wFolderName1, MAX_STR, '|');
			fin.getline(wFullpath, MAX_PATH);

			if (fin.eof())
				break;

			// �н� ����Ʈ �߰�
			MYFILEPATH*		pImgPath = new MYFILEPATH;
			TCHAR			szPath[MAX_PATH] = L"";
			pImgPath->wstrFullPath = wFullpath;
			pImgPath->wFileName = wFolderName1;

			m_MyPathInfoList.push_back(pImgPath);



			// ����Ʈ �ڽ��߰�
		//	wstrCombined = wstring(wFolderName1) + L"|" + wFullpath;
			m_ListBox.AddString(wFolderName1);
		}

		fin.close();		// close �Լ��� ���� ����(��ü Ÿ���̾ �Ҹ� ������ �˾Ƽ� ������ ���� ���� �Ҹ� ����)
	}

	int cnt = m_MyPathInfoList.size();
	for (auto filename : m_MyPathInfoList)
	{
		filename->wstrFullPath;

		CImage*		pPngImage = new  CImage;
		pPngImage->Load(filename->wstrFullPath.c_str()); // �ش� ��� �̹����� �ε�

		m_MapPngImage.emplace(filename->wFileName, pPngImage);
	}



	UpdateData(FALSE);

	// �������� �⺻ ���α׷��� ��������ִ� �Լ�
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}

void CPathFind::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	ClearData();
#pragma region �������


	// ���� ��Ӵٿ�� �ؽ��� �̹����� Ž���Ѵ�.
	// ���������� ���� Ž�� �缳���ؾߵɵ�

	// ��Ӵٿ�
	UpdateData(TRUE);
	CDialog::OnDropFiles(hDropInfo);

	// ��ü ���
	TCHAR	szFilePath[MAX_PATH] = L"";

	// DragQueryFile : ��ӵ� ������ ������ ������ �Լ�
	// 0xffffffff(-1) : �� ��° ���ڰ��� -1�� �����ϸ� ��ӵ� ���� ������ ��ȯ�ϴ� �ɼ�
	int iFileCnt = DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFileName[64] = L"";

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		// �̰� ����
		CFileInfo::DirInfoExtraction_Custom(szFilePath, m_MyPathInfoList, FILETYPE_PNG);
	}
	m_ListBox.ResetContent();

	//	wstring	wstrCombined = L"";
	//	TCHAR	szBuf[MIN_STR] = L"";

		// /, \, |, \\, || : ������(token)

		//for (auto& iter : m_MyPathInfoList)
		//{
		//	// ������ wstr�� ��ȯ
		//	// ���� 10���� ���ڿ��� ��ȯ�ϰڴٴ� �ǹ�
		//	_itow_s(iter->iCount, szBuf, 10);

		//	wstrCombined = iter->wstrObjKey + L"|" + iter->wstrStateKey + L"|" + szBuf + L"|" + iter->wstrPath;
		//	m_ListBox.AddString(wstrCombined.c_str());
		//}

	for (auto& iter : m_MyPathInfoList)
	{
		m_ListBox.AddString(iter->wFileName.c_str());
	}
#pragma endregion


#pragma region �ʿ� �̹������� ����
	int cnt = m_MyPathInfoList.size();

	// #Bug CImage �޸𸮸�
	for (auto filename : m_MyPathInfoList)
	{
		filename->wstrFullPath;

		CImage*		pPngImage = new  CImage;
		pPngImage->Load(filename->wstrFullPath.c_str()); // �ش� ��� �̹����� �ε�

		m_MapPngImage.emplace(filename->wFileName, pPngImage);
	}

#pragma endregion
	HorizontalScroll();


	UpdateData(FALSE);
}

wstring CPathFind::FindPath(wstring strname)
{
	for (auto pathname : m_MyPathInfoList)
	{
		if (pathname->wFileName == strname)
			return pathname->wstrFullPath;
	}
	return L"";
}

HRESULT CPathFind::ClearData()
{
	if (m_MyPathInfoList.empty() == false)
	{
		for_each(m_MyPathInfoList.begin(), m_MyPathInfoList.end(), Safe_Delete<MYFILEPATH*>);
		m_MyPathInfoList.clear();
	}

	for (auto& iter : m_MapPngImage)
	{
		iter.second->Destroy();
		Safe_Delete(iter.second);
	}
	m_MapPngImage.clear();

	return S_OK;
}

void CPathFind::HorizontalScroll(void)
{
	CString		strName;
	CSize		size;

	int			iCX = 0;

	// ����Ʈ �ڽ��� DC�� ����
	CDC*		pDC = m_ListBox.GetDC();

	// ���� ����Ʈ �ڽ� ����� ������ ��ȯ
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		// ���� ���ڿ��� ���̸� �ȼ������� ��ȯ
		size = pDC->GetTextExtent(strName);

		if (size.cx > iCX)
			iCX = size.cx;
	}

	m_ListBox.ReleaseDC(pDC);

	// GetHorizontalExtent :����Ʈ �ڽ� ���η� ��ũ�� �� �� �ִ� �ִ� ������ ������ �Լ�
	if (iCX > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iCX);


}



BOOL CPathFind::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CDialog::DestroyWindow();
}


BOOL CPathFind::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_GameObject_Rect_Tool = nullptr;
	m_GameObject_Rect_Tool = GetSingle(CSuperToolSIngleton)->GetObjectRect();
	m_GameObject_Rect_Tool->AddRef();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
