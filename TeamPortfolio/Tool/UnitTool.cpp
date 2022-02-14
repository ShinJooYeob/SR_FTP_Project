// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strTemp(_T(""))
	, m_strResult(_T(""))
	, m_strName(_T(""))
	, m_iAttack(0)
	, m_iDef(0)
	, m_strFindName(_T(""))
{

}

CUnitTool::~CUnitTool()
{
	for (auto& iter : m_mapUnitData)
		Safe_Delete(iter.second);

	m_mapUnitData.clear();

	
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strTemp);
	DDX_Text(pDX, IDC_EDIT2, m_strResult);
	DDX_Text(pDX, IDC_EDIT3, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_iAttack);
	DDX_Text(pDX, IDC_EDIT5, m_iDef);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);

	if (!m_bStart)
	{
		m_Radio[0].SetCheck(TRUE);
		m_bStart = true;
	}

	DDX_Control(pDX, IDC_CHECK1, m_Check[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Check[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Check[2]);
	DDX_Control(pDX, IDC_BUTTON3, m_BitMap);

	HBITMAP		hBitMap = (HBITMAP)LoadImage(nullptr, L"../Texture/JusinLogo1.bmp", IMAGE_BITMAP, 100, 50, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	m_BitMap.SetBitmap(hBitMap);

	DDX_Text(pDX, IDC_EDIT6, m_strFindName);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnAddCharacter)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON6, &CUnitTool::OnDelete)
	ON_EN_CHANGE(IDC_EDIT6, &CUnitTool::OnSearch)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnLoadData)
END_MESSAGE_MAP()


// CUnitTool �޽��� ó�����Դϴ�.


void CUnitTool::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


	// UpdateData(TRUE) : ���̾�α� �ڽ��κ��� �Էµ� ������ ����
	UpdateData(TRUE);

	m_strResult = m_strTemp;

	// UpdateData(FALSE) : ������ ����� ������ ���̾�α� �ڽ��� �ݿ�
	UpdateData(FALSE);
}


void CUnitTool::OnAddCharacter()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	UNITDATA*		pUnit = new UNITDATA;

	pUnit->strName = m_strName;
	pUnit->iAttack = m_iAttack;
	pUnit->iDef = m_iDef;

	for (int i = 0; i < 3; ++i)
	{
		if (m_Radio[i].GetCheck())
		{
			pUnit->byJobIndex = i;
			break;
		}
	}

	pUnit->byItem = 0x00;

	if (m_Check[0].GetCheck())
		pUnit->byItem |= RUBY;

	if (m_Check[1].GetCheck())
		pUnit->byItem |= DIAMOND;

	if (m_Check[2].GetCheck())
		pUnit->byItem |= SAPPHIRE;
		

	m_ListBox.AddString(pUnit->strName);

	m_mapUnitData.emplace(pUnit->strName, pUnit);

	UpdateData(FALSE);
}


void CUnitTool::OnListBox()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : ���� ����Ʈ�ڿ��� ���õ� ����� �ε��� ���� ��ȯ
	int iSelect = m_ListBox.GetCurSel();

	// GetText �ε��� ���� �ش��ϴ� ���ڿ��� ���ͼ� 2���ڿ��� �־��ش�.
	m_ListBox.GetText(iSelect, strFindName);

	auto		iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	m_strName = iter->second->strName;
	m_iAttack = iter->second->iAttack;
	m_iDef = iter->second->iDef;

	for (int i = 0; i < 3; ++i)
		m_Radio[i].SetCheck(FALSE);	 
	
	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);


	for (int i = 0; i < 3; ++i)
		m_Check[i].SetCheck(FALSE);

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);


	UpdateData(FALSE);
}


void CUnitTool::OnDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : ���� ����Ʈ�ڿ��� ���õ� ����� �ε��� ���� ��ȯ
	int iSelect = m_ListBox.GetCurSel();

	// LB_ERR(-1)
	if (LB_ERR == iSelect)
		return;

	m_ListBox.GetText(iSelect, strFindName);

	auto& iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	Safe_Delete(iter->second);
	m_mapUnitData.erase(strFindName);

	// �ش� ����� ����Ʈ ���ڿ��� �����ϴ� �Լ�
	m_ListBox.DeleteString(iSelect);

	UpdateData(FALSE);
}



void CUnitTool::OnSearch()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	auto& iter = m_mapUnitData.find(m_strFindName);

	if (iter == m_mapUnitData.end())
		return;

	// �ε��� 0������ Ž���Ͽ� ����Ʈ �ڽ����� �ִ� ��ġ�ϴ� ���� �׸� �ε��� ��ȯ
	int iIndex = m_ListBox.FindString(0, m_strFindName);

	if (LB_ERR == iIndex)
		return;

	// ���� �ε����� ���õ� ��ó�� ǥ��
	m_ListBox.SetCurSel(iIndex);


	m_strName = iter->second->strName;
	m_iAttack = iter->second->iAttack;
	m_iDef = iter->second->iDef;

	for (int i = 0; i < 3; ++i)
		m_Radio[i].SetCheck(FALSE);

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);


	for (int i = 0; i < 3; ++i)
		m_Check[i].SetCheck(FALSE);

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);


	UpdateData(FALSE);

}


