#include "stdafx.h"
#include "MyButton.h"
#include "UI_Image.h"
#include "../public/Quest_Image.h"


CQuest_Image::CQuest_Image(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CQuest_Image::CQuest_Image(const CQuest_Image & rhs)
	: CUI(rhs), m_UIPrototypes(rhs.m_UIPrototypes)
{

	for (auto& pair : m_UIPrototypes)
	{
		Safe_AddRef(pair.second);
	}
	
}

HRESULT CQuest_Image::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	m_UIPrototypes.emplace(TEXT("UI_ProtoType_Button"), CMyButton::Create(m_pGraphicDevice));
	m_UIPrototypes.emplace(TEXT("UI_ProtoType_Image"), CUI_Image::Create(m_pGraphicDevice));
	return S_OK;
}

HRESULT CQuest_Image::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_vUIDesc = _float4(150 , g_iWinCY >> 1, 300, 437);
	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button(TEXT("Layer_Button"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI_Image(TEXT("Layer_UI_Image"))))
		return E_FAIL;

	if (FAILED(m_ComTexture->Change_TextureLayer(TEXT("Quest_1"))))
		return E_FAIL;


	return S_OK;
}

_int CQuest_Image::Update(_float fDeltaTime)
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

_int CQuest_Image::Manual_Render(_int eSkill)
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

_int CQuest_Image::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;





	//렌더링 그룹에 넣어주는 역활
	CGameInstance* pInstance = GetSingle(CGameInstance);

	if (pInstance->Get_DIKeyState(DIK_U) & DIS_Down)
		m_bIsPress = !m_bIsPress;

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

_int CQuest_Image::Render()
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

_int CQuest_Image::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

CUI * CQuest_Image::Find_UI(const _tchar * tagUI)
{
	auto iter = find_if(m_UIPrototypes.begin(), m_UIPrototypes.end(), CTagFinder(tagUI));

	if (iter == m_UIPrototypes.end())
		return nullptr;

	return iter->second;
}

CUI * CQuest_Image::Find_Image(const _tchar * tagUIList)
{
	auto iter = find_if(m_UIList.begin(), m_UIList.end(), CTagFinder(tagUIList));

	if (iter == m_UIList.end())
		return nullptr;

	return iter->second;
}
HRESULT CQuest_Image::Ready_Layer_UI_Image(const _tchar * pLayerTag)
{


	/*CUI_Image* temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x - 450, m_vUIDesc.y - 140, 100, 100)));
	temp->Set_ImageName(L"Quest_frame");
	m_UIList.emplace(L"Quest_1", (CUI*)temp);*/

	
	return S_OK;
}
HRESULT CQuest_Image::Set_Image_Render(const _tchar * tagUIList, _bool bCheck)
{
	CUI_Image* temp = (CUI_Image*)Find_Image(tagUIList);
	if (bCheck == true)
		temp->Set_ImageRenderTrue();
	else if (bCheck == false)
		temp->Set_ImageRenderFalse();

	return S_OK;
}

HRESULT CQuest_Image::Ready_Layer_Button(const _tchar * pLayerTag)
{

	CMyButton* temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x -60, m_vUIDesc.y -115, 64, 64)));
	temp->Set_ButtonName(L"Quest_Button1");
	m_UIButtonList.emplace(L"Quest_Button_1", (CUI*)temp);

	
	return S_OK;
}
HRESULT CQuest_Image::Update_UIList(_float fTimeDelta)
{
	return S_OK;
}
HRESULT CQuest_Image::Update_UIButtonList(_float fTimeDelta)
{
	int hr;
	CGameInstance* pInstance = GetSingle(CGameInstance);

	
		for (auto pair : m_UIButtonList)
			hr = (pair.second->Update(fTimeDelta));
	//		switch (hr)
	//		{
	//		case SHOP_SPEEDUP:
	//			m_iChosenSkill = SHOP_SPEEDUP;
	//			Manual_Render(m_iChosenSkill);
	//			break;
	//		case SHOP_DUBBLEJUMP:
	//			m_iChosenSkill = SHOP_DUBBLEJUMP;
	//			Manual_Render(m_iChosenSkill);
	//			break;
	//		case SHOP_DASH:
	//			m_iChosenSkill = SHOP_DASH;
	//			Manual_Render(m_iChosenSkill);
	//			break;
	//		case SHOP_POTION:
	//			m_iChosenSkill = SHOP_POTION;
	//			Manual_Render(m_iChosenSkill);
	//			break;
	//		case SHOP_BUY:
	//			if (m_iChosenSkill > SKILL_END + 9)//스킬인덱스 초과 방지용
	//				break;
	//			
	//			break;
	//		case SHOP_EXIT:
	//			m_bIsPress = false;
	//			break;
	//		case SHOP_SELL:
	//			break;
	//		default:/*m_iChosenSkill = SKILL_END;*/
	//			break;
	//		}

	//	}
	//}

	return S_OK;
}

HRESULT CQuest_Image::LateUpdate_UIList(_float fTimeDelta)
{
	for (auto pair : m_UIList)
	{
		if (FAILED(pair.second->LateUpdate(fTimeDelta)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CQuest_Image::LateUpdate_UIButtonList(_float fTimeDelta)
{
	for (auto pair : m_UIButtonList)
	{
		if (FAILED(pair.second->LateUpdate(fTimeDelta)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CQuest_Image::SetUp_Components()
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

	if (m_Player_Inventory == nullptr)
		return E_FAIL;

	Safe_AddRef(m_Player_Inventory);


	return S_OK;
}




HRESULT CQuest_Image::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;



	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);



	return S_OK;
}

HRESULT CQuest_Image::Release_RenderState()
{
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}


CQuest_Image * CQuest_Image::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CQuest_Image* pInstance = new CQuest_Image(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CShop_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CQuest_Image::Clone(void * pArg)
{
	CQuest_Image* pInstance = new CQuest_Image((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CShop_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CQuest_Image::Free()
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
