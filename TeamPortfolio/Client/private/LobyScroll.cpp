#include "stdafx.h"
#include "..\public\LobyScroll.h"





CLobyScroll::CLobyScroll(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CLobyScroll::CLobyScroll(const CLobyScroll & rhs)
	: CUI(rhs)
{
}

HRESULT CLobyScroll::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLobyScroll::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_vUIDesc_Frame.w = 180;
	m_vUIDesc_Frame.z = g_iWinCX;
	m_vUIDesc_Frame.y = g_iWinCY - m_vUIDesc_Frame.w * 0.5f - 20;
	m_vUIDesc_Frame.x = g_iWinCX * 0.5f;

	m_vUIDesc_IMG = m_vUIDesc_Frame;

	m_fPassedTime = 0;
	m_fStartPoint = m_vUIDesc_Frame.x;
	m_fTargetPoint = m_fStartPoint - 256;

	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc_Frame)))
		return E_FAIL;


	return S_OK;
}
void CLobyScroll::Set_UI_TransformRect( _float4 vRect)
{
	//vRect.x,y,z,w=top,left,bottom,right
	_float4 vResult{};
	/*≥ ∫Ò*/vResult.z = vRect.w-vRect.y;
	/*≥Ù¿Ã*/vResult.w = vRect.z-vRect.x;
	/*x¡¬«•*/vResult.x = vRect.y + vResult.z*0.5f;
	/*y¡¬«•*/vResult.y = vRect.x + vResult.w*0.5f;
	Set_UI_Transform(m_ComTransform, vResult);
	
}

_int CLobyScroll::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	
	//m_vUIDesc_IMG.x -= fDeltaTime * 300;
	//if(m_vUIDesc_IMG.x < -g_iWinCX*0.5f + 256)
	//	m_vUIDesc_IMG.x = g_iWinCX * 0.5f;

	m_fPassedTime += fDeltaTime;
	if (m_fPassedTime < 1.5f)
		m_vUIDesc_IMG.x = GetSingle(CGameInstance)->Easing(TYPE_BounceOut, m_fStartPoint, m_fTargetPoint, m_fPassedTime, 1.5f);
	if (m_fPassedTime > 3.5f)
	{
		m_fPassedTime = 0;
		m_iCount += 1;

		if (m_iCount >= 4)
		{
			m_iCount = 0;
			m_fStartPoint = m_vUIDesc_Frame.x;
			m_fTargetPoint = m_fStartPoint - 256;
		}
		else {
			m_fStartPoint = m_fTargetPoint;
			m_fTargetPoint = m_fStartPoint - 256;

		}
	}





	return _int();

}





_int CLobyScroll::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;





	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return _int();
}

_int CLobyScroll::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;



	FAILED_CHECK(First_SetUp_RenderState());

	FAILED_CHECK(Second_SetUp_RenderState());

	FAILED_CHECK(Third_SetUp_RenderState());



	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return _int();
}

_int CLobyScroll::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

HRESULT CLobyScroll::First_SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;
	if (FAILED(m_ComTexture->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc_Frame)))
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESS);
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_ComTransform->Set_MatrixState(CTransform::STATE_POS, m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 0, 0.00001f));


	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CLobyScroll::Second_SetUp_RenderState()
{
	if (FAILED(m_ComTexture->Bind_Texture(1)))
		return E_FAIL;

	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc_IMG)))
		return E_FAIL;

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 0, 0.00001f));

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CLobyScroll::Third_SetUp_RenderState()
{
	if (FAILED(m_ComTexture->Bind_Texture(2)))
		return E_FAIL;

	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc_Frame)))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CLobyScroll::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	return S_OK;
}

HRESULT CLobyScroll::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));


	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_LobyScroll"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;


	return S_OK;
}





CLobyScroll * CLobyScroll::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CLobyScroll* pInstance = new CLobyScroll(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CShop_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CLobyScroll::Clone(void * pArg)
{
	CLobyScroll* pInstance = new CLobyScroll((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CShop_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CLobyScroll::Free()
{
	__super::Free();


	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
