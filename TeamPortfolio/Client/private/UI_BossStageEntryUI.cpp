#include "stdafx.h"
#include "Player.h"
#include "UI_Result.h"
#include "..\public\UI_BossStageEntryUI.h"





CUI_BossStageEntryUI::CUI_BossStageEntryUI(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CUI_BossStageEntryUI::CUI_BossStageEntryUI(const CUI_BossStageEntryUI & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_BossStageEntryUI::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_BossStageEntryUI::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	FAILED_CHECK(SetUp_UIDesc());

	return S_OK;
}
void CUI_BossStageEntryUI::Set_UI_TransformRect( _float4 vRect)
{
	//vRect.x,y,z,w=top,left,bottom,right
	_float4 vResult{};
	/*³Êºñ*/vResult.z = vRect.w-vRect.y;
	/*³ôÀÌ*/vResult.w = vRect.z-vRect.x;
	/*xÁÂÇ¥*/vResult.x = vRect.y + vResult.z*0.5f;
	/*yÁÂÇ¥*/vResult.y = vRect.x + vResult.w*0.5f;
	Set_UI_Transform(m_ComTransform, vResult);
	
}

_int CUI_BossStageEntryUI::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	
	m_fFrameTime += fDeltaTime;
	FAILED_CHECK(Update_Animation(fDeltaTime));



	if (m_fFrameTime > 16.f)
		DIED();

	return _int();

}





_int CUI_BossStageEntryUI::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;





	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return _int();
}

_int CUI_BossStageEntryUI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;



	FAILED_CHECK(First_SetUp_RenderState());
	
	FAILED_CHECK(Second_SetUp_RenderState());

	FAILED_CHECK(Third_SetUp_RenderState());



	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return _int();
}

_int CUI_BossStageEntryUI::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

HRESULT CUI_BossStageEntryUI::First_SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(_uint(m_vBackFadeColor.x), _uint(m_vBackFadeColor.y), _uint(m_vBackFadeColor.z), _uint(m_vBackFadeColor.w)));

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	FAILED_CHECK(m_ComTexture->Bind_Texture(0));

	FAILED_CHECK(Set_UI_Transform(m_ComTransform, m_vUIDesc[0]));
	FAILED_CHECK(m_ComTransform->Bind_WorldMatrix());
	FAILED_CHECK(m_ComVIBuffer->Render());



	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);

	return S_OK;
}

HRESULT CUI_BossStageEntryUI::Second_SetUp_RenderState()
{
	FAILED_CHECK(m_ComTexture->Bind_Texture(1));

	if (m_bIsEndEntry)
	{
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);


		m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(_uint(m_fFadeOutFrame), 255, 255, 255));

		m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		FAILED_CHECK(Set_UI_Transform(m_ComTransform, m_vUIDesc[1]));
		FAILED_CHECK(m_ComTransform->Bind_WorldMatrix());
		FAILED_CHECK(m_ComVIBuffer->Render());


		FAILED_CHECK(Set_UI_Transform(m_ComTransform, m_vUIDesc[2]));
		FAILED_CHECK(m_ComTransform->Bind_WorldMatrix());
		FAILED_CHECK(m_ComVIBuffer->Render());

	}
	else {


		m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		FAILED_CHECK(Set_UI_Transform(m_ComTransform, m_vUIDesc[1]));
		FAILED_CHECK(m_ComTransform->Bind_WorldMatrix());
		FAILED_CHECK(m_ComVIBuffer->Render());


		FAILED_CHECK(Set_UI_Transform(m_ComTransform, m_vUIDesc[2]));
		FAILED_CHECK(m_ComTransform->Bind_WorldMatrix());
		FAILED_CHECK(m_ComVIBuffer->Render());

	}
	
	return S_OK;
}

