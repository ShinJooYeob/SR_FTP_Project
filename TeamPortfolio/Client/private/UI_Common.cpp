#include "stdafx.h"
#include "MyButton.h"
#include "UI_Image.h"
#include "../public/UI_Common.h"


CUI_Common::CUI_Common(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CUI_Common::CUI_Common(const CUI_Common & rhs)
	: CUI(rhs), m_UIPrototypes(rhs.m_UIPrototypes)
{

	for (auto& pair : m_UIPrototypes)
	{
		Safe_AddRef(pair.second);
	}

}

HRESULT CUI_Common::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	m_UIPrototypes.emplace(TEXT("UI_ProtoType_Button"), CMyButton::Create(m_pGraphicDevice));
	m_UIPrototypes.emplace(TEXT("UI_ProtoType_Image"), CUI_Image::Create(m_pGraphicDevice));
	return S_OK;
}

HRESULT CUI_Common::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_ComTransform->Rotation_CW(_float3(0.f, 0.f, 1.f), D3DXToRadian(90));
	m_vUIDesc = _float4(m_fPosX, 360, 800, 100);
	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button(TEXT("Layer_Button"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI_Image(TEXT("Layer_UI_Image"))))
		return E_FAIL;

	if (FAILED(m_ComTexture->Change_TextureLayer(TEXT("Button12"))))
		return E_FAIL;


	return S_OK;
}

_int CUI_Common::Set_SkillSlot()
{
	((CUI_Image*)Find_Image(L"Common_Image_9"))->Set_ImageAlpha(40);
	((CUI_Image*)Find_Image(L"Common_Image_10"))->Set_ImageAlpha(40);
	((CUI_Image*)Find_Image(L"Common_Image_11"))->Set_ImageAlpha(40);
	((CUI_Image*)Find_Image(L"Common_Image_12"))->Set_ImageAlpha(40);

	if (m_Player_Inventory->Get_Skill_Level(SKILL_SPEEDUP) == 1)
		((CUI_Image*)Find_Image(L"Common_Image_9"))->Set_ImageAlpha(255);
	if (m_Player_Inventory->Get_Skill_Level(SKILL_DUBBLEJUMP) == 1)
		((CUI_Image*)Find_Image(L"Common_Image_10"))->Set_ImageAlpha(255);
	if (m_Player_Inventory->Get_Skill_Level(SKILL_DASH) == 1)
		((CUI_Image*)Find_Image(L"Common_Image_11"))->Set_ImageAlpha(255);
	if (m_Player_Inventory->Get_Skill_Level(SKILL_POTION) == 1)
		((CUI_Image*)Find_Image(L"Common_Image_12"))->Set_ImageAlpha(255);
	
	return 0;
}
_int CUI_Common::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	
	Set_SkillSlot();
	//_float fPosX = GetSingle(CGameInstance)->Easing(TYPE_ExpoInOut, m_fStartPosX, m_fTargetPosx, m_fPassedTime);
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	m_rcRect.top = (LONG)0;
	m_rcRect.left = (LONG)0;
	m_rcRect.bottom = (LONG)720;
	m_rcRect.right = (LONG)50;
	if (PtInRect(&m_rcRect, ptMouse))
	{
		
		m_MouseOn = true;
		if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON)&(DIS_Down))
		{
			m_bIsClicked = !m_bIsClicked;
		}
	}
	else
		m_MouseOn = false;
	if(!m_bIsClicked)
	{
		
		if (m_MouseOn)
			Show_UI(fDeltaTime);

		if (!m_MouseOn)
			Hide_UI(fDeltaTime);
	}


	

	if (FAILED(Update_UIList(fDeltaTime)))
		return E_FAIL;
	if (FAILED(Update_UIButtonList(fDeltaTime)))
		return E_FAIL;



	return _int();
}
HRESULT CUI_Common::Show_UI(_float fDeltaTime)
{
	if(m_fPosX<40)
	{
		m_fPosX += fDeltaTime * 320;
		m_vUIDesc = _float4(m_fPosX, 360, 800, 100);
			if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
				return E_FAIL;
			for (auto pair : m_UIList)
				((CUI_Image*)(pair.second))->Set_ImageUIDesc(fDeltaTime * 320);
	}


}
HRESULT CUI_Common::Hide_UI(_float fDeltaTime)
{
	if (m_fPosX>-40)
	{
		m_fPosX -= fDeltaTime * 320;
		m_vUIDesc = _float4(m_fPosX, 360, 800, 100);
		if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
			return E_FAIL;
		for (auto pair : m_UIList)
			((CUI_Image*)(pair.second))->Set_ImageUIDesc(-(fDeltaTime * 320));
	}


}

