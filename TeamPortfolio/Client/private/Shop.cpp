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
	m_vUIDesc = _float4(g_iWinCX >> 1, g_iWinCY >> 1, 800, 600);
	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button(TEXT("Layer_Button"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI_Image(TEXT("Layer_UI_Image"))))
		return E_FAIL;

	if (FAILED(SetUp_Skills()))
		return E_FAIL;

	//m_Player_Inventory->Set_Gold(10000);
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
	if(m_bNotEnoughMoney)
		m_fTime += fDeltaTime; //초당 1씩증가
	if (m_bIsExcessMaxLevel)
		m_fTime += fDeltaTime;
	return _int();
}

_int CShop::Manual_Render(_int eSkill)
{


	if (eSkill == SHOP_SPEEDUP)
	{
		Set_Image_Render(L"Image_5.0", true);
		Set_Image_Render(L"Image_5.1", false);
		Set_Image_Render(L"Image_5.2", false);
		Set_Image_Render(L"Image_5.3", false);
	}
	else if (eSkill == SHOP_DUBBLEJUMP)
	{
		Set_Image_Render(L"Image_5.0", false);
		Set_Image_Render(L"Image_5.1", true);
		Set_Image_Render(L"Image_5.2", false);
		Set_Image_Render(L"Image_5.3", false);
	}
	else if (eSkill == SHOP_DASH)
	{
		Set_Image_Render(L"Image_5.0", false);
		Set_Image_Render(L"Image_5.1", false);
		Set_Image_Render(L"Image_5.2", true);
		Set_Image_Render(L"Image_5.3", false);
	}
	else if (eSkill == SHOP_POTION)
	{
		Set_Image_Render(L"Image_5.0", false);
		Set_Image_Render(L"Image_5.1", false);
		Set_Image_Render(L"Image_5.2", false);
		Set_Image_Render(L"Image_5.3", true);
	}



	return 0;
}

