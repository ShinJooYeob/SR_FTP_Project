#include "stdafx.h"
#include "LobyCube.h"
#include "..\public\Loby_UI.h"
#include "Camera_Main.h"




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

	m_vUIDesc_Logo.x = g_iWinCX *0.5f;
	m_vUIDesc_Logo.y = 180;
	m_vUIDesc_Logo.z = 1000;
	m_vUIDesc_Logo.w = 300;

	m_fAlphaValue = 0;
	m_fTextFrame = 0;
	m_fIndexAlpha = 255.f;
	m_IsSceneChange = false;
	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc_Logo)))
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

	if (m_fIndexAlpha > 0)
	{
		m_fIndexAlpha -= fDeltaTime * 255.f;
		if (m_fIndexAlpha < 0)
			m_fIndexAlpha = 0;
	}

	CGameInstance* pInstace = GetSingle(CGameInstance);

	if (pInstace->Get_DIKeyState(DIK_RETURN) & DIS_Down)
	{
		if (!m_iPageIndex)
		{
			switch (m_iManuIndex)
			{
			case 0:
			{
				CCamera_Main* pMainCam = (CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
				if (pMainCam == nullptr)
					return E_FAIL;
				pMainCam->CameraEffect(CCamera_Main::CAM_EFT_FADE_IN, fDeltaTime, 1.8f);
				m_pLobyCube->Rot_N_SceneChange(fDeltaTime);
				m_fIndexAlpha = 9999999.f;
				m_IsSceneChange = true;
			}
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				PostQuitMessage(NULL);
				break;
			default:
				break;
			}

		}
	}
	if (!m_fIndexAlpha)
	{

		if (pInstace->Get_DIKeyState(DIK_UP) & DIS_Down)
		{
			m_fIndexAlpha = 255.f;
			m_iManuIndex--;
			if (m_iManuIndex < 0)
				m_iManuIndex = 0;
		}
		else if (pInstace->Get_DIKeyState(DIK_DOWN) & DIS_Down)
		{
			m_fIndexAlpha = 255.f;
			m_iManuIndex++;
			if (m_iManuIndex > 3)
				m_iManuIndex = 3;
		}	
		else if (pInstace->Get_DIKeyState(DIK_RIGHT) & DIS_Down)
		{
			m_iPageIndex++;
			m_fIndexAlpha = 510.f;
			if (m_iPageIndex > 3)
				m_iPageIndex = 0;
			m_pLobyCube->Strat_Turning(0);
		}
		else if (pInstace->Get_DIKeyState(DIK_LEFT) & DIS_Down)
		{
			m_iPageIndex--;
			m_fIndexAlpha = 510.f;
			if (m_iPageIndex < 0)
				m_iPageIndex = 3;

			m_pLobyCube->Strat_Turning(1);
		}


	}


	return _int();

}




_int CLoby_UI::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;


	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return _int();
}

HRESULT CLoby_UI::First_SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_vUIDesc_Logo.x = g_iWinCX *0.5f;
	m_vUIDesc_Logo.y = 180;
	m_vUIDesc_Logo.z = 1000;
	m_vUIDesc_Logo.w = 300;

	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc_Logo)))
		return E_FAIL;


	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture(0)))
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


	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CLoby_UI::Second_SetUp_RenderState()
{
	m_vUIDesc_Index.x = g_iWinCX * 0.5f;
	m_vUIDesc_Index.y = 415 + m_iManuIndex * 55.f;
	m_vUIDesc_Index.z = 300;
	m_vUIDesc_Index.w = 50;

	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc_Index)))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture(1)))
		return E_FAIL;

	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_fIndexAlpha > 255)
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 255);
	else 
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, _uint(m_fIndexAlpha));

	


	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	return S_OK;
}
HRESULT CLoby_UI::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);

	return S_OK;
}


_int CLoby_UI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	FAILED_CHECK(First_SetUp_RenderState());
	if (!m_iPageIndex)
		FAILED_CHECK(Second_SetUp_RenderState());


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
	

	m_pLobyCube = (CLobyCube*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Layer_LobyCube")));

	if (m_pLobyCube == nullptr)
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
