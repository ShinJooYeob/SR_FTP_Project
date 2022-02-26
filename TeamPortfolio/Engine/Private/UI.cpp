#include "..\Public\UI.h"
#include "GameInstance.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CUI::CUI(const CUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::Initialize_Clone(void * pArg)
{	
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;


	return S_OK;
}

_int CUI::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	return _int();
}

_int CUI::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;


	return _int();
}


_int CUI::Render()
{
	if (FAILED( __super::Render()))
		return E_FAIL;
	
	return S_OK;
}

_int CUI::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;




	return _int();
}


HRESULT CUI::Set_UI_Transform(CTransform * pComTransform, _float4 vUIDesc)
{
	if (pComTransform == nullptr)
		return E_FAIL;

	m_fX= vUIDesc.x;
	m_fY= vUIDesc.y;
	m_fSizeX= vUIDesc.z;
	m_fSizeY= vUIDesc.w;
	D3DVIEWPORT9		ViewPortDesc;
	m_pGraphicDevice->GetViewport(&ViewPortDesc);
	_int iWinCX = ViewPortDesc.Width;
	_int iWinCY = ViewPortDesc.Height;

	pComTransform->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	pComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(m_fX - iWinCX * 0.5f, -m_fY + iWinCY * 0.5f, 0.f));
	return S_OK;
}



void CUI::Free()
{
	__super::Free();


}
