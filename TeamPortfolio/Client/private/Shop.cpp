#include "stdafx.h"
#include "..\public\Shop.h"
#include "MyButton.h"
#include "UI_Image.h"


CShop::CShop(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CShop::CShop(const CShop & rhs)
	: CUI(rhs), m_UIPrototypes(rhs.m_UIPrototypes)
{

	for (auto& pair : m_UIPrototypes)
	{
		Safe_AddRef(pair.second);
	}
}

HRESULT CShop::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	m_UIPrototypes.emplace(TEXT("UI_ProtoType_Button"), CMyButton::Create(m_pGraphicDevice));
	m_UIPrototypes.emplace(TEXT("UI_ProtoType_Image"), CUI_Image::Create(m_pGraphicDevice));
	return S_OK;
}

HRESULT CShop::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_vUIDesc = _float4(g_iWinCX >> 1, g_iWinCY >> 1, g_iWinCX >> 1, g_iWinCY >> 1);
	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button(TEXT("Layer_Button"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI_Image(TEXT("Layer_UI_Image"))))
		return E_FAIL;

	if (FAILED(SetUp_Skills()))
		return E_FAIL;
	m_Player_Inventory->Set_Gold(10000);
	if (FAILED(m_ComTexture->Change_TextureLayer(TEXT("Shop1"))))
		return E_FAIL;


	return S_OK;
}

_int CShop::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	if (m_bIsPress == true)
	{
		if (FAILED(Update_UIButtonList(fDeltaTime)))
			return E_FAIL;
	}

	return _int();
}

_int CShop::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;





	//렌더링 그룹에 넣어주는 역활
	CGameInstance* pInstance = GetSingle(CGameInstance);

	if (pInstance->Get_DIKeyState(DIK_F11) & DIS_Down)
		m_bIsPress = !m_bIsPress;

	if (m_bIsPress == true)
	{
		if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
			return E_FAIL;

		if (FAILED(LateUpdate_UIButtonList(fDeltaTime)))
			return E_FAIL;
	}

	return _int();
}

_int CShop::Render()
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

_int CShop::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

CUI * CShop::Find_UI(const _tchar * tagUI)
{
	auto iter = find_if(m_UIPrototypes.begin(), m_UIPrototypes.end(), CTagFinder(tagUI));

	if (iter == m_UIPrototypes.end())
		return nullptr;

	return iter->second;
}

HRESULT CShop::Ready_Layer_UI_Image(const _tchar * pLayerTag)
{

	CUI_Image* temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x + 605, m_vUIDesc.y + 75, 100, 100)));
	temp->Set_ButtonName(L"test");
	m_UIList.emplace(L"Image_test", (CUI*)temp);

	return S_OK;
}
HRESULT CShop::Ready_Layer_Button(const _tchar * pLayerTag)
{

	CMyButton* temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x + 115, m_vUIDesc.y + 75, 100, 100)));
	temp->Set_ButtonName(L"Buy");
	m_UIButtonList.emplace(L"Button_Buy", (CUI*)temp);

	temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x + 215, m_vUIDesc.y + 75, 100, 100)));
	temp->Set_ButtonName(L"Exit");
	m_UIButtonList.emplace(L"Button_Exit", (CUI*)temp);

	temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x - 215, m_vUIDesc.y - 60, 100, 100)));
	temp->Set_ButtonName(L"SPEEDUP");
	m_UIButtonList.emplace(L"Button_SPEEDUP", (CUI*)temp);

	temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x - 115, m_vUIDesc.y - 60, 100, 100)));
	temp->Set_ButtonName(L"DUBBLEJUMP");
	m_UIButtonList.emplace(L"Button_DUBBLEJUMP", (CUI*)temp);

	temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x - 15, m_vUIDesc.y - 60, 100, 100)));
	temp->Set_ButtonName(L"DASH");
	m_UIButtonList.emplace(L"Button_DASH", (CUI*)temp);

	temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x + 85, m_vUIDesc.y - 60, 100, 100)));
	temp->Set_ButtonName(L"POTION");
	m_UIButtonList.emplace(L"Button_POTION", (CUI*)temp);
	return S_OK;
}