_int CShop::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;





	//렌더링 그룹에 넣어주는 역활
	CGameInstance* pInstance = GetSingle(CGameInstance);

	

	if (m_bIsPress == true)
	{
		if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
			return E_FAIL;
		if (FAILED(LateUpdate_UIList(fDeltaTime)))
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

	wstring temp;
	_tchar szbuf[64];

	_itow_s(m_Player_Inventory->Get_Gold(), szbuf, 10);
	temp = wstring(szbuf) + L"gold";
	GetSingle(CGameInstance)->Render_UI_Font(temp, { 800.f,190.f }, { 20.f,30.f }, _float3(255, 255, 0));

	if (m_Player_Inventory->Get_Skill_Level(SKILL_SPEEDUP) < m_Player_Inventory->Get_MaxLevel(SKILL_SPEEDUP))
	{
		_itow_s(m_Player_Inventory->Get_Skill_Level(SKILL_SPEEDUP), szbuf, 10);
		temp = L"LV" + wstring(szbuf);
		GetSingle(CGameInstance)->Render_UI_Font(temp, { m_vUIDesc.x - 85, m_vUIDesc.y - 140 }, { 16.f,24.f }, _float3(255, 100, 150));
	}
	else if (m_Player_Inventory->Get_Skill_Level(SKILL_SPEEDUP) == m_Player_Inventory->Get_MaxLevel(SKILL_SPEEDUP))
	{
		_itow_s(m_Player_Inventory->Get_Skill_Level(SKILL_SPEEDUP), szbuf, 10);
		temp = L"LV" + wstring(szbuf) + L"MAX";
		GetSingle(CGameInstance)->Render_UI_Font(temp, { m_vUIDesc.x - 85, m_vUIDesc.y - 140 }, { 16.f,24.f }, _float3(255, 100, 150));
	}
	_itow_s(m_Player_Inventory->Get_Skill_Level(SKILL_DUBBLEJUMP), szbuf, 10);
	temp = L"LV" + wstring(szbuf);
	GetSingle(CGameInstance)->Render_UI_Font(temp, { m_vUIDesc.x - 85, m_vUIDesc.y - 40 }, { 16.f,24.f }, _float3(255, 100, 150));

	_itow_s(m_Player_Inventory->Get_Skill_Level(SKILL_CAMERA), szbuf, 10);
	temp = L"LV" + wstring(szbuf);
	GetSingle(CGameInstance)->Render_UI_Font(temp, { m_vUIDesc.x - 85, m_vUIDesc.y + 60 }, { 16.f,24.f }, _float3(255, 100, 150));

	_itow_s(m_Player_Inventory->Get_Skill_Level(SKILL_POTION), szbuf, 10);
	temp = L"LV" + wstring(szbuf);
	GetSingle(CGameInstance)->Render_UI_Font(temp, { m_vUIDesc.x - 85, m_vUIDesc.y + 160 }, { 16.f,24.f }, _float3(255, 100, 150));
	if (m_bNotEnoughMoney)
	{
		GetSingle(CGameInstance)->Render_UI_Font(L"Not enough money", { 500.f,550.f }, { 20.f,30.f }, _float3(255, 255, 255));
		if (m_fTime > 1)//1초가 지나면
		{
			m_bNotEnoughMoney = false;//b false로바꿔서 여기 안들어옴
			m_fTime = 0;//시간 초기화
		}
	}

	if (m_bIsExcessMaxLevel)
	{
		GetSingle(CGameInstance)->Render_UI_Font(L"Excess Max Level", { 500.f,550.f }, { 20.f,30.f }, _float3(255, 255, 255));
		if (m_fTime > 1)//1초가 지나면
		{
			m_bIsExcessMaxLevel = false;//b false로바꿔서 여기 안들어옴
			m_fTime = 0;//시간 초기화
		}
	}

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

CUI * CShop::Find_Image(const _tchar * tagUIList)
{
	auto iter = find_if(m_UIList.begin(), m_UIList.end(), CTagFinder(tagUIList));

	if (iter == m_UIList.end())
		return nullptr;

	return iter->second;
}
HRESULT CShop::Ready_Layer_UI_Image(const _tchar * pLayerTag)
{


	CUI_Image* temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x - 285, m_vUIDesc.y - 140, 100, 100)));
	temp->Set_ImageName(L"Button2");
	m_UIList.emplace(L"Image_1", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x - 285, m_vUIDesc.y - 40, 100, 100)));
	temp->Set_ImageName(L"Button2");
	m_UIList.emplace(L"Image_2", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x - 285, m_vUIDesc.y + 60, 100, 100)));
	temp->Set_ImageName(L"Button2");
	m_UIList.emplace(L"Image_3", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x - 285, m_vUIDesc.y + 160, 100, 100)));
	temp->Set_ImageName(L"Button2");
	m_UIList.emplace(L"Image_4", (CUI*)temp);


	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x + 150, m_vUIDesc.y - 50, 300, 180)));
	temp->Set_ImageName(L"manual_Speedup");
	m_UIList.emplace(L"Image_5.0", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x + 150, m_vUIDesc.y - 50, 300, 180)));
	temp->Set_ImageName(L"manual_DJ");
	m_UIList.emplace(L"Image_5.1", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x + 150, m_vUIDesc.y - 50, 300, 180)));
	temp->Set_ImageName(L"manual_Dash");
	m_UIList.emplace(L"Image_5.2", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x + 150, m_vUIDesc.y - 50, 300, 180)));
	temp->Set_ImageName(L"manual_Potion");
	m_UIList.emplace(L"Image_5.3", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x - 185, m_vUIDesc.y - 140, 100, 100)));
	temp->Set_ImageName(L"Price2");
	m_UIList.emplace(L"Image_6", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x - 185, m_vUIDesc.y - 40, 100, 100)));
	temp->Set_ImageName(L"Price3");
	m_UIList.emplace(L"Image_7", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x - 185, m_vUIDesc.y + 60, 100, 100)));
	temp->Set_ImageName(L"Price3");
	m_UIList.emplace(L"Image_8", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x - 185, m_vUIDesc.y + 160, 100, 100)));
	temp->Set_ImageName(L"Price1");
	m_UIList.emplace(L"Image_9", (CUI*)temp);
	return S_OK;
}
HRESULT CShop::Set_Image_Render(const _tchar * tagUIList, _bool bCheck)
{
	CUI_Image* temp = (CUI_Image*)Find_Image(tagUIList);
	if (bCheck == true)
		temp->Set_ImageRenderTrue();
	else if (bCheck == false)
		temp->Set_ImageRenderFalse();

	return S_OK;
}

