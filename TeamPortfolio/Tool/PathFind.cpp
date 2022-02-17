// PathFind.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathFind.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "ObjectTool_Rect.h"

// CPathFind 대화 상자입니다.

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

// CPathFind 메시지 처리기입니다.

void CPathFind::OnLbnSelchangeList1()
{
	if (m_GameObject_Rect_Tool == nullptr)
		return;

	// 텍스처 파일 선택시 보여지게 하기

	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : 현재 리스트박에서 선택된 목록의 인덱스 값을 반환
	int iSelect = m_ListBox.GetCurSel();

	// GetText 인덱스 값에 해당하는 문자열을 얻어와서 2인자에게 넣어준다.
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
	//	//	isdigit : 인자 값으로 넘겨받은 단일 문자가 숫자인지 아니면 일반 글자인지 판별하는 함수
	//	// 숫자라고 판명된 글자인 경우 0이 아닌 값을 반환
	//	if (0 != isdigit(strFindName[i]))
	//		break;
	//}

	//// 첫 인자의 위치부터 두 번째 인자의 위치(카운트) 만큼 문자를 삭제하는 함수
	//strFindName.Delete(0, i);

	//// _tstoi : 문자를 정수형으로 바꿔주는 함수, 비슷한 이름의 함수가 매우 많으니 유의해서 찾아볼 것
	//m_iDrawID = _tstoi(strFindName);

	UpdateData(FALSE);


}

void CPathFind::OnSaveData()
{
	// 저장버튼을 누르면 해당 경로 txt 파일로 저장된다.
	wstring strImgPath = L"../Data/ImgPath.txt";
	wofstream		fout;
	fout.open(strImgPath);

	// 파일 개방 성공
	if (!fout.fail())
	{
		for (auto& iter : m_MyPathInfoList)
		{
			// |문자는 구분 기호
			fout << iter->wFileName << L"|" << iter->wstrFullPath << endl;
		}

		fout.close();		// close 함수는 생략 가능(객체 타입이어서 소멸 시점에 알아서 개방한 파일 또한 소멸 가능)
	}

	// 윈도우의 기본 프로그램을 실행시켜주는 함수
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}

void CPathFind::OnLoadData()
{
	ClearData();

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	wstring strImgPath = L"../Data/ImgPath.txt";

	UpdateData(TRUE);

	// 해당 경로의 파일 탐색
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
			// '|' 단위로 끊어서 문자열 입력 처리

			fin.getline(wFolderName1, MAX_STR, '|');
			fin.getline(wFullpath, MAX_PATH);

			if (fin.eof())
				break;

			// 패스 리스트 추가
			MYFILEPATH*		pImgPath = new MYFILEPATH;
			TCHAR			szPath[MAX_PATH] = L"";
			pImgPath->wstrFullPath = wFullpath;
			pImgPath->wFileName = wFolderName1;

			m_MyPathInfoList.push_back(pImgPath);



			// 리스트 박스추가
		//	wstrCombined = wstring(wFolderName1) + L"|" + wFullpath;
			m_ListBox.AddString(wFolderName1);
		}

		fin.close();		// close 함수는 생략 가능(객체 타입이어서 소멸 시점에 알아서 개방한 파일 또한 소멸 가능)
	}

	int cnt = m_MyPathInfoList.size();
	for (auto filename : m_MyPathInfoList)
	{
		filename->wstrFullPath;

		CImage*		pPngImage = new  CImage;
		pPngImage->Load(filename->wstrFullPath.c_str()); // 해당 경로 이미지를 로드

		m_MapPngImage.emplace(filename->wFileName, pPngImage);
	}



	UpdateData(FALSE);

	// 윈도우의 기본 프로그램을 실행시켜주는 함수
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}

void CPathFind::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ClearData();
#pragma region 경로저장


	// 폴더 드롭다운시 텍스쳐 이미지를 탐색한다.
	// 세부적으로 파일 탐색 재설정해야될듯

	// 드롭다운
	UpdateData(TRUE);
	CDialog::OnDropFiles(hDropInfo);

	// 전체 경로
	TCHAR	szFilePath[MAX_PATH] = L"";

	// DragQueryFile : 드롭된 파일을 정보를 얻어오는 함수
	// 0xffffffff(-1) : 두 번째 인자값을 -1로 지정하면 드롭된 파일 개수를 반환하는 옵션
	int iFileCnt = DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFileName[64] = L"";

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		// 이거 수정
		CFileInfo::DirInfoExtraction_Custom(szFilePath, m_MyPathInfoList, FILETYPE_PNG);
	}
	m_ListBox.ResetContent();

	//	wstring	wstrCombined = L"";
	//	TCHAR	szBuf[MIN_STR] = L"";

		// /, \, |, \\, || : 구분자(token)

		//for (auto& iter : m_MyPathInfoList)
		//{
		//	// 정수를 wstr로 변환
		//	// 현재 10진수 문자열로 변환하겠다는 의미
		//	_itow_s(iter->iCount, szBuf, 10);

		//	wstrCombined = iter->wstrObjKey + L"|" + iter->wstrStateKey + L"|" + szBuf + L"|" + iter->wstrPath;
		//	m_ListBox.AddString(wstrCombined.c_str());
		//}

	for (auto& iter : m_MyPathInfoList)
	{
		m_ListBox.AddString(iter->wFileName.c_str());
	}
#pragma endregion


#pragma region 맵에 이미지파일 저장
	int cnt = m_MyPathInfoList.size();

	// #Bug CImage 메모리릭
	for (auto filename : m_MyPathInfoList)
	{
		filename->wstrFullPath;

		CImage*		pPngImage = new  CImage;
		pPngImage->Load(filename->wstrFullPath.c_str()); // 해당 경로 이미지를 로드

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

	// 리스트 박스의 DC를 얻어옴
	CDC*		pDC = m_ListBox.GetDC();

	// 현재 리스트 박스 목록의 개수를 반환
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		// 현재 문자열의 길이를 픽셀단위로 변환
		size = pDC->GetTextExtent(strName);

		if (size.cx > iCX)
			iCX = size.cx;
	}

	m_ListBox.ReleaseDC(pDC);

	// GetHorizontalExtent :리스트 박스 가로로 스크롤 할 수 있는 최대 범위를 얻어오는 함수
	if (iCX > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iCX);


}



BOOL CPathFind::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::DestroyWindow();
}


BOOL CPathFind::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_GameObject_Rect_Tool = nullptr;
	m_GameObject_Rect_Tool = GetSingle(CSuperToolSIngleton)->GetObjectRect();
	m_GameObject_Rect_Tool->AddRef();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
