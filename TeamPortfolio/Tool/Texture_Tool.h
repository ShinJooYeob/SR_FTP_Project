#pragma once

#include "Include.h"

class CTexture_Tool
{
public:
	CTexture_Tool();
	virtual ~CTexture_Tool();

public:
	virtual const TEXINFO*		Get_Texture(const TCHAR* pStateKey = L"", const int& iCnt = 0)PURE;
public:
	virtual HRESULT		InsertTexture(const TCHAR* pFilePath,  // ���� ���
		const TCHAR* pStateKey = L"", // ��Ƽ �ؽ�ó�� ��� ���, ��������Ʈ ���� Ű��
		const int& iCnt = 0)PURE;     // ��Ƽ �ؽ�ó�� ��� ���, �����ִ°�

	virtual void		Release(void)PURE;
};
