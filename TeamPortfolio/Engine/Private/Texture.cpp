#include "..\Public\Texture.h"



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
	
	// 첫번째 놈으로 binded 바인드 시켜놔주자
	auto iter =  m_mapTextureLayers.begin();

	m_pBindedTextureLayer = iter->second;
	Safe_AddRef(m_pBindedTextureLayer);
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
	// 툴에서 텍스처 날리는데 사용

	for (auto& pTextureLayer : m_mapTextureLayers)
		Safe_Release(pTextureLayer.second);

	Safe_Release(m_pBindedTextureLayer);

	m_mapTextureLayers.clear();

	return S_OK;
}

HRESULT CTexture::Read_TextFile(TYPE eTextureType, const _tchar * szFilePath)
{
	wifstream		fin;
	fin.open(szFilePath);

	if (!fin.fail())
	{
		_tchar	szPath[MAX_PATH] = L"";
		_tchar	szStateKey[MAX_PATH] = L"";
		_tchar	szCount[MAX_PATH] = L"";

		while (true)
		{
			// '|' 단위로 끊어서 문자열 입력 처리

			fin.getline(szPath, MAX_PATH,'|');
			fin.getline(szStateKey, MAX_PATH, '|');
			fin.getline(szCount, MAX_PATH, '|');

			if (fin.eof())
				break;

			// 문자열을 정수로 변환 함수
			_int iCount = _ttoi(szCount);

			if (FAILED(Insert_TextureLayer(eTextureType, szPath, szStateKey, iCount)))
			{
				MSGBOX("Fail to Insert Texture Layer");
				return E_FAIL;
			}

		}

		fin.close();		// close 함수는 생략 가능(객체 타입이어서 소멸 시점에 알아서 개방한 파일 또한 소멸 가능)
	}


	auto iter = m_mapTextureLayers.begin();

	m_pBindedTextureLayer = iter->second;
	Safe_AddRef(m_pBindedTextureLayer);

	return S_OK;
}

HRESULT CTexture::Insert_TextureLayer(TYPE eType, _tchar * szFilePath, _tchar * szStateKey, _int iNumTextureCount)
{

	auto iter = find_if(m_mapTextureLayers.begin(), m_mapTextureLayers.end(), CTagFinder(szStateKey));

	if (iter != m_mapTextureLayers.end())
		return E_FAIL;


	CTextureLayer::TEXTURELAYERDESC tagLayerDesc = {};

	tagLayerDesc.eTextureType = eType;
	tagLayerDesc.szFilePath = szFilePath;
	tagLayerDesc.iNumTexture = iNumTextureCount;



	CTextureLayer* pTextureLayer = CTextureLayer::Create(m_pGraphicDevice, &tagLayerDesc);

	if (pTextureLayer == nullptr)
		return E_FAIL;

	///////////////////이거 수정해야함////////////////////////////////////////////////////
	m_mapTextureLayers.emplace(szStateKey, pTextureLayer);
	////////////////////////////////////////////////////////////////////


	return S_OK;
}

HRESULT CTexture::Change_TextureLayer(const _tchar * tagTexureLayer)
{
	Safe_Release(m_pBindedTextureLayer);

	auto iter = find_if(m_mapTextureLayers.begin(), m_mapTextureLayers.end(), CTagFinder(tagTexureLayer));
	
	if (iter == m_mapTextureLayers.end())
		return E_FAIL;

	m_pBindedTextureLayer = iter->second;
	Safe_AddRef(m_pBindedTextureLayer);


	return S_OK;
}

HRESULT CTexture::Bind_Texture(_uint iTextureIndex)
{
	if (m_pBindedTextureLayer == nullptr)
		return E_FAIL;

	return m_pBindedTextureLayer->Bind_Texture(iTextureIndex);
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

	for (auto& pTextureLayer : m_mapTextureLayers)
		Safe_Release(pTextureLayer.second);

	Safe_Release(m_pBindedTextureLayer);

	m_mapTextureLayers.clear();

}