_int CUI_Common::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;





	//������ �׷쿡 �־��ִ� ��Ȱ

	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;
	if (FAILED(LateUpdate_UIList(fDeltaTime)))
		return E_FAIL;
	if (FAILED(LateUpdate_UIButtonList(fDeltaTime)))
		return E_FAIL;


	return _int();
}

_int CUI_Common::Render()
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

_int CUI_Common::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

CUI * CUI_Common::Find_UI(const _tchar * tagUI)
{
	auto iter = find_if(m_UIPrototypes.begin(), m_UIPrototypes.end(), CTagFinder(tagUI));

	if (iter == m_UIPrototypes.end())
		return nullptr;

	return iter->second;
}

CUI * CUI_Common::Find_Image(const _tchar * tagUIList)
{
	auto iter = find_if(m_UIList.begin(), m_UIList.end(), CTagFinder(tagUIList));

	if (iter == m_UIList.end())
		return nullptr;

	return iter->second;
}
HRESULT CUI_Common::Ready_Layer_UI_Image(const _tchar * pLayerTag)
{
	CUI_Image* temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x , m_vUIDesc.y-280, 50, 50)));
	temp->Set_ImageName(L"Common_2");
	m_UIList.emplace(L"Common_Image_9", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x, m_vUIDesc.y-200, 50, 50)));
	temp->Set_ImageName(L"Common_3");
	m_UIList.emplace(L"Common_Image_10", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x, m_vUIDesc.y-120, 50, 50)));
	temp->Set_ImageName(L"Common_4");
	m_UIList.emplace(L"Common_Image_11", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x, m_vUIDesc.y-40, 50, 50)));
	temp->Set_ImageName(L"Common_5");
	m_UIList.emplace(L"Common_Image_12", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x, m_vUIDesc.y - 280, 50, 50)));
	temp->Set_ImageName(L"Common_6");
	m_UIList.emplace(L"Common_Image_13", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x, m_vUIDesc.y - 200, 50, 50)));
	temp->Set_ImageName(L"Common_6");
	m_UIList.emplace(L"Common_Image_14", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x, m_vUIDesc.y - 120, 50, 50)));
	temp->Set_ImageName(L"Common_6");
	m_UIList.emplace(L"Common_Image_15", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x, m_vUIDesc.y - 40, 50, 50)));
	temp->Set_ImageName(L"Common_6");
	m_UIList.emplace(L"Common_Image_16", (CUI*)temp);



	 temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x, m_vUIDesc.y - 280, 64, 64)));
	temp->Set_ImageName(L"Common_1");
	m_UIList.emplace(L"Common_Image_1", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x, m_vUIDesc.y - 200, 64, 64)));
	temp->Set_ImageName(L"Common_1");
	m_UIList.emplace(L"Common_Image_2", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x, m_vUIDesc.y - 120, 64, 64)));
	temp->Set_ImageName(L"Common_1");
	m_UIList.emplace(L"Common_Image_3", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x, m_vUIDesc.y - 40, 64, 64)));
	temp->Set_ImageName(L"Common_1");
	m_UIList.emplace(L"Common_Image_4", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x, m_vUIDesc.y + 40, 64, 64)));
	temp->Set_ImageName(L"Common_1");
	m_UIList.emplace(L"Common_Image_5", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x, m_vUIDesc.y + 120, 64, 64)));
	temp->Set_ImageName(L"Common_1");
	m_UIList.emplace(L"Common_Image_6", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x, m_vUIDesc.y + 200, 64, 64)));
	temp->Set_ImageName(L"Common_1");
	m_UIList.emplace(L"Common_Image_7", (CUI*)temp);

	temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x, m_vUIDesc.y + 280, 64, 64)));
	temp->Set_ImageName(L"Common_1");
	m_UIList.emplace(L"Common_Image_8", (CUI*)temp);



	
	return S_OK;
}
HRESULT CUI_Common::Set_Image_Render(const _tchar * tagUIList, _bool bCheck)
{
	CUI_Image* temp = (CUI_Image*)Find_Image(tagUIList);
	if (bCheck == true)
		temp->Set_ImageRenderTrue();
	else if (bCheck == false)
		temp->Set_ImageRenderFalse();

	return S_OK;
}

