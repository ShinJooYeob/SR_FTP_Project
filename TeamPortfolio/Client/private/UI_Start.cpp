#include "stdafx.h"
#include "..\public\UI_Start.h"
#include "Scene.h"





CUI_Start::CUI_Start(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CUI_Start::CUI_Start(const CUI_Start & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Start::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Start::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	ZeroMemory(m_vUIDesc, sizeof(_float4) * 3);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	FAILED_CHECK(SetUp_UIDesc());

	return S_OK;
}
void CUI_Start::Set_UI_TransformRect(_float4 vRect)
{
	//vRect.x,y,z,w=top,left,bottom,right
	_float4 vResult{};
	/*너비*/vResult.z = vRect.w - vRect.y;
	/*높이*/vResult.w = vRect.z - vRect.x;
	/*x좌표*/vResult.x = vRect.y + vResult.z*0.5f;
	/*y좌표*/vResult.y = vRect.x + vResult.w*0.5f;
	Set_UI_Transform(m_ComTransform, vResult);

}

_int CUI_Start::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;




	FAILED_CHECK(Update_MouseButton(fDeltaTime));

	FAILED_CHECK(Update_Animation(fDeltaTime));

	return _int();

}





_int CUI_Start::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;



	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return _int();
}

_int CUI_Start::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;



	if (m_fCountFrame < 6)
	{
		FAILED_CHECK(First_SetUp_RenderState());

		FAILED_CHECK(Second_SetUp_RenderState());
	}



	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return _int();
}

_int CUI_Start::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

HRESULT CUI_Start::First_SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc[0])))
		return E_FAIL;
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;
	FAILED_CHECK(m_ComTexture->Change_TextureLayer(L"UI_Stage"));
	if (FAILED(m_ComTexture->Bind_Texture(m_eNowSceneNum-4)))
		return E_FAIL;
	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Start::Second_SetUp_RenderState()
{

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	//카운트다운
	if (m_bStarton == false)
	{
		if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc[1])))
			return E_FAIL;

		if (FAILED(m_ComTransform->Bind_WorldMatrix()))
			return E_FAIL;

		FAILED_CHECK(m_ComTexture->Change_TextureLayer(L"UI_Num"));
		FAILED_CHECK(m_ComTexture->Bind_Texture(_uint(m_fCountFrame)));
		if ((_uint)m_fCountFrame != (_int)m_fOldframe)
		{
			GetSingle(CGameInstance)->PlaySound(L"JW_CountDown.ogg", CHANNEL_UI,2.f);
			m_fOldframe = m_fCountFrame;
		}
		if (FAILED(m_ComVIBuffer->Render()))
			return E_FAIL;
	}
	

	//스타트
	if (m_bStarton)
	{
		FAILED_CHECK(m_ComTexture->Change_TextureLayer(L"UI_Start"));
		if ((_uint)m_fCountFrame != (_uint)m_fOldframe)
		{
			if (m_fCountFrame < 5)
			{
				GetSingle(CGameInstance)->PlaySound(L"JW_stage_start.wav", CHANNEL_UI, 2.f);
				m_fOldframe = m_fCountFrame;
			}
		}
		
		if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc[2])))
			return E_FAIL;

		if (FAILED(m_ComTransform->Bind_WorldMatrix()))
			return E_FAIL;
		if (FAILED(m_ComTexture->Bind_Texture(0)))
			return E_FAIL;

		if (FAILED(m_ComVIBuffer->Render()))
			return E_FAIL;
	}





	return S_OK;
}


HRESULT CUI_Start::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	return S_OK;
}

HRESULT CUI_Start::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));


	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_StartUI"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Start::SetUp_UIDesc()
{
	//스테이지
	m_vUIDesc[0].x = g_iWinCX >> 1;
	m_vUIDesc[0].y = (g_iWinCY>>1)-200;
	m_vUIDesc[0].z = 320;
	m_vUIDesc[0].w = 180;
	//숫자 카운트다운
	m_vUIDesc[1].x = (g_iWinCX >> 1);
	m_vUIDesc[1].y = (g_iWinCY >> 1);
	m_vUIDesc[1].z = 180;
	m_vUIDesc[1].w = 180;
	//스타트
	m_vUIDesc[2].x = (g_iWinCX >> 1); //m_vUIDesc[1].x + 170.f;// m_vUIDesc[1].x - 10.f;
	m_vUIDesc[2].y = (g_iWinCY >> 1) ;
	m_vUIDesc[2].z = 800;
	m_vUIDesc[2].w = 600;


	return S_OK;
}

HRESULT CUI_Start::Update_MouseButton(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CUI_Start::Update_Animation(_float fTimeDelta)
{
	
	m_fCountFrame += fTimeDelta ;
	if (m_fCountFrame >= 4)
		m_bStarton = true;
	
	
	if (m_bStarton&&m_fCountFrame < 6)
	{
		m_fStartPoint = g_iWinCX >> 1;
		m_fTargetPoint = g_iWinCX + 400;
		m_fDepth = -2;
		m_fPassedTime += fTimeDelta;
		

			m_vUIDesc[2].x = GetSingle(CGameInstance)->Easing(TYPE_ElasticInOut, m_fStartPoint, m_fTargetPoint, m_fPassedTime, 2.f);
			m_vUIDesc[0].x = g_iWinCX -(m_vUIDesc[2].x);
	}
	

	return S_OK;
}

RECT CUI_Start::TransUIDesc_to_Rect(_float4 UIDesc)
{
	RECT tRsult{};

	tRsult.left = (LONG)(UIDesc.x - UIDesc.z*0.5f);
	tRsult.top = (LONG)(UIDesc.y - UIDesc.w*0.5f);
	tRsult.right = (LONG)(UIDesc.x + UIDesc.z*0.5f);
	tRsult.bottom = (LONG)(UIDesc.y + UIDesc.w*0.5f);


	return tRsult;
}





CUI_Start * CUI_Start::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CUI_Start* pInstance = new CUI_Start(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create Pause_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CUI_Start::Clone(void * pArg)
{
	CUI_Start* pInstance = new CUI_Start((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create Pause_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CUI_Start::Free()
{
	__super::Free();


	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
