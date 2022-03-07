#include "..\Public\Shader.h"

CShader::CShader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device), m_pEffect(nullptr)
{
	
}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
{
	Safe_AddRef(m_pEffect);
}

HRESULT CShader::Initialize_Prototype(const _tchar * pShaderFilePath)
{
	// 셰이더 파일 초기화
	FAILED_CHECK(D3DXCreateEffectFromFile(m_pGraphicDevice, pShaderFilePath, nullptr, nullptr, 0, nullptr, &m_pEffect, nullptr));

	return S_OK;
}

HRESULT CShader::Initialize_Clone(void * pArg)
{
	return S_OK;
}


HRESULT CShader::SetUp_ValueOnShader(D3DXHANDLE hParameter, void* pData, _uint iLength)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	return m_pEffect->SetRawValue(hParameter, pData, 0, iLength);	
}

HRESULT CShader::SetUp_TextureOnShader(D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9 pTexture)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	return m_pEffect->SetTexture(hParameter, pTexture);

	
}

HRESULT CShader::Begin_Shader(_uint iPassIndex)
{
	NULL_CHECK_BREAK(m_pEffect);

	_uint		iNumPasses = 0;
	// 최대 Pass개수 반환
	if (FAILED(m_pEffect->Begin(&iNumPasses, 0)))
		return E_FAIL;

	if (iPassIndex >= iNumPasses)
		return E_FAIL;

	return m_pEffect->BeginPass(iPassIndex);	
}

HRESULT CShader::End_Shader()
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	if (FAILED(m_pEffect->EndPass()))
		return E_FAIL;

	return m_pEffect->End();
}

CShader * CShader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pShaderFilePath)
{
	CShader*	pInstance = new CShader(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath)))
	{
		MSGBOX("Failed to Creating CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CShader::Clone(void * pArg)
{
	CShader*	pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Creating CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	__super::Free();

	Safe_Release(m_pEffect);
}
