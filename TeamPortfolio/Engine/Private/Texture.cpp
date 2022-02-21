#include "..\Public\Texture.h"
#include "TextureLayer.h"



CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CComponent(pGraphicDevice)
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs),
	m_mapTextureLayers(rhs.m_mapTextureLayers)
{

	for (auto& Pair : m_mapTextureLayers) 
		Safe_AddRef(Pair.second);
	
	// ù��° ������ binded ���ε� ���ѳ�����
	auto iter =  m_mapTextureLayers.begin();

	m_pBindedTextureLayer = iter->second;
	m_TagNowTexture = iter->first.c_str();
	Safe_AddRef(m_pBindedTextureLayer);

	m_fFrameTime = 0;
	m_iNumMaxTexture = m_pBindedTextureLayer->Get_TextureNum();

}




HRESULT CTexture::Initialize_Prototype(void * pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	TEXTUREDESC tTextureDesc{};
	memcpy(&tTextureDesc, pArg, sizeof(TEXTUREDESC));
	
	if (FAILED(Read_TextFile(tTextureDesc.eTextureType,tTextureDesc.szTextFilePath)))
		return E_FAIL;
	
	return S_OK;
}


HRESULT CTexture::Initialize_Clone(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::ClearTexture()
{
	// ������ �ؽ�ó �����µ� ���

	for (auto& Pair : m_mapTextureLayers)
		Safe_Release(Pair.second);
	Safe_Release(m_pBindedTextureLayer);
	m_mapTextureLayers.clear();
	return S_OK;
}

HRESULT CTexture::Read_TextFile(TYPE eTextureType, const _tchar * szFilePath)
{
	wifstream		fin;

	_tchar FullFilePath[MAX_PATH]= L"../Bin/Resources/Textures/TexturePathData/";

	lstrcat(FullFilePath, szFilePath);

	fin.open(FullFilePath);

	if (!fin.fail())
	{
		_tchar	szPadding[10] = L"";
		_tchar	szPath[MAX_PATH] = L"";
		_tchar	szStateKey[MAX_PATH] = L"";
		_tchar	szCount[MAX_PATH] = L"";

		while (true)
		{

			// '|' ������ ��� ���ڿ� �Է� ó��

			fin.getline(szPadding, MAX_PATH, '|');
			fin.getline(szStateKey, MAX_PATH, '|');
			fin.getline(szCount, MAX_PATH, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			// ���ڿ��� ������ ��ȯ �Լ�
			_int iCount = _ttoi(szCount);

			if (FAILED(Insert_TextureLayer(eTextureType, szPath, szStateKey, iCount)))
			{
				MSGBOX("Fail to Insert Texture Layer");
				return E_FAIL;
			}

			if (fin.eof())
				break;
		}

		fin.close();		// close �Լ��� ���� ����(��ü Ÿ���̾ �Ҹ� ������ �˾Ƽ� ������ ���� ���� �Ҹ� ����)
	}


	auto iter = m_mapTextureLayers.begin();

	m_pBindedTextureLayer = iter->second;
	m_TagNowTexture = iter->first.c_str();
	Safe_AddRef(m_pBindedTextureLayer);

	m_fFrameTime = 0;
	m_iNumMaxTexture = m_pBindedTextureLayer->Get_TextureNum();

	return S_OK;
}

HRESULT CTexture::Insert_TextureLayer(TYPE eType, _tchar * szFilePath, _tchar * szStateKey, _int iNumTextureCount)
{



	CTextureLayer::TEXTURELAYERDESC tagLayerDesc = {};

	tagLayerDesc.eTextureType = eType;
	tagLayerDesc.szFilePath = szFilePath;
	tagLayerDesc.iNumTexture = iNumTextureCount;

	auto iter = find_if(m_mapTextureLayers.begin(), m_mapTextureLayers.end(), CTagStringFinder(szStateKey));

	if (iter != m_mapTextureLayers.end()) {

		if (FAILED((iter)->second->Add_Another_Texture(&tagLayerDesc)))
			return E_FAIL;
	}
	else 
	{

		CTextureLayer* pTextureLayer = CTextureLayer::Create(m_pGraphicDevice, &tagLayerDesc);

		if (pTextureLayer == nullptr)
			return E_FAIL;


		m_mapTextureLayers.emplace(szStateKey, pTextureLayer);

	}




	return S_OK;
}

HRESULT CTexture::Change_TextureLayer(const _tchar * tagTexureLayer, _float fFramePerSec)
{
	if (!lstrcmp(m_TagNowTexture, tagTexureLayer))
		return S_FALSE;

	auto iter = find_if(m_mapTextureLayers.begin(), m_mapTextureLayers.end(), CTagStringFinder(tagTexureLayer));
	
	if (iter == m_mapTextureLayers.end())
		return E_FAIL;


	Safe_Release(m_pBindedTextureLayer);
	m_pBindedTextureLayer = iter->second;
	Safe_AddRef(m_pBindedTextureLayer);

	m_fFrameTime = 0;
	m_fFramePerSec = fFramePerSec;
	m_TagNowTexture = tagTexureLayer;
	m_iNumMaxTexture = m_pBindedTextureLayer->Get_TextureNum();

	m_bIsWaitTexture = false;
	m_bIsReturnTexture = false;
	m_szReturnTag = nullptr;
	return S_OK;
}

HRESULT CTexture::Change_TextureLayer_ReturnTo(const _tchar * tagTexureLayer, const _tchar * szReturnTag, _float fFramePerSec, _float fResturnFps)
{
	if (tagTexureLayer == nullptr || szReturnTag == nullptr)
		return E_FAIL;


	if (lstrcmp(m_TagNowTexture, tagTexureLayer)) 
	{
		auto iter = find_if(m_mapTextureLayers.begin(), m_mapTextureLayers.end(), CTagStringFinder(tagTexureLayer));

		if (iter == m_mapTextureLayers.end())
			return E_FAIL;


		Safe_Release(m_pBindedTextureLayer);
		m_pBindedTextureLayer = iter->second;
		Safe_AddRef(m_pBindedTextureLayer);

		m_fFrameTime = 0;
		m_iNumMaxTexture = m_pBindedTextureLayer->Get_TextureNum();
		m_TagNowTexture = tagTexureLayer;
	}

	m_bIsWaitTexture = false;
	m_bIsReturnTexture = true;
	m_fFramePerSec = fFramePerSec;
	m_iReturnFps = fResturnFps;
	m_szReturnTag = szReturnTag;
	return S_OK;
}

HRESULT CTexture::Change_TextureLayer_Wait(const _tchar * tagTexureLayer, _float fFramePerSec)
{

	if (!lstrcmp(m_TagNowTexture, tagTexureLayer))
		return S_FALSE;


	auto iter = find_if(m_mapTextureLayers.begin(), m_mapTextureLayers.end(), CTagStringFinder(tagTexureLayer));

	if (iter == m_mapTextureLayers.end())
		return E_FAIL;


	Safe_Release(m_pBindedTextureLayer);
	m_pBindedTextureLayer = iter->second;
	Safe_AddRef(m_pBindedTextureLayer);

	m_fFrameTime = 0;
	m_fFramePerSec = fFramePerSec;
	m_TagNowTexture = tagTexureLayer;
	m_iNumMaxTexture = m_pBindedTextureLayer->Get_TextureNum();

	m_bIsWaitTexture = true;
	m_bIsReturnTexture = false;
	m_szReturnTag = nullptr;

	return S_OK;
}

HRESULT CTexture::Bind_Texture_AutoFrame(_float fTimeDelta)
{
	if (m_pBindedTextureLayer == nullptr)
		return E_FAIL;

	m_fFrameTime += m_fFramePerSec * fTimeDelta;

	if (m_fFrameTime >= (_float)(m_iNumMaxTexture + 1))
	{
		if (m_bIsWaitTexture)
		{
			m_fFrameTime = (_float)m_iNumMaxTexture;
			m_TagNowTexture = nullptr;
		}
		else if (m_bIsReturnTexture)
		{
			Change_TextureLayer(m_szReturnTag, m_iReturnFps);
		}
		else
		{
			m_fFrameTime = 0.f;
		}
	}


	return m_pBindedTextureLayer->Bind_Texture((_uint)m_fFrameTime);

}

HRESULT CTexture::Bind_Texture(_uint iTextureIndex)
{
	if (m_pBindedTextureLayer == nullptr)
		return E_FAIL;

	return m_pBindedTextureLayer->Bind_Texture(iTextureIndex);
}

_uint CTexture::CurrentTextureLayerSize()
{
	return m_pBindedTextureLayer->Get_TextureNum();
}


CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDevice, TEXTUREDESC* pArg)
{
	CTexture*	pInstance = new CTexture(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Creating CTexture Prototype");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CTexture::Clone(void * pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg))) 
	{
		MSGBOX("Failed to Creating CTexture Clone");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& Pair : m_mapTextureLayers)
		Safe_Release(Pair.second);

	

	Safe_Release(m_pBindedTextureLayer);

	m_mapTextureLayers.clear();

}
