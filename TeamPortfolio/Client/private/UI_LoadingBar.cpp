#include "stdafx.h"
#include "..\public\UI_LoadingBar.h"


CUI_LoadingBar::CUI_LoadingBar(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CUI_LoadingBar::CUI_LoadingBar(const CUI_LoadingBar & rhs)
	: CUI(rhs)
{
}


HRESULT CUI_LoadingBar::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_LoadingBar::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;

	vRect = { 230,655,230,690 };
	Set_UI_Transform(m_ComTransform, Set_byRectPos(vRect));

	m_fDepth = -6666;
	//850
	//m_vUIDesc = _float4(g_iWinCX >> 1, g_iWinCY >> 1, g_iWinCX, g_iWinCY);
	//m_vUIDesc = _float4(WinPosX, WinPosY, WinCX, WinCY);

	//x = Ltop


	m_ComTexture->Change_TextureLayer(TEXT("LoadingBar"));

	//m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(280.f, 675.f, 0.f));

	return S_OK;
}

_int CUI_LoadingBar::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	m_fFrame = fDeltaTime;


	return _int();
}

_int CUI_LoadingBar::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;

	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;
	return _int();
}

_int CUI_LoadingBar::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	//if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(m_fFrame)))
	//	return E_FAIL;
	if (FAILED(m_ComTexture->Bind_Texture()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CUI_LoadingBar::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_float4 CUI_LoadingBar::Set_byRectPos(_float4 tRect)
{
	_float4 Result = {};
	Result.z = tRect.z - tRect.x;
	Result.w = tRect.w - tRect.y;
	Result.x = tRect.x + Result.z * 0.5f;
	Result.y = tRect.y + Result.w * 0.5f;
	

	return Result;
}

void CUI_LoadingBar::Set_Progress(_float _ProgressCount, _float _MaxCount)
{
	vRect.z = 820.f * _ProgressCount / (_float)_MaxCount + 230;



	m_ComTransform->Set_MatrixState(CTransform::STATE_RIGHT, _float3(1, 0, 0));
	m_ComTransform->Set_MatrixState(CTransform::STATE_UP, _float3(0, 1, 0));

	Set_UI_Transform(m_ComTransform, Set_byRectPos(vRect));


}

HRESULT CUI_LoadingBar::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	//m_eNowSceneNum = SCENEID::SCENE_LOADING;

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Loading_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_LoadingBar::SetUp_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	return S_OK;
}

HRESULT CUI_LoadingBar::Release_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

CUI_LoadingBar * CUI_LoadingBar::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CUI_LoadingBar* pInstance = new CUI_LoadingBar(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CUI_LoadingBar_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CUI_LoadingBar::Clone(void * pArg)
{
	CUI_LoadingBar* pInstance = new CUI_LoadingBar((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CUI_LoadingBar_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CUI_LoadingBar::Free()
{
	__super::Free();


	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
