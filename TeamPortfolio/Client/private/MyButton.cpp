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
	vUIDesc =*(_float4*)pArg;
	m_rcButtonRect.top = LONG(vUIDesc.y - vUIDesc.w *0.5f);
	m_rcButtonRect.bottom = LONG(vUIDesc.y + vUIDesc.w *0.5f);
	m_rcButtonRect.right = LONG(vUIDesc.x + vUIDesc.z*0.5f);
	m_rcButtonRect.left = LONG(vUIDesc.x - vUIDesc.z*0.5f);

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

		if (PtInRect(&m_rcButtonRect, ptMouse))
		{
			if (!lstrcmp(L"SPEEDUP", m_pButtonName))
					return SHOP_SPEEDUP;
			else if (!lstrcmp(L"DUBBLEJUMP", m_pButtonName))
				return SHOP_DUBBLEJUMP;
			else if (!lstrcmp(L"DASH", m_pButtonName))
					return SHOP_DASH;
			else if (!lstrcmp(L"POTION", m_pButtonName))
					return SHOP_POTION;
			else if (!lstrcmp(L"Buy", m_pButtonName))
					return SHOP_BUY;
			else if (!lstrcmp(L"Exit", m_pButtonName))
				return SHOP_EXIT;
			else if (!lstrcmp(L"Sell", m_pButtonName))
			{
				MSGBOX("판매 완료");
				return SHOP_SELL;
			}
		}
	}
	return _int();

}

_int CMyButton::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;





	//렌더링 그룹에 넣어주는 역활
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

	/*if (FAILED(SetUp_RenderState()))
	return E_FAIL;*/

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	//if (FAILED(Release_RenderState()))
	//	return E_FAIL;



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

	Safe_Release(m_Player_Inventory);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
