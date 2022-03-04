// PathFind.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathFind.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "ObjectTool_ToolObject.h"

// CPathFind 대화 상자입니다.

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

// CPathFind 메시지 처리기입니다.

void CPathFind::OnLbnSelchangeList1()
{
	// 리스트 박스 선택시 이벤트

//	if (m_GameObject_Rect_Tool == nullptr)
	//	return;


	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : 현재 리스트박에서 선택된 목록의 인덱스 값을 반환
	int iSelect = m_ListBox.GetCurSel();

	// GetText 인덱스 값에 해당하는 문자열을 얻어와서 2인자에게 넣어준다.
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
	wstring SaveFileFullPath = GetSaveFilePath();
	if(SaveFileFullPath.length()<1)
	{
		FAILED_TOOL_MSG(L"NoFileName");
		return;
	}


	wofstream		fout;
	fout.open(SaveFileFullPath);

	// 파일 개방 성공
	if (!fout.fail())
	{
		for (auto& iter : m_PathInfoList)
		{
			// |문자는 구분 기호
			fout << iter->wstrObjKey << L"|" << iter->wstrStateKey <<L"|"<<
				iter->iCount<<L"|"<<iter->wstrPath <<  endl;
		}

		fout.close();		// close 함수는 생략 가능(객체 타입이어서 소멸 시점에 알아서 개방한 파일 또한 소멸 가능)
	}

	// 윈도우의 기본 프로그램을 실행시켜주는 함수
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

	// 해당 경로의 파일 탐색
	wifstream		fin;
	fin.open(LoadFileFullPath);

	if (!fin.fail())
	{
		// 오브젝트키 | 스테이트키 | 개수 | FULLPATH

		TCHAR	wObjKey[MAX_STR] = L"";
		TCHAR	wStateKey[MAX_STR] = L"";
		TCHAR	wCount[MAX_STR] = L"";
		TCHAR	wFullpath[MAX_PATH] = L"";
		wstring	wstrCombined = L"";

		// 초기화
		m_ListBox.ResetContent();

		while (true)
		{
			// '|' 단위로 끊어서 문자열 입력 처리
			fin.getline(wObjKey, MAX_STR, '|');
			fin.getline(wStateKey, MAX_STR, '|');
			fin.getline(wCount, MAX_STR, '|');
			fin.getline(wFullpath, MAX_PATH);

			if (fin.eof())
				break;

			// 패스 리스트 추가
			IMGPATH*		pImgPath = new IMGPATH;
			pImgPath->wstrObjKey = wObjKey;
			pImgPath->wstrStateKey = wStateKey;
			pImgPath->iCount = _tstoi(wCount);
			pImgPath->wstrPath = wFullpath;
			m_PathInfoList.push_back(pImgPath);

			// 패스 정보 추가
			wstrCombined = wstring(wObjKey) + L"|" + wstring(wStateKey) + L"|" + wstring(wCount) + L"|" + wstring(wFullpath);


		}

		fin.close();		// close 함수는 생략 가능(객체 타입이어서 소멸 시점에 알아서 개방한 파일 또한 소멸 가능)
	}
	
	Update_PathListData();

	UpdateData(FALSE);

	// 윈도우의 기본 프로그램을 실행시켜주는 함수
	CString pathstr = L"notepad.exe ";
	pathstr = pathstr + LoadFileFullPath.c_str();
	WinExec(CT2CA(pathstr.operator LPCWSTR()), SW_SHOW);
}

void CPathFind::OnDropFiles(HDROP hDropInfo)
{
	// 파일 드롭시 처리
	ClearPathData();
	
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
		CFileInfo::DirInfoExtraction(szFilePath, m_PathInfoList, m_ePathFileType);
	}

	Update_PathListData();

#pragma endregion

#pragma region 맵에 이미지파일 저장
	//int cnt = m_MyPathInfoList.size();


	//for (auto filename : m_MyPathInfoList)
	//{
	//	filename->wstrFullPath;

	//	CImage*		pPngImage = new  CImage;
	//	pPngImage->Load(filename->wstrFullPath.c_str()); // 해당 경로 이미지를 로드

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
	wstring SaveFileFullPath = g_FilePath_TexturePathData + wstrFilename + Extension;
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
	m_ComboBox_Filetype.SetCurSel(0);
	m_ePathFileType = (E_FILETYPE)m_ComboBox_Filetype.GetCurSel();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPathFind::OnCbnSelchangeCombo1()
{
	
	// change FileType
	m_ePathFileType = (E_FILETYPE)m_ComboBox_Filetype.GetCurSel();
}
