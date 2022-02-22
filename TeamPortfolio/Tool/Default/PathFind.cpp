// PathFind.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathFind.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "ObjectTool_ToolObject.h"

// CPathFind ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathFind, CDialog)

CPathFind::CPathFind(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHFIND, pParent)
{
}

CPathFind::~CPathFind()
{

//	Safe_Release(m_GameObject_Rect_Tool);

	ClearPathData();

}



void CPathFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_EDIT2, mEditBox);
	//	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBox_Filetype);
}

BEGIN_MESSAGE_MAP(CPathFind, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPathFind::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathFind::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON7, &CPathFind::OnLoadData)
	ON_WM_DROPFILES()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPathFind::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()

// CPathFind �޽��� ó�����Դϴ�.

void CPathFind::OnLbnSelchangeList1()
{
	// ����Ʈ �ڽ� ���ý� �̺�Ʈ

//	if (m_GameObject_Rect_Tool == nullptr)
	//	return;


	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : ���� ����Ʈ�ڿ��� ���õ� ����� �ε��� ���� ��ȯ
	int iSelect = m_ListBox.GetCurSel();

	// GetText �ε��� ���� �ش��ϴ� ���ڿ��� ���ͼ� 2���ڿ��� �־��ش�.
	m_ListBox.GetText(iSelect, strFindName);
	wstring wStrFilename = strFindName.GetString();
	wstring fullpath = FindPath(wStrFilename);

	//TCHAR DD[20];
	//const _tchar* constDD = DD;
	//{
	//	TCHAR DD1[20] = L"aa";
	//	lstrcpy(DD, DD1);
	//}


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
	wstring SaveFileFullPath = GetSaveFilePath();
	if(SaveFileFullPath.length()<1)
	{
		FAILED_TOOL_MSG(L"NoFileName");
		return;
	}


	wofstream		fout;
	fout.open(SaveFileFullPath);

	// ���� ���� ����
	if (!fout.fail())
	{
		for (auto& iter : m_PathInfoList)
		{
			// |���ڴ� ���� ��ȣ
			fout << iter->wstrObjKey << L"|" << iter->wstrStateKey <<L"|"<<
				iter->iCount<<L"|"<<iter->wstrPath <<  endl;
		}

		fout.close();		// close �Լ��� ���� ����(��ü Ÿ���̾ �Ҹ� ������ �˾Ƽ� ������ ���� ���� �Ҹ� ����)
	}

	// �������� �⺻ ���α׷��� ��������ִ� �Լ�
	CString pathstr = L"notepad.exe ";
	pathstr = pathstr + SaveFileFullPath.c_str();

	WinExec(CT2CA(pathstr.operator LPCWSTR()),SW_SHOW);
}

void CPathFind::OnLoadData()
{
	ClearPathData();

	wstring LoadFileFullPath = GetSaveFilePath();
	if (LoadFileFullPath.length() < 1)
	{
		FAILED_TOOL_MSG(L"NoFileName");
		return;
	}

	UpdateData(TRUE);

	// �ش� ����� ���� Ž��
	wifstream		fin;
	fin.open(LoadFileFullPath);

	if (!fin.fail())
	{
		// ������ƮŰ | ������ƮŰ | ���� | FULLPATH

		TCHAR	wObjKey[MAX_STR] = L"";
		TCHAR	wStateKey[MAX_STR] = L"";
		TCHAR	wCount[MAX_STR] = L"";
		TCHAR	wFullpath[MAX_PATH] = L"";
		wstring	wstrCombined = L"";

		// �ʱ�ȭ
		m_ListBox.ResetContent();

		while (true)
		{
			// '|' ������ ��� ���ڿ� �Է� ó��
			fin.getline(wObjKey, MAX_STR, '|');
			fin.getline(wStateKey, MAX_STR, '|');
			fin.getline(wCount, MAX_STR, '|');
			fin.getline(wFullpath, MAX_PATH);

			if (fin.eof())
				break;

			// �н� ����Ʈ �߰�
			IMGPATH*		pImgPath = new IMGPATH;
			pImgPath->wstrObjKey = wObjKey;
			pImgPath->wstrStateKey = wStateKey;
			pImgPath->iCount = _tstoi(wCount);
			pImgPath->wstrPath = wFullpath;
			m_PathInfoList.push_back(pImgPath);

			// �н� ���� �߰�
			wstrCombined = wstring(wObjKey) + L"|" + wstring(wStateKey) + L"|" + wstring(wCount) + L"|" + wstring(wFullpath);


		}

		fin.close();		// close �Լ��� ���� ����(��ü Ÿ���̾ �Ҹ� ������ �˾Ƽ� ������ ���� ���� �Ҹ� ����)
	}
	
	Update_PathListData();

	UpdateData(FALSE);

	// �������� �⺻ ���α׷��� ��������ִ� �Լ�
	CString pathstr = L"notepad.exe ";
	pathstr = pathstr + LoadFileFullPath.c_str();
	WinExec(CT2CA(pathstr.operator LPCWSTR()), SW_SHOW);
}