HRESULT CShop::Update_UIButtonList(_float fTimeDelta)
{
	int hr;
	CGameInstance* pInstance = GetSingle(CGameInstance);

	if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
	{
		for (auto pair : m_UIButtonList)
		{

			hr = (pair.second->Update(fTimeDelta));
			switch (hr)
			{
			case SHOP_SPEEDUP:
				m_iChosenSkill = SHOP_SPEEDUP;
				break;
			case SHOP_DUBBLEJUMP:
				m_iChosenSkill = SHOP_DUBBLEJUMP;
				break;
			case SHOP_DASH:
				m_iChosenSkill = SHOP_DASH;
				break;
			case SHOP_POTION:
				m_iChosenSkill = SHOP_POTION;
				break;
			case SHOP_BUY:
				if (m_iChosenSkill > SKILL_END + 9)//스킬인덱스 초과 방지용
					break;
				Buy_Skill(m_iChosenSkill - SHOP_SPEEDUP);
				break;
			case SHOP_EXIT:
				m_bIsPress = false;
				break;
			case SHOP_SELL:
				break;
			default:/*m_iChosenSkill = SKILL_END;*/
				break;
			}
		}
	}

	return S_OK;
}

HRESULT CShop::LateUpdate_UIButtonList(_float fTimeDelta)
{
	for (auto pUI : m_UIButtonList)
	{
		if (FAILED(pUI.second->LateUpdate(fTimeDelta)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CShop::SetUp_Components()
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
	m_Player_Inventory = (CInventory*)(GetSingle(CGameInstance)->Get_Commponent_By_LayerIndex(SCENE_STAGESELECT, TEXT("Layer_Player"), TEXT("Com_Inventory"), 0));
	Safe_AddRef(m_Player_Inventory);


	return S_OK;
}

HRESULT CShop::SetUp_Skills()
{
	ZeroMemory(m_Skill, sizeof(_int)*SKILL_END);

	m_Skill[SKILL_SPEEDUP].Move_Speed += 1 * (m_Player_Inventory->Get_Skill_Level(SKILL_SPEEDUP));
	m_Skill[SKILL_SPEEDUP].Price = 1000;

	m_Skill[SKILL_DUBBLEJUMP].Price = 3000;

	m_Skill[SKILL_DASH].Price = 3000;

	m_Skill[SKILL_POTION].Price = 500;


	return S_OK;
}
HRESULT CShop::Set_Skill_Rect()
{

	return S_OK;
}

HRESULT CShop::Buy_Skill(_int ChosenSkill)
{


	if (m_Skill[ChosenSkill].Price <= m_Player_Inventory->Get_Gold())
	{
		m_Player_Inventory->Set_Skill_Level(ChosenSkill, 1);
		m_Player_Inventory->Set_Gold(-m_Skill[ChosenSkill].Price);
	}

	else
		MSGBOX("소지금이 부족합니다")

		return S_OK;
}

HRESULT CShop::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;



	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);



	return S_OK;
}

HRESULT CShop::Release_RenderState()
{
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}


CShop * CShop::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CShop* pInstance = new CShop(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CShop_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CShop::Clone(void * pArg)
{
	CShop* pInstance = new CShop((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CShop_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CShop::Free()
{
	__super::Free();


	for (auto& pair : m_UIButtonList)
	{
		Safe_Release(pair.second);

	}
	m_UIButtonList.clear();

	for (auto& pair : m_UIList)
	{
		Safe_Release(pair.second);

	}
	m_UIList.clear();
	for (auto pair : m_UIPrototypes)
	{
		Safe_Release(pair.second);

	}
	m_UIPrototypes.clear();


	Safe_Release(m_Player_Inventory);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
