#include "stdafx.h"
#include "..\public\MyButton.h"





CMyButton::CMyButton(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CMyButton::CMyButton(const CMyButton & rhs)
	: CUI(rhs)
{
}

HRESULT CMyButton::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMyButton::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	_float4 vUIDesc;
	vUIDesc = *(_float4*)pArg;
	m_rcRect.top = LONG(vUIDesc.y - vUIDesc.w *0.5f);
	m_rcRect.bottom = LONG(vUIDesc.y + vUIDesc.w *0.5f);
	m_rcRect.right = LONG(vUIDesc.x + vUIDesc.z*0.5f);
	m_rcRect.left = LONG(vUIDesc.x - vUIDesc.z*0.5f);

	if (FAILED(Set_UI_Transform(m_ComTransform, vUIDesc)))
		return E_FAIL;


	return S_OK;
}

_int CMyButton::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;

	CGameInstance* pInstance = GetSingle(CGameInstance);

	if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
	{
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);
		m_isClicked = false;
		if (PtInRect(&m_rcRect, ptMouse))
		{
			if (!lstrcmp(L"SPEEDUP", m_pButtonName))
			{
				m_isClicked = true;
				return SHOP_SPEEDUP;
			}
			else if (!lstrcmp(L"DUBBLEJUMP", m_pButtonName))
			{
				m_isClicked = true;
				return SHOP_DUBBLEJUMP;
			}
			else if (!lstrcmp(L"DASH", m_pButtonName))
			{
				m_isClicked = true;
				return SHOP_DASH;
			}
			else if (!lstrcmp(L"POTION", m_pButtonName))
			{
				m_isClicked = true;
				return SHOP_POTION;
			}
			else if (!lstrcmp(L"Buy", m_pButtonName))
			{

				return SHOP_BUY;
			}
			else if (!lstrcmp(L"Exit", m_pButtonName))
			{

				return SHOP_EXIT;
			}
			else if (!lstrcmp(L"Sell", m_pButtonName))
			{
				MSGBOX("�Ǹ� �Ϸ�");
				return SHOP_SELL;
			}
		}
	}
	return SHOP_END;

}

HRESULT CMyButton::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	if (m_isClicked)
	{
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//Sour => ���� �׸������ϴ� �׸��� ��
		//Dest => �������� ȭ�鿡 �׷��� ��
		//
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
		m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(120, 255, 255, 255));
		//
		//
		//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);


		m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 20);
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	}

	return S_OK;
}

HRESULT CMyButton::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}


_int CMyButton::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;





	//������ �׷쿡 �־��ִ� ��Ȱ
	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;
	return _int();
}

_int CMyButton::Render()
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

_int CMyButton::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

void CMyButton::Set_ButtonName(TCHAR * pButtonName)
{
	{ m_pButtonName = pButtonName; };
	if (!lstrcmp(L"Buy", m_pButtonName))
	{
		m_ComTexture->Change_TextureLayer(L"Buy");
	}
	else if (!lstrcmp(L"Exit", m_pButtonName))
	{
		m_ComTexture->Change_TextureLayer(L"Exit");
	}
	else if (!lstrcmp(L"DUBBLEJUMP", m_pButtonName))
	{
		m_ComTexture->Change_TextureLayer(L"DUBBLEJUMP");
	}
	else if (!lstrcmp(L"DASH", m_pButtonName))
	{
		m_ComTexture->Change_TextureLayer(L"DASH");
	}
	else if (!lstrcmp(L"SPEEDUP", m_pButtonName))
	{
		m_ComTexture->Change_TextureLayer(L"SPEEDUP");
	}
	else if (!lstrcmp(L"POTION", m_pButtonName))
	{
		m_ComTexture->Change_TextureLayer(L"POTION");
	}
}

HRESULT CMyButton::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));


	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STAGESELECT, TEXT("Prototype_Component_Texture_Shop"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;


	return S_OK;
}





CMyButton * CMyButton::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CMyButton* pInstance = new CMyButton(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CShop_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CMyButton::Clone(void * pArg)
{
	CMyButton* pInstance = new CMyButton((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CShop_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CMyButton::Free()
{
	__super::Free();


	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