HRESULT CUI_BossStageEntryUI::Third_SetUp_RenderState()
{

	if (m_bIsEndEntry)
	{
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);


		m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(_uint(m_fFadeOutFrame), 255, 255, 255));

		m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		FAILED_CHECK(m_ComTexture->Bind_Texture(2));

		FAILED_CHECK(Set_UI_Transform(m_ComTransform, m_vUIDesc[3]));
		FAILED_CHECK(m_ComTransform->Bind_WorldMatrix());
		FAILED_CHECK(m_ComVIBuffer->Render());


		FAILED_CHECK(m_ComTexture->Bind_Texture(4));
		FAILED_CHECK(m_ComVIBuffer->Render());


		FAILED_CHECK(m_ComTexture->Bind_Texture(3));
		FAILED_CHECK(Set_UI_Transform(m_ComTransform, m_vUIDesc[4]));
		FAILED_CHECK(m_ComTransform->Bind_WorldMatrix());
		FAILED_CHECK(m_ComVIBuffer->Render());


	}
	else {



		if (m_vMarkFadeTimes.x > 0)
		{
			m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
			m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


			m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

			m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(_uint(m_vMarkFadeTimes.x), 255, 255, 255));


			FAILED_CHECK(m_ComTexture->Bind_Texture(2));

			FAILED_CHECK(Set_UI_Transform(m_ComTransform, m_vUIDesc[3]));
			FAILED_CHECK(m_ComTransform->Bind_WorldMatrix());
			FAILED_CHECK(m_ComVIBuffer->Render());



			m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

			m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);


			_uint TempAlpha = _uint(255.f - m_vMarkFadeTimes.y);
			if (TempAlpha < 32)
				TempAlpha = 32;

			m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, TempAlpha);
			m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			FAILED_CHECK(m_ComTexture->Bind_Texture(4));
			FAILED_CHECK(m_ComVIBuffer->Render());

			m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, _uint(255.f - m_vMarkFadeTimes.z));
			m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			FAILED_CHECK(m_ComTexture->Bind_Texture(3));
			FAILED_CHECK(Set_UI_Transform(m_ComTransform, m_vUIDesc[4]));
			FAILED_CHECK(m_ComTransform->Bind_WorldMatrix());
			FAILED_CHECK(m_ComVIBuffer->Render());


		}

	}
	return S_OK;
}

HRESULT CUI_BossStageEntryUI::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	return S_OK;
}


HRESULT CUI_BossStageEntryUI::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));


	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_BossEntry"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_BossStageEntryUI::SetUp_UIDesc()
{
	m_vUIDesc[0].x = g_iWinCX * 0.5f;
	m_vUIDesc[0].y = g_iWinCY * 0.5f;
	m_vUIDesc[0].z = g_iWinCX;
	m_vUIDesc[0].w = g_iWinCY;


	m_vUIDesc[1].x = g_iWinCX * 1.5f;
	m_vUIDesc[1].y = 50;
	m_vUIDesc[1].z = g_iWinCX * 3.f;
	m_vUIDesc[1].w = 100;

	m_vUIDesc[2].x = g_iWinCX * -0.5f + 255;
	m_vUIDesc[2].y = g_iWinCY  - 50;
	m_vUIDesc[2].z = g_iWinCX * 3.f;
	m_vUIDesc[2].w = 100;

	m_vUIDesc[3].x = g_iWinCX * 0.5f;
	m_vUIDesc[3].y = g_iWinCY *0.5f -100;
	m_vUIDesc[3].z = g_iWinCX * 0.5f;
	m_vUIDesc[3].w = g_iWinCY * 0.5f;

	m_vUIDesc[4].x = m_vUIDesc[3].x;
	m_vUIDesc[4].y = m_vUIDesc[3].y - m_vUIDesc[3].w * 0.28f;
	m_vUIDesc[4].z = m_vUIDesc[3].z * 0.15f;
	m_vUIDesc[4].w = m_vUIDesc[3].w * 0.25f;



	m_vMarkFadeTimes = _float3(-128.f, 0, 0);

	return S_OK;
}


