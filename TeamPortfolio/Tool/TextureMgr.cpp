#include "stdafx.h"
#include "TextureMgr.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO * CTextureMgr::Get_Texture(const TCHAR* pObjKey, const TCHAR * pStateKey, const int & iCnt)
{
	auto	iter = find_if(m_mapTexture.begin(),
		m_mapTexture.end(),
		[&](auto& MyPair)->bool
	{
		if (pObjKey == MyPair.first)
			return true;

		return false;
	});

	if (iter == m_mapTexture.end())
		return nullptr;


	return iter->second->Get_Texture(pStateKey, iCnt);
}

HRESULT CTextureMgr::InsertTexture(TEXTYPE eType, const TCHAR * pFilePath, const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCnt)
{
	auto	iter = find_if(m_mapTexture.begin(),
						   m_mapTexture.end(),
							[&](auto& MyPair)->bool
						{
							if (pObjKey == MyPair.first)
								return true;

							return false;
						});

	if (iter == m_mapTexture.end())
	{
		CTexture*		pTexture = nullptr;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->InsertTexture(pFilePath, pStateKey, iCnt)))
		{
			MSG_BOX(pFilePath);
			return E_FAIL;
		}

		m_mapTexture.emplace(pObjKey, pTexture);
	}
	else if (TEX_MULTI == eType)
		iter->second->InsertTexture(pFilePath, pStateKey, iCnt);

	return S_OK;
}

void CTextureMgr::Release(void)
{
	for_each(m_mapTexture.begin(), m_mapTexture.end(), 
		[](auto& MyPair)
	{
		Safe_Delete(MyPair.second);
	});

	m_mapTexture.clear();

}
