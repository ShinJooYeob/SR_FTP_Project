#pragma once

#include "Texture_Tool.h"

class CMultiTexture : public CTexture_Tool
{
public:
	CMultiTexture();
	virtual ~CMultiTexture();

public:
	virtual const TEXINFO* Get_Texture(const TCHAR* pStateKey = L"", const int& iCnt = 0) override;
	virtual HRESULT InsertTexture(const TCHAR* pFilePath, const TCHAR* pStateKey = L"", const int& iCnt = 0) override;
	virtual void Release(void) override;

private:
	map<wstring, vector<TEXINFO*>>			m_mapMultiTex;

};