void CPathFind::OnDropFiles(HDROP hDropInfo)
{
	// ���� ��ӽ� ó��
	ClearPathData();
	
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
		CFileInfo::DirInfoExtraction(szFilePath, m_PathInfoList, m_ePathFileType);
	}

	Update_PathListData();

#pragma endregion

#pragma region �ʿ� �̹������� ����
	//int cnt = m_MyPathInfoList.size();

	//// #Bug CImage �޸𸮸�
	//for (auto filename : m_MyPathInfoList)
	//{
	//	filename->wstrFullPath;

	//	CImage*		pPngImage = new  CImage;
	//	pPngImage->Load(filename->wstrFullPath.c_str()); // �ش� ��� �̹����� �ε�

	//	m_MapPngImage.emplace(filename->wFileName, pPngImage);
	//}

#pragma endregion
	HorizontalScroll();


	UpdateData(FALSE);
}

wstring CPathFind::FindPath(wstring strname)
{
	for (auto pathname : m_PathInfoList)
	{
		/*if (pathname->wFileName == strname)
			return pathname->wstrFullPath;*/
	}
	return L"";
}

wstring CPathFind::GetSaveFilePath()
{
	CString cstrFIleName = {};
	GetDlgItemText(mEditBox.GetDlgCtrlID(), cstrFIleName);
	if (cstrFIleName.GetLength() < 1)
		return L"";
	wstring wstrFilename;
	wstrFilename = cstrFIleName.operator LPCWSTR();
	wstring SaveFileFullPath = FilePath + wstrFilename + Extension;
	return SaveFileFullPath;
}

HRESULT CPathFind::ClearPathData()
{
	if (!m_PathInfoList.empty())
	{
		for_each(m_PathInfoList.begin(), m_PathInfoList.end(), Safe_Delete<IMGPATH*>);
		m_PathInfoList.clear();
	}

	/*if (m_MyPathInfoList.empty() == false)
	{
		for_each(m_MyPathInfoList.begin(), m_MyPathInfoList.end(), Safe_Delete<MYFILEPATH*>);
		m_MyPathInfoList.clear();
	}

	for (auto& iter : m_MapPngImage)
	{
		iter.second->Destroy();
		Safe_Delete(iter.second);
	}
	m_MapPngImage.clear();*/

	return S_OK;
}

HRESULT CPathFind::Update_PathListData()
{
	if (m_PathInfoList.empty())
		return E_FAIL;
	m_ListBox.ResetContent();
	for (auto& iter : m_PathInfoList)
	{
		TCHAR buf[10];
		_itot_s(iter->iCount, buf, 10);
		wstring combinestring = iter->wstrObjKey + L"|" + iter->wstrStateKey + L"|" + buf + L"|" + iter->wstrPath;
		m_ListBox.AddString(combinestring.c_str());
	}
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
	m_ComboBox_Filetype.SetCurSel(0);
	m_ePathFileType = (E_FILETYPE)m_ComboBox_Filetype.GetCurSel();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CPathFind::OnCbnSelchangeCombo1()
{
	
	// change FileType
	m_ePathFileType = (E_FILETYPE)m_ComboBox_Filetype.GetCurSel();
}
