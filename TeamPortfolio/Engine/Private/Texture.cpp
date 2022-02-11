#include "..\Public\Texture.h"




CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CComponent(pGraphicDevice)
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs),
	m_vecTexture(rhs.m_vecTexture)
{
	for (auto& pTexture : m_vecTexture)
		Safe_AddRef(pTexture);

}



HRESULT CTexture::Initialize_Protoype(void * pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	TEXTUREDESC TextureDesc{};
	memcpy(&TextureDesc, pArg, sizeof(TEXTUREDESC));

	_tchar	szFullPath[MAX_PATH] = L"";

	for (_uint i = 0; i< TextureDesc.m_iNumTexture;i++)
	{
		wsprintf(szFullPath, TextureDesc.szFilePath, i);

		LPDIRECT3DBASETEXTURE9		pTexture = nullptr;
		HRESULT		hr = 0;
		
		if(TextureDesc.eTextureType == TYPE_DEFAULT)
			hr = D3DXCreateTextureFromFile(m_pGraphicDevice, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
		else
			hr = D3DXCreateCubeTextureFromFile(m_pGraphicDevice, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);

		if (FAILED(hr))
			return E_FAIL;


		m_vecTexture.emplace_back(pTexture);
	}


	return S_OK;
}

HRESULT CTexture::Initialize_Clone(void * pArg)
{
	return S_OK;
}


HRESULT CTexture::Bind_Texture(_uint iTextureIndex)
{
	if (iTextureIndex >= m_vecTexture.size())
		return E_FAIL;

	return m_pGraphicDevice->SetTexture(0, m_vecTexture[iTextureIndex]);

}


CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDevice, TEXTUREDESC* pArg)
{
	CTexture*	pInstance = new CTexture(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Protoype(pArg)))
	{
		MSGBOX("Failed to Creating CTexture");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CTexture::Clone(void * pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg))) {
		MSGBOX("Failed to Creating CTexture");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pTexture : m_vecTexture)
		Safe_Release(pTexture);


	m_vecTexture.clear();

}
