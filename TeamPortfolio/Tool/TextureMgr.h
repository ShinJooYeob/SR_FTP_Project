#pragma once

#include "SingleTexture.h"
#include "MultiTexture.h"
#include "Include.h"

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEXINFO*		Get_Texture(const TCHAR* pObjKey, const TCHAR* pStateKey = L"", const int& iCnt = 0);

public:
	HRESULT				InsertTexture(TEXTYPE eType, const TCHAR* pFilePath, const TCHAR* pObjKey, const TCHAR* pStateKey = L"", const int& iCnt = 0); 
	void				Release(void);

private:
	map<wstring, CTexture*>			m_mapTexture;

};

