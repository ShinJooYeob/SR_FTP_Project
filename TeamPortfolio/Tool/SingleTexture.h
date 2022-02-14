#pragma once
#include "Texture.h"
class CSingleTexture :	public CTexture
{
public:
	CSingleTexture();
	virtual ~CSingleTexture();

public:
	virtual const TEXINFO*		Get_Texture(const TCHAR* pStateKey = L"", const int& iCnt = 0) { return m_pTexInfo; }
	virtual HRESULT InsertTexture(const TCHAR* pFilePath, const TCHAR* pStateKey = L"", const int& iCnt = 0) override;
	virtual void Release(void) override;

private:
	TEXINFO*			m_pTexInfo;

};

