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
	m_rcButtonRect.top = vUIDesc.y - vUIDesc.w *0.5f;
	m_rcButtonRect.bottom = vUIDesc.y + vUIDesc.w *0.5f;
	m_rcButtonRect.right = vUIDesc.x + vUIDesc.z*0.5f;
	m_rcButtonRect.left = vUIDesc.x - vUIDesc.z*0.5f;

	if (FAILED(Set_UI_Transform(m_ComTransform, vUIDesc)))
		return E_FAIL;


	return S_OK;
}

_int CMyButton::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	
	CGameInstance* pInstance = GetSingle(CGameInstance);
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&m_rcButtonRect, ptMouse))
	{
		if (pInstance->Get_DIKeyState(CInput_Device::MBS_LBUTTON) & DIS_Down)
		{
			if (!lstrcmp(L"Buy", m_pButtonName))
			{
				m_Player_Inventory->Set_Skill_Level(m_eSkill, 1);
				/*m_Player_Inventory->Set_Gold(m_eSkill,m_Player_Inventory->)*/
			}
			else if (!lstrcmp(L"Sell", m_pButtonName))
			{
				m_Player_Inventory->Set_Skill_Level(m_eSkill, -1);
			}
			else if (!lstrcmp(L"Exit", m_pButtonName))
					return 1;
			else if (!lstrcmp(L"?", m_pButtonName))
					return 1;
			else if (!lstrcmp(L"?", m_pButtonName))
					return 1;
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
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STAGESELECT, TEXT("Prototype_Component_Texture_Run"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	m_Player_Inventory = (CInventory*)(GetSingle(CGameInstance)->Get_Commponent_By_LayerIndex(SCENE_STAGESELECT, TEXT("Layer_Player"), TEXT("Com_Inventory"), 0));
	Safe_AddRef(m_Player_Inventory);


	return S_OK;
}




HRESULT CMyButton::Buy_Skill(_int eSKILL)
{
	/*if (m_Skill[eSKILL].Price <= m_Player_Inventory->Get_Gold())
	{
		m_Player_Inventory->Set_Skill_Level(eSKILL, 1);
		m_Player_Inventory->Set_Gold(-m_Skill[eSKILL].Price);
	}
	else
		MSGBOX("소지금이 부족합니다")*/

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
