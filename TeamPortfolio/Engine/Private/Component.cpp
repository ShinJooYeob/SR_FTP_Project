#include "..\Public\Component.h"



CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDevice)
	:m_pGraphicDevice(pGraphicDevice), m_bIsClone(false)
{
	m_pGraphicDevice->AddRef();
}

CComponent::CComponent(const CComponent & rhs)
	:m_pGraphicDevice(rhs.m_pGraphicDevice), m_bIsClone(true)
{
	m_pGraphicDevice->AddRef();
}

HRESULT CComponent::Initialize_Protoype(void * pArg)
{
	return S_OK;
}

HRESULT CComponent::Initialize_Clone(void * pArg)
{
	return S_OK;
}

void CComponent::Free()
{
	Safe_Release(m_pGraphicDevice);

}
