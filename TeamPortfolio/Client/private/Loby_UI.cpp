#include "stdafx.h"
#include "..\public\Loby_UI.h"





CLoby_UI::CLoby_UI(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CLoby_UI::CLoby_UI(const CLoby_UI & rhs)
	: CUI(rhs)
{
}

HRESULT CLoby_UI::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLoby_UI::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_vUIDesc.x = g_iWinCX *0.5f;
	m_vUIDesc.y = 180;
	m_vUIDesc.z = 1000;
	m_vUIDesc.w = 300;

	m_fAlphaValue = 0;
	m_fTextFrame = 0;
	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
		return E_FAIL;


	return S_OK;
}
void CLoby_UI::Set_UI_TransformRect( _float4 vRect)
{
	//vRect.x,y,z,w=top,left,bottom,right
	_float4 vResult{};
	/*너비*/vResult.z = vRect.w-vRect.y;
	/*높이*/vResult.w = vRect.z-vRect.x;
	/*x좌표*/vResult.x = vRect.y + vResult.z*0.5f;
	/*y좌표*/vResult.y = vRect.x + vResult.w*0.5f;
	Set_UI_Transform(m_ComTransform, vResult);
	
}

_int CLoby_UI::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;

	if (m_fAlphaValue < 255) {

		m_fAlphaValue += fDeltaTime * 85.f;
		if (m_fAlphaValue > 255)
			m_fAlphaValue = 255;
	}
	m_fTextFrame += fDeltaTime * 5;

	return _int();

}

HRESULT CLoby_UI::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//Sour => 현재 그리려고하는 그림의 색
	//Dest => 직전까지 화면에 그려진 색
	//
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(_uint(m_fAlphaValue), 255, 255, 255));
	//
	//
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);


	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CLoby_UI::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	return S_OK;
}


_int CLoby_UI::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;


	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return _int();
}

_int CLoby_UI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	if (m_fTextFrame - 15 > 0)
		GetSingle(CGameInstance)->Render_UI_Font(L"Shin Joo Yeob\n\nJung Jin Woo\n\nUm Ji Hwan\n\nPark Eun Hyuk", { g_iWinCX - 280,g_iWinCY - 270 }, { 20,30 }, { 83,250,120 }, (_int)(m_fTextFrame - 15));

	return _int();
}

_int CLoby_UI::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

HRESULT CLoby_UI::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));


	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_LobyUI"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	

	return S_OK;
}





CLoby_UI * CLoby_UI::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CLoby_UI* pInstance = new CLoby_UI(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create LobyUI_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CLoby_UI::Clone(void * pArg)
{
	CLoby_UI* pInstance = new CLoby_UI((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create LobyUI_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CLoby_UI::Free()
{
	__super::Free();


	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