HRESULT CUI_Common::Ready_Layer_Button(const _tchar * pLayerTag)
{

	/*CMyButton* temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x - 76, m_vUIDesc.y - 115, 32, 32)));
	temp->Set_ButtonName(L"Quest_Button1");
	m_UIButtonList.emplace(L"Quest_Button_1", (CUI*)temp);

	temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x - 44, m_vUIDesc.y - 115, 32, 32)));
	temp->Set_ButtonName(L"Quest_Button2");
	m_UIButtonList.emplace(L"Quest_Button_2", (CUI*)temp);

	temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x - 12, m_vUIDesc.y - 115, 32, 32)));
	temp->Set_ButtonName(L"Quest_Button3");
	m_UIButtonList.emplace(L"Quest_Button_3", (CUI*)temp);

	temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x + 20, m_vUIDesc.y - 115, 32, 32)));
	temp->Set_ButtonName(L"Quest_Button4");
	m_UIButtonList.emplace(L"Quest_Button_4", (CUI*)temp);*/


	return S_OK;
}
HRESULT CUI_Common::Make_Bigger(const _tchar * pLayerTag)
{
	CUI_Image* temp = (CUI_Image*)Find_Image(pLayerTag);
	if (temp->Get_ImageBigger() == CUI_Image::BIGGER_ON)
		temp->Set_ImageBigger(CUI_Image::BIGGER_OFF);
	else if (!(temp->Get_ImageBigger() == CUI_Image::BIGGER_ON))
		temp->Set_ImageBigger(CUI_Image::BIGGER_ON);
	return S_OK;
}



HRESULT CUI_Common::Update_UIList(_float fTimeDelta)
{
	for (auto pair : m_UIList)
		(pair.second->Update(fTimeDelta));
	return S_OK;
}
HRESULT CUI_Common::Update_UIButtonList(_float fTimeDelta)
{
	int hr;


	for (auto pair : m_UIButtonList) {
		hr = (pair.second->Update(fTimeDelta));
		if (hr != SHOP_END)
			break;
	}

	//switch (hr)
	//{
	//case 101:
	//	m_BiggerTag = (L"Quest_Image_1");
	//	Make_Bigger(m_BiggerTag);
	//	break;
	//case 102:
	//	m_BiggerTag = (L"Quest_Image_1");
	//	Make_Bigger(m_BiggerTag);
	//	break;
	//case 103:
	//	m_BiggerTag = (L"Quest_Image_1");
	//	Make_Bigger(m_BiggerTag);
	//	break;
	//case 104:
	//	m_BiggerTag = (L"Quest_Image_1");
	//	Make_Bigger(m_BiggerTag);
	//	break;
	//default:/*m_iChosenSkill = SKILL_END;*/
	//	break;
	//}




	return S_OK;
}

HRESULT CUI_Common::LateUpdate_UIList(_float fTimeDelta)
{
	for (auto pair : m_UIList)
	{
		if (FAILED(pair.second->LateUpdate(fTimeDelta)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_Common::LateUpdate_UIButtonList(_float fTimeDelta)
{
	for (auto pair : m_UIButtonList)
	{
		if (FAILED(pair.second->LateUpdate(fTimeDelta)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_Common::SetUp_Components()
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




HRESULT CUI_Common::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;



	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);



	return S_OK;
}

HRESULT CUI_Common::Release_RenderState()
{
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}


CUI_Common * CUI_Common::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CUI_Common* pInstance = new CUI_Common(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CUI_Common_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CUI_Common::Clone(void * pArg)
{
	CUI_Common* pInstance = new CUI_Common((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CUI_Common_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CUI_Common::Free()
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
