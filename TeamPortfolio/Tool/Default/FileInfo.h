#pragma once

#include "Include.h"

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString			ConvertRelativePath(CString strFullPath);

	// ������ ��� ������ �����ϴ� �Լ�
	static void				DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathInfoList, E_FILETYPE type = FILETYPE_PNG);
	// ������ ��� ������ �����ϴ� �Լ�2
	static void				DirInfoExtraction_Custom(const wstring& wstrPath, list<wstring>& rFullPathList, E_FILETYPE type);

	static	int				DirFileCount(const wstring& wstrPath);
private:
	static	bool			FindType(CFileFind& find, E_FILETYPE type);
};
