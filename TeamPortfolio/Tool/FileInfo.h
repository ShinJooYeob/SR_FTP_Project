#pragma once

#include "Include.h"

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString			ConvertRelativePath(CString strFullPath);

	// 폴더의 경로 정보를 추출하는 함수
	static void				DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathInfoList);
	static	int				DirFileCount(const wstring& wstrPath);

};

