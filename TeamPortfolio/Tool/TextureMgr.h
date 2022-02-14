#pragma once

#include "SingleTexture.h"
#include "MultiTexture.h"

#include "Base.h"

class CTextureMgr final: public CBase
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	explicit CTextureMgr();
	virtual ~CTextureMgr() = default;

public:
	const TEXINFO*		Get_Texture(const TCHAR* pObjKey, const TCHAR* pStateKey = L"", const int& iCnt = 0);

public:
	HRESULT				InsertTexture(TEXTYPE eType, const TCHAR* pFilePath, const TCHAR* pObjKey, const TCHAR* pStateKey = L"", const int& iCnt = 0); 

private:
	map<wstring, CTexture_Tool*>			m_mapTexture;


	// CBase을(를) 통해 상속됨
	virtual void Free() override;

};

