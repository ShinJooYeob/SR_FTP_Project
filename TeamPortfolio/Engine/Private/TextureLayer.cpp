#include "..\Public\TextureLayer.h"
#include "Shader.h"



CTextureLayer::CTextureLayer(LPDIRECT3DDEVICE9 pGraphicDevice)
	:m_pGraphicDevice(pGraphicDevice)
{
	Safe_AddRef(m_pGraphicDevice);
}


HRESULT CTextureLayer::Initialize_Prototype(void * pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	TEXTURELAYERDESC tTextureLayerDesc{};
	memcpy(&tTextureLayerDesc, pArg, sizeof(TEXTURELAYERDESC));

	m_vecTexture.reserve(tTextureLayerDesc.iNumTexture + 1);

	for (_uint i = 0; i < tTextureLayerDesc.iNumTexture; i++)
	{
		_tchar	szFullPath[MAX_PATH] = L"";
		wsprintf(szFullPath, tTextureLayerDesc.szFilePath, i);

		LPDIRECT3DBASETEXTURE9		pTexture = nullptr;
		HRESULT		hr = 0;

		if (tTextureLayerDesc.eTextureType == 0)
  			hr = D3DXCreateTextureFromFile(m_pGraphicDevice, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
		else
			hr = D3DXCreateCubeTextureFromFile(m_pGraphicDevice, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);

		FAILED_CHECK(hr);


		m_vecTexture.emplace_back(pTexture);
	}


	return S_OK;
}



HRESULT CTextureLayer::Bind_Texture(_uint iTextureIndex,_uint iStageIndex)
{
	if (iTextureIndex >= m_vecTexture.size())
		return E_FAIL;

	return m_pGraphicDevice->SetTexture(iStageIndex, m_vecTexture[iTextureIndex]);
}

HRESULT CTextureLayer::Bind_ShaderTexture(CShader * pShader, D3DXHANDLE param, _uint iTextureIndex)
{

	if (iTextureIndex >= m_vecTexture.size())
		return E_FAIL;
	return pShader->SetUp_TextureOnShader(param, m_vecTexture[iTextureIndex]);
}

HRESULT CTextureLayer::ClearTexture()
{
	for (auto& pTexture : m_vecTexture)
		Safe_Release(pTexture);

	m_vecTexture.clear();

	return S_OK;
}

HRESULT CTextureLayer::Add_Another_Texture(TEXTURELAYERDESC * pArg)
{


	if (pArg == nullptr)
		return E_FAIL;

	TEXTURELAYERDESC tTextureLayerDesc{};
	memcpy(&tTextureLayerDesc, pArg, sizeof(TEXTURELAYERDESC));


	for (_uint i = 0; i < tTextureLayerDesc.iNumTexture; i++)
	{
		_tchar	szFullPath[MAX_PATH] = L"";
		wsprintf(szFullPath, tTextureLayerDesc.szFilePath, i);

		LPDIRECT3DBASETEXTURE9		pTexture = nullptr;
		HRESULT		hr = 0;

		if (tTextureLayerDesc.eTextureType == 0)
			hr = D3DXCreateTextureFromFile(m_pGraphicDevice, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
		else
			hr = D3DXCreateCubeTextureFromFile(m_pGraphicDevice, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);

		if (FAILED(hr))
			return E_FAIL;


		m_vecTexture.emplace_back(pTexture);
	}





	return S_OK;
}



CTextureLayer * CTextureLayer::Create(LPDIRECT3DDEVICE9 pGraphicDevice, TEXTURELAYERDESC * pArg)
{
	CTextureLayer*	pInstance = new CTextureLayer(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Creating TextureLayer Prototype");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTextureLayer::Free()
{
	for (auto& pTexture : m_vecTexture)
		Safe_Release(pTexture);

	m_vecTexture.clear();

	Safe_Release(m_pGraphicDevice);
}
