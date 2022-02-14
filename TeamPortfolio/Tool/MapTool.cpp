// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"


// CMapTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
{

}

CMapTool::~CMapTool()
{
	for (auto& iter : m_MapPngImage)
	{
		iter.second->Destroy();
		Safe_Delete(iter.second);
	}
	m_MapPngImage.clear();

}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeList1)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnSaveData)
END_MESSAGE_MAP()


// CMapTool �޽��� ó�����Դϴ�.


void CMapTool::OnLbnSelchangeList1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : ���� ����Ʈ�ڿ��� ���õ� ����� �ε��� ���� ��ȯ
	int iSelect = m_ListBox.GetCurSel();

	// GetText �ε��� ���� �ش��ϴ� ���ڿ��� ���ͼ� 2���ڿ��� �־��ش�.
	m_ListBox.GetText(iSelect, strFindName);

	auto		iter = m_MapPngImage.find(strFindName);

	if (iter == m_MapPngImage.end())
		return;

	m_Picture.SetBitmap(*(iter->second));

	int i = 0;

	for (; i < strFindName.GetLength(); ++i)
	{
		//	isdigit : ���� ������ �Ѱܹ��� ���� ���ڰ� �������� �ƴϸ� �Ϲ� �������� �Ǻ��ϴ� �Լ�
		// ���ڶ�� �Ǹ�� ������ ��� 0�� �ƴ� ���� ��ȯ
		if (0 != isdigit(strFindName[i]))
			break;
	}

	// ù ������ ��ġ���� �� ��° ������ ��ġ(ī��Ʈ) ��ŭ ���ڸ� �����ϴ� �Լ�
	strFindName.Delete(0, i);

	// _tstoi : ���ڸ� ���������� �ٲ��ִ� �Լ�, ����� �̸��� �Լ��� �ſ� ������ �����ؼ� ã�ƺ� ��
	m_iDrawID = _tstoi(strFindName);

	UpdateData(FALSE);
}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	UpdateData(TRUE);
	CDialog::OnDropFiles(hDropInfo);


	TCHAR	szFilePath[MAX_PATH] = L"";

	// DragQueryFile : ��ӵ� ������ ������ ������ �Լ�
	// 0xffffffff(-1) : �� ��° ���ڰ��� -1�� �����ϸ� ��ӵ� ���� ������ ��ȯ�ϴ� �ɼ�

	int iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	TCHAR szFileName[64] = L"";

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString		strRelative = CFileInfo::ConvertRelativePath(szFilePath);

		// ��� �� ���� �̸��� �����ϴ� �Լ�
		CString		strFileName = PathFindFileName(strRelative);

		lstrcpy(szFileName, strFileName.GetString());
		// Ȯ���ڸ��� �߶󳻴� �Լ�
		PathRemoveExtension(szFileName);

		// Ű���� ��ġ�ϴ� ���ڿ��� �־��ֱ� ���� Ȯ���ڸ� ���� �߶� �����̸��� �ٽ� strFileName�� �������ְ� ����
		strFileName = szFileName;

		auto	iter = m_MapPngImage.find(strFileName);

		if (iter == m_MapPngImage.end())
		{
			CImage*		pPngImage = new  CImage;
			pPngImage->Load(strRelative); // �ش� ��� �̹����� �ε�

			m_MapPngImage.emplace(strFileName, pPngImage);
			m_ListBox.AddString(strFileName);
		}			
		
	}
	HorizontalScroll();


	UpdateData(FALSE);
}

void CMapTool::HorizontalScroll(void)
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


void CMapTool::OnSaveData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CFileDialog		Dlg(FALSE,
		L"dat",
		L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||",
		this);

	TCHAR	szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		// GetPathName : ���õ� ��θ� ��ȯ�ϴ� �Լ�
		CString				str = Dlg.GetPathName().GetString();
		const TCHAR*		pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;


		CMainFrame*	pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView*	pToolView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
		CTerrain*	pTerrain = pToolView->m_pTerrain;
		vector<TILE*>& vecTile = pTerrain->Get_Tile();
	
		DWORD	dwByte = 0;

		for (auto& iter : vecTile)
		{
			WriteFile(hFile, iter, sizeof(TILE), &dwByte, nullptr);

		}
		CloseHandle(hFile);
	}
}