HRESULT CShop::Ready_Layer_Button(const _tchar * pLayerTag)
{

	CMyButton* temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x + 95, m_vUIDesc.y + 100, 250, 100)));
	temp->Set_ButtonName(L"Buy");
	m_UIButtonList.emplace(L"Button_Buy", (CUI*)temp);

	temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x + 265, m_vUIDesc.y + 100, 100, 100)));
	temp->Set_ButtonName(L"Exit");
	m_UIButtonList.emplace(L"Button_Exit", (CUI*)temp);

	temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x - 285, m_vUIDesc.y - 140, 50, 50)));
	temp->Set_ButtonName(L"SPEEDUP");
	m_UIButtonList.emplace(L"Button_SPEEDUP", (CUI*)temp);

	temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x - 285, m_vUIDesc.y - 40, 50, 50)));
	temp->Set_ButtonName(L"DUBBLEJUMP");
	m_UIButtonList.emplace(L"Button_DUBBLEJUMP", (CUI*)temp);

	temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x - 285, m_vUIDesc.y + 60, 50, 50)));
	temp->Set_ButtonName(L"CAMERA");
	m_UIButtonList.emplace(L"Button_CAMERA", (CUI*)temp);

	temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x - 285, m_vUIDesc.y + 160, 50, 50)));
	temp->Set_ButtonName(L"POTION");
	m_UIButtonList.emplace(L"Button_POTION", (CUI*)temp);
	return S_OK;
}
HRESULT CShop::Update_UIList(_float fTimeDelta)
{
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
				Manual_Render(m_iChosenSkill);
				break;
			case SHOP_DUBBLEJUMP:
				m_iChosenSkill = SHOP_DUBBLEJUMP;
				Manual_Render(m_iChosenSkill);
				break;
			case SHOP_DASH:
				m_iChosenSkill = SHOP_DASH;
				Manual_Render(m_iChosenSkill);
				break;
			case SHOP_POTION:
				m_iChosenSkill = SHOP_POTION;
				Manual_Render(m_iChosenSkill);
				break;
			case SHOP_BUY:
				if (m_iChosenSkill==0 || m_iChosenSkill > SKILL_END + 9)//스킬인덱스 초과 방지용
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

HRESULT CShop::LateUpdate_UIList(_float fTimeDelta)
{
	for (auto pair : m_UIList)
	{
		if (FAILED(pair.second->LateUpdate(fTimeDelta)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CShop::LateUpdate_UIButtonList(_float fTimeDelta)
{
	for (auto pair : m_UIButtonList)
	{
		if (FAILED(pair.second->LateUpdate(fTimeDelta)))
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
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STAGESELECT, TAG_CP(Prototype_Texture_UI), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	m_Player_Inventory = (CInventory*)(GetSingle(CGameInstance)->Get_Commponent_By_LayerIndex(SCENE_STATIC, TEXT("Layer_Player"), TEXT("Com_Inventory"), 0));

	if (m_Player_Inventory == nullptr)
		return E_FAIL;

	Safe_AddRef(m_Player_Inventory);


	return S_OK;
}

HRESULT CShop::SetUp_Skills()
{
	ZeroMemory(m_Skill, sizeof(_int)*SKILL_END);

	m_Skill[SKILL_SPEEDUP].Price = 1000;

	m_Skill[SKILL_DUBBLEJUMP].Price = 3000;
	
	m_Skill[SKILL_CAMERA].Price = 3000;

	m_Skill[SKILL_POTION].Price = 500;

	return S_OK;
}


HRESULT CShop::Buy_Skill(_int ChosenSkill)
{
	

	if (m_Skill[ChosenSkill].Price <= m_Player_Inventory->Get_Gold())
	{
		m_bNotEnoughMoney = false;
		if (m_Player_Inventory->Get_Skill_Level(ChosenSkill) == m_Player_Inventory->Get_MaxLevel(ChosenSkill))
		{
			m_fTime = 0;
			m_bIsExcessMaxLevel = true;
			return S_OK;
		}
		GetSingle(CGameInstance)->PlaySound(L"JW_Coins.mp3", CHANNEL_UI);
		m_Player_Inventory->Set_Skill_LevelUP(ChosenSkill);
		m_Player_Inventory->Set_Gold(-m_Skill[ChosenSkill].Price);
	}

	else if (m_Skill[ChosenSkill].Price > m_Player_Inventory->Get_Gold())
	{
		m_fTime = 0;
		m_bNotEnoughMoney = true;
			return E_FAIL;
		
	}
		

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
