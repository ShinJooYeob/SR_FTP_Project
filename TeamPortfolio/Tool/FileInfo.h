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
	static void				DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathInfoList);
	static	int				DirFileCount(const wstring& wstrPath);

};