void CUnitTool::OnSaveData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// CFileDialog : ���� ����, ���� �۾��� �ʿ��� ��ȭ���ڸ� �����ϴ� ��ü

	/*1. TRUE(����), FALSE(�ٸ��̸����� ����) ��� ����
	2. default ���� Ȯ���ڸ�
	3. ��ȭ ���ڿ� ���ʷ� ǥ�õ� ���ϸ�
	4. OFN_HIDEREADONLY(�б� ���� üũ�ڽ� ����) | OFN_OVERWRITEPROMPT(�ߺ� ���� ���� �� ��� �޼��� ���)
	5. ��ȭ ���ڿ� ǥ�õ� ���� ���� "�޺��ڽ��� ��µ� ���ڿ� | ���� ����� ���͸� ���ڿ� ||"
	6. �θ� ������ �ּ�*/

	CFileDialog		Dlg(FALSE,
					L"dat",
					L"*.dat", 
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					L"Data Files(*.dat)|*.dat||",
					this);

	TCHAR	szPath[MAX_PATH] = L"";

	// ���� ������Ʈ�ִ� ���丮 ��θ� ������ �Լ�
	GetCurrentDirectory(MAX_PATH, szPath);

	//	D:\����ȯ\124��\Frame124\Tool

	// ��ü ��ο��� ���� �̸��� �߶��ִ� �Լ�
	// ���� ��� �� ���ϸ��� ���ٸ� �� ������ �������� �߶�

	PathRemoveFileSpec(szPath);
	//	D:\����ȯ\124��\Frame124\
	
	lstrcat(szPath, L"\\Data");
	//	D:\����ȯ\124��\Frame124\Data

	// ��ȭ ���ڸ� ������ �� ���̴� �⺻ ��θ� ����
	Dlg.m_ofn.lpstrInitialDir = szPath;

	// DoModal : ��ȭ���ڸ� ����, IDOK �Ǵ� IDCANCEL�� ��ȯ
	if (IDOK == Dlg.DoModal())
	{
		// GetPathName : ���õ� ��θ� ��ȯ�ϴ� �Լ�
		CString				str = Dlg.GetPathName().GetString();
		const TCHAR*		pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte = 0;
		DWORD	dwStrByte = 0;

		for (auto& iter : m_mapUnitData)
		{
			dwStrByte = sizeof(TCHAR) * (iter.first.GetLength() + 1);

			// key �� ����
			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, iter.first.GetString(), dwStrByte, &dwByte, nullptr);

			// value �� ���� : unitdata ����ü ���ο� cstring Ŭ���� ����� ���� ������ ����ü ��°�� ����ϴ� ���� ����, ���� ��� ���� ���� ����
			WriteFile(hFile, &(iter.second->byItem), sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(iter.second->byJobIndex), sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(iter.second->iDef), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(iter.second->iAttack), sizeof(int), &dwByte, nullptr);

		}
		CloseHandle(hFile);
	}
}

/*
string	strName = "ȫ�浿";
char*	pName = strName;
char szName[128] = strName;

strcpy_s(szName, 128, strName.c_str());*/

void CUnitTool::OnLoadData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	CFileDialog		Dlg(TRUE,
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
		for (auto& iter : m_mapUnitData)
			Safe_Delete(iter.second);

		m_mapUnitData.clear();

		// ������ ����Ʈ ����� ��� �ʱ�ȭ
		m_ListBox.ResetContent();


		CString				str = Dlg.GetPathName().GetString();
		const TCHAR*		pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte = 0;
		DWORD	dwStrByte = 0;

		UNITDATA	tData{};

		while (true)
		{
			// key �� �ε�
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			TCHAR*		pName = new TCHAR[dwStrByte];

			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			// value �� �ε�
			ReadFile(hFile, &(tData.byItem), sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &(tData.byJobIndex), sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iDef), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iAttack), sizeof(int), &dwByte, nullptr);

			if (0 == dwByte)
			{
				delete[]pName;
				pName = nullptr;
				break;
			}

			UNITDATA*		pUnitData = new UNITDATA;
			pUnitData->strName = pName;
			delete[]pName;
			pName = nullptr;

			pUnitData->byItem = tData.byItem;
			pUnitData->byJobIndex = tData.byJobIndex;
			pUnitData->iDef = tData.iDef;
			pUnitData->iAttack = tData.iAttack;

			m_mapUnitData.emplace( pUnitData->strName, pUnitData);
			m_ListBox.AddString(pUnitData->strName);
		}
		CloseHandle(hFile);
	}


	UpdateData(FALSE);
}
