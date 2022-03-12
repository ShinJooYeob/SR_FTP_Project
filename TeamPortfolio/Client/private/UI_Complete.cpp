#include "stdafx.h"
#include "..\public\UI_Complete.h"




CUI_Complete::CUI_Complete(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CUI_Complete::CUI_Complete(const CUI_Complete & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Complete::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Complete::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	ZeroMemory(m_vUIDesc, sizeof(_float4) );

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	FAILED_CHECK(SetUp_UIDesc());

	return S_OK;
}


_int CUI_Complete::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;




	FAILED_CHECK(Update_Animation(fDeltaTime));

	return _int();

}





_int CUI_Complete::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;



	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return _int();
}

_int CUI_Complete::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;



	
		FAILED_CHECK(First_SetUp_RenderState());



	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return _int();
}

_int CUI_Complete::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

HRESULT CUI_Complete::First_SetUp_RenderState()
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
	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 50);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
		return E_FAIL;
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;
	FAILED_CHECK(m_ComTexture->Change_TextureLayer(L"Clear"));
	if (FAILED(m_ComTexture->Bind_Texture()))
		return E_FAIL;
	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;


	return S_OK;
}


HRESULT CUI_Complete::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	return S_OK;
}

HRESULT CUI_Complete::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));


	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_UI"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Complete::SetUp_UIDesc()
{
	//깔판
	m_vUIDesc.x = g_iWinCX >> 1;
	m_vUIDesc.y = (g_iWinCY >> 1);
	m_vUIDesc.z = 0.1;
	m_vUIDesc.w = 200;
	


	return S_OK;
}

HRESULT CUI_Complete::Update_Animation(_float fTimeDelta)
{
	if(m_bStartTime==true)
		m_fCountFrame += fTimeDelta;
	for (int i = 0; i < QUEST_END; ++i)
	{
		if (GetSingle(CQuest)->Get_QuestNeedPercent(i) == 100)
			if (GetSingle(CQuest)->Get_QuestComplete(i) == 0)
			{
				GetSingle(CGameInstance)->PlaySoundW(L"JW_success.wav", CHANNELID::CHANNEL_UI);
				m_bStarton = true;
				GetSingle(CQuest)->Set_QuestCompleteIndex(i,1);
			}
	}
	if (m_bStarton)
	{
		m_fStartPoint = 0;
		m_fTargetPoint = g_iWinCX >> 1;
		m_fDepth = -2;
		m_bStartTime = true;
		m_iAlpha = 255;
		m_fPassedTime += fTimeDelta;
		m_vUIDesc.z = GetSingle(CGameInstance)->Easing(TYPE_CubicOut, m_fStartPoint, m_fTargetPoint, m_fPassedTime, 2.f);
		if (m_vUIDesc.z >= m_fTargetPoint)
			m_bStarton = false;
	}
	
	else if(m_bStarton==false&&m_fCountFrame>2.5)
	{
		m_bStartTime = false;
		m_fCountFrame = 0;
		m_iAlpha = 0;
		m_fPassedTime = 0;
	}


	return S_OK;
}







CUI_Complete * CUI_Complete::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CUI_Complete* pInstance = new CUI_Complete(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CUI_Complete_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CUI_Complete::Clone(void * pArg)
{
	CUI_Complete* pInstance = new CUI_Complete((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CUI_Complete_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CUI_Complete::Free()
{
	__super::Free();


	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