HRESULT CUI_BossStageEntryUI::Update_Animation(_float fTimeDelta)
{

	if (m_fFrameTime > 12.f)
	{
		m_bIsEndEntry = true;
		m_fFadeOutFrame -= 85.f * fTimeDelta;

		if (m_fFadeOutFrame < 0)
			m_fFadeOutFrame = 0;
	}

	CGameInstance* pInstance = GetSingle(CGameInstance);

	//_float4			m_vBackFadeColor = _float4(255.f,		219.f,		0,		20.f);
	//_float4 target =							{ 0,			245,		210,	9 };
	//////////////////////////////////////////////////////////////////////////
	_uint iShareValue = _uint(m_fFrameTime / 0.5f);

	if (iShareValue % 2)
	{
		m_vBackFadeColor.x = pInstance->Easing(TYPE_Linear, 10.f, 240.f, m_fFrameTime - (iShareValue * 0.5f), 0.5f);
		m_vBackFadeColor.y = pInstance->Easing(TYPE_Linear, 245.f, 219.f, m_fFrameTime - (iShareValue * 0.5f), 0.5f);
		m_vBackFadeColor.z = pInstance->Easing(TYPE_Linear, 170, 0.f, m_fFrameTime - (iShareValue * 0.5f), 0.5f);
		m_vBackFadeColor.w = pInstance->Easing(TYPE_Linear, 9.f, 20.f, m_fFrameTime - (iShareValue * 0.5f), 0.5f);
	}
	else {

		m_vBackFadeColor.y = pInstance->Easing(TYPE_Linear, 219.f, 245.f, m_fFrameTime - (iShareValue * 0.5f), 0.5f);
		m_vBackFadeColor.z = pInstance->Easing(TYPE_Linear, 0, 170.f, m_fFrameTime - (iShareValue * 0.5f), 0.5f);
		m_vBackFadeColor.w = pInstance->Easing(TYPE_Linear, 20.f, 9.f, m_fFrameTime - (iShareValue * 0.5f), 0.5f);

	}
	iShareValue = _uint(m_fFrameTime / 0.75f);
	if (iShareValue%2)
	{
		m_vBackFadeColor.x = pInstance->Easing(TYPE_Linear, 10.f, m_fFadeOutFrame, m_fFrameTime - (iShareValue * 0.75f), 0.75f);
	}
	else
	{

		m_vBackFadeColor.x = pInstance->Easing(TYPE_Linear, m_fFadeOutFrame, 10.f, m_fFrameTime - (iShareValue * 0.75f), 0.75f);
	}


	//////////////////////////////////////////////////////////////////////////



	m_vUIDesc[1].x -= fTimeDelta * g_iWinCX * 0.3f;
	if (m_vUIDesc[1].x < g_iWinCX * -0.5f + 255)
		m_vUIDesc[1].x = g_iWinCX * 1.5f;
	m_vUIDesc[2].x += fTimeDelta * g_iWinCX * 0.3f;
	if (m_vUIDesc[2].x > g_iWinCX * 1.5f)
		m_vUIDesc[2].x = g_iWinCX * -0.5f + 255;


	//////////////////////////////////////////////////////////////////////////
	if (m_vMarkFadeTimes.x < 255.f)
		m_vMarkFadeTimes.x += 85.f * fTimeDelta;
	else if (m_vMarkFadeTimes.y < 255.f)
		m_vMarkFadeTimes.y += 85.f*fTimeDelta;
	else if (m_vMarkFadeTimes.z < 255.f)
		m_vMarkFadeTimes.z += 85.f*fTimeDelta;

	if (m_vMarkFadeTimes.x > 255.f)
		m_vMarkFadeTimes.x = 255.f;
	if (m_vMarkFadeTimes.y > 255.f)
		m_vMarkFadeTimes.y = 255.f;
	if (m_vMarkFadeTimes.z > 255.f)
		m_vMarkFadeTimes.z = 255.f;
	//////////////////////////////////////////////////////////////////////////

	return S_OK	;
}

RECT CUI_BossStageEntryUI::TransUIDesc_to_Rect(_float4 UIDesc)
{
	RECT tRsult{};

	tRsult.left = (LONG)(UIDesc.x - UIDesc.z*0.5f);
	tRsult.top = (LONG)(UIDesc.y - UIDesc.w*0.5f);
	tRsult.right = (LONG)(UIDesc.x + UIDesc.z*0.5f);
	tRsult.bottom = (LONG)(UIDesc.y + UIDesc.w*0.5f);


	return tRsult;
}





CUI_BossStageEntryUI * CUI_BossStageEntryUI::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CUI_BossStageEntryUI* pInstance = new CUI_BossStageEntryUI(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create UIStatus_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CUI_BossStageEntryUI::Clone(void * pArg)
{
	CUI_BossStageEntryUI* pInstance = new CUI_BossStageEntryUI((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create UIStatus_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CUI_BossStageEntryUI::Free()
{
	__super::Free();

	
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
