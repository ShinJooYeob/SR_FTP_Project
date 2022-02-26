#include "stdafx.h"
#include "..\public\UI_Image.h"





CUI_Image::CUI_Image(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CUI_Image::CUI_Image(const CUI_Image & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Image::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Image::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	_float4 vUIDesc;
	vUIDesc = *(_float4*)pArg;
	m_vUIDesc = vUIDesc;
	m_rcRect.top = LONG(vUIDesc.y - vUIDesc.w *0.5f);
	m_rcRect.bottom = LONG(vUIDesc.y + vUIDesc.w *0.5f);
	m_rcRect.right = LONG(vUIDesc.x + vUIDesc.z*0.5f);
	m_rcRect.left = LONG(vUIDesc.x - vUIDesc.z*0.5f);

	if (FAILED(Set_UI_Transform(m_ComTransform, vUIDesc)))
		return E_FAIL;


	return S_OK;
}

_int CUI_Image::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	if (!lstrcmp(L"Common_1", m_pImageName)|| !lstrcmp(L"Common_2", m_pImageName)|| !lstrcmp(L"Common_3", m_pImageName)|| !lstrcmp(L"Common_4", m_pImageName)|| !lstrcmp(L"Common_5", m_pImageName))
	{
		
		if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
			return E_FAIL;
	}
	
	if (m_iBigger == BIGGER_ON)
	{
		if (m_bEasingStart == false)
		{
			m_vEasingDesc.x = 0.00001f;
			m_vEasingDesc.y = 200.f;
			m_vEasingDesc.z = 0.f;
			m_vEasingDesc.w = 1.f;
			m_bEasingStart = true;
		}
		if (m_bEasingStart == true)
		{
			m_vEasingDesc.z += fDeltaTime;
			if (m_vEasingDesc.z > m_vEasingDesc.w)
			{
				
				
			}
			else
			{
				
				m_vUIDesc.z = GetSingle(CGameInstance)->Easing(TYPE_BounceIn, m_vEasingDesc.x, m_vEasingDesc.y, m_vEasingDesc.z, m_vEasingDesc.w);
				m_vUIDesc.w = m_vUIDesc.z * 18 / 20;
				if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
					return E_FAIL;
			}
		}
	}

	if (m_iBigger == BIGGER_OFF)
	{
		if (m_bEasingStart == false)
		{
			m_vEasingDesc.x = 200.f;
			m_vEasingDesc.y = 0.00001f;
			m_vEasingDesc.z = 0.f;
			m_vEasingDesc.w = 2.f;
			m_bEasingStart = true;
		}
		if (m_bEasingStart == true)
		{
			m_vEasingDesc.z += fDeltaTime;
			if (m_vEasingDesc.z > m_vEasingDesc.w)
			{
				
			}
			else
			{
				
				m_vUIDesc.z = GetSingle(CGameInstance)->Easing(TYPE_BounceOut, m_vEasingDesc.x, m_vEasingDesc.y, m_vEasingDesc.z, m_vEasingDesc.w);
				m_vUIDesc.w = m_vUIDesc.z * 18 / 20;
				if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
					return E_FAIL;
			}
		}
	}
	


	
	if (m_iBigger == BIGGER_OFF && m_vUIDesc.z>20)
	{
		m_vUIDesc.z -= 20;
		m_vUIDesc.w -= 18;

		if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
			return E_FAIL;


	}
	return _int();

}

HRESULT CUI_Image::SetUp_RenderState()
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
	//
	//
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);


	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 20);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	return S_OK;
}

HRESULT CUI_Image::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}


_int CUI_Image::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;





	//렌더링 그룹에 넣어주는 역활
	if (m_bRender == true)
	{
		if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
			return E_FAIL;
	}
	return _int();
}

_int CUI_Image::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture((_uint)m_fFrame)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return _int();
}

_int CUI_Image::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

void CUI_Image::Set_ImageName(TCHAR * pImageName)
{
	{ m_pImageName = pImageName; };
	if (!lstrcmp(L"Button1", m_pImageName))
	{
		m_ComTexture->Change_TextureLayer(L"Button1");
	}
	else if (!lstrcmp(L"Button2", m_pImageName))
	{
		m_ComTexture->Change_TextureLayer(L"Button2");
	}
	else if (!lstrcmp(L"Price1", m_pImageName))
	{
		m_ComTexture->Change_TextureLayer(L"Price1");
	}
	else if (!lstrcmp(L"Price2", m_pImageName))
	{
		m_ComTexture->Change_TextureLayer(L"Price2");
	}
	else if (!lstrcmp(L"Price3", m_pImageName))
	{
		m_ComTexture->Change_TextureLayer(L"Price3");
	}
	else if (!lstrcmp(L"manual_Speedup", m_pImageName))
	{
		m_ComTexture->Change_TextureLayer(L"manual_Speedup");
		m_bRender = false;
	}
	else if (!lstrcmp(L"manual_DJ", m_pImageName))
	{
		m_ComTexture->Change_TextureLayer(L"manual_DJ");
		m_bRender = false;
	}
	else if (!lstrcmp(L"manual_Dash", m_pImageName))
	{
		m_ComTexture->Change_TextureLayer(L"manual_Dash");
		m_bRender = false;
	}
	else if (!lstrcmp(L"manual_Potion", m_pImageName))
	{
		m_ComTexture->Change_TextureLayer(L"manual_Potion");
		m_bRender = false;
	}
	////////////////////common///////////////
	else if (!lstrcmp(L"Quest_2", m_pImageName))
	{
		m_ComTexture->Change_TextureLayer(L"Quest_2");
	}
	else if (!lstrcmp(L"Common_1", m_pImageName))
	{
		m_ComTexture->Change_TextureLayer(L"Common_1");
	}
	else if (!lstrcmp(L"Common_2", m_pImageName))
	{
		m_ComTexture->Change_TextureLayer(L"SPEEDUP");
	}
	else if (!lstrcmp(L"Common_3", m_pImageName))
	{
		m_ComTexture->Change_TextureLayer(L"DUBBLEJUMP");
	}
	else if (!lstrcmp(L"Common_4", m_pImageName))
	{
		m_ComTexture->Change_TextureLayer(L"DASH");
	}
	else if (!lstrcmp(L"Common_5", m_pImageName))
	{
		m_ComTexture->Change_TextureLayer(L"POTION");
	}
	else if (!lstrcmp(L"Common_6", m_pImageName))
	{
		m_ComTexture->Change_TextureLayer(L"CoolDown");
	}
}

HRESULT CUI_Image::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));


	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STAGESELECT, TAG_CP(Prototype_Texture_UI), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;


	return S_OK;
}





CUI_Image * CUI_Image::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CUI_Image* pInstance = new CUI_Image(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CShop_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CUI_Image::Clone(void * pArg)
{
	CUI_Image* pInstance = new CUI_Image((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CShop_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CUI_Image::Free()
{
	__super::Free();


	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
