#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	// ��� ��θ� ������ �迭
	TCHAR		szRelativePath[MAX_PATH] = L"";

	// ���� ���� ��θ� �����ϰ� �ִ� �迭
	TCHAR		szCurDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirPath);


	// ��� ��η� ������ִ� �Լ�
	// ��, �� ��ΰ� �ݵ�� ���� ����̺꿡 �־�� ��.
	PathRelativePathTo(szRelativePath, 
		szCurDirPath,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(), 
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<IMGPATH*>& rPathInfoList)
{
	wstring	wstrFilePath = wstrPath + L"\\*.*";

	// mfc���� �����ϴ� ���� �� ��� ���� ���� Ŭ����
	CFileFind		Find;

	// �־��� ��ο� ������ ������ Ȯ���ϴ� �Լ�
	// �������� ������ false ����, �����ϸ� true����
	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if(Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			// GetFilePath : ���� ã�� ��θ� ������ �Լ�
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathInfoList);
		}

		else // ������ ã�� ��Ȳ
		{
			// ã���� �ý��� ������ ��� �ǳʶڴ�.
			if (Find.IsSystem())
				continue;

			IMGPATH*		pImgPath = new IMGPATH;
			TCHAR			szPath[MAX_PATH] = L"";

			lstrcpy(szPath, Find.GetFilePath().GetString());

			// ���� ��� ���¿��� ���ϸ�� Ȯ���� ���� �߶�
			PathRemoveFileSpec(szPath);

			// ������ ������ ���ؼ� ����ü ��� ������ ������ ä����
			pImgPath->iCount = DirFileCount(szPath);

			// GetFileTitle : ���ϸ� ������ �Լ�(Ȯ���ڸ� ����)
			// L"AKIHA_AKI01_000"
			wstring	wstrTextureName = Find.GetFileTitle().GetString();

			// substr(����, ��) : ���ۿ��� ���� �ش��ϴ� ���ڿ��� ������ �Լ�
			// L"AKIHA_AKI01_00%d.png"
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";

			TCHAR	szBuf[MAX_STR] = L"";
			lstrcpy(szBuf, Find.GetFilePath().GetString());
			// D:\����ȯ\124��\Frame124\Texture\Stage\Player\Attack\AKIHA_AKI01_000.png

			PathRemoveFileSpec(szBuf);
			// D:\����ȯ\124��\Frame124\Texture\Stage\Player\Attack\

			// �� ���ڿ��� �����Ͽ� �ϼ��� ��θ� ù ��° ���ڰ��� ����
			PathCombine(szBuf, szBuf, wstrTextureName.c_str());
			// D:\����ȯ\124��\Frame124\Texture\Stage\Player\Attack\AKIHA_AKI01_00%d.png

			pImgPath->wstrPath = ConvertRelativePath(szBuf);
			// ..\Texture\Stage\Player\Attack\AKIHA_AKI01_00%d.png

			PathRemoveFileSpec(szBuf);
			// ..\Texture\Stage\Player\Attack\

			// PathFindFileName : ���ϸ��� ã�ų� ���� ������ ��θ� ã�Ƴ��� �Լ�
			pImgPath->wstrStateKey = PathFindFileName(szBuf);

			PathRemoveFileSpec(szBuf);
			// ..\Texture\Stage\Player\

			// PathFindFileName : ���ϸ��� ã�ų� ���� ������ ��θ� ã�Ƴ��� �Լ�
			pImgPath->wstrObjKey = PathFindFileName(szBuf);		

			rPathInfoList.push_back(pImgPath);

			Find.FindFile(Find.GetFilePath());
			Find.FindNextFile();
		}
		
	}

}

int CFileInfo::DirFileCount(const wstring & wstrPath)
{
	wstring	wstrFilePath = wstrPath + L"\\*.*";

	// mfc���� �����ϴ� ���� �� ��� ���� ���� Ŭ����
	CFileFind		Find;

	// �־��� ��ο� ������ ������ Ȯ���ϴ� �Լ�
	// �������� ������ false ����, �����ϸ� true����
	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	int	iFileCnt = 0;

	while (bContinue)
	{
		// ������ ������ Ȯ���ϰ� ���� �� ���Ͽ� �����ϰ� ���� ���
		// FindNextFile�Լ��� ������ ȣ���ؾ� �Ѵ�. *.* ���� ��� �ش� ��� ���� ��� ������ ã�� ������ �ؼ�
		// FindNextFile ���� ù ��° ���Ͽ� �����ϰ� �ȴ�.

		// FindNextFile : ���� ��� �ȿ��� ���� ������ ã�� �Լ�
		// ���̻� ã���� ���ų� ������ ����� ��� 0�� ����
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;
		
		// ã���� �ý��� ������ ��� �ǳʶڴ�.
		if(Find.IsSystem())
			continue;

		++iFileCnt;
	}

	return iFileCnt;
}
