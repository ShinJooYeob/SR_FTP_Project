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
	m_vUIDesc = _float4(150, g_iWinCY >> 1, 300, 437);
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

static int temp = lstrlen(L"0123456789\nShe's gone\nOut of my life\nI was wrong\nI'm to blame\nI was so untrue\nI can't live without her love\nIn my life\nThere's just an empty space\nAll my dreams are lost\nI'm wasting away\nForgive me, girl");
_int CQuest_Image::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	if (m_fTextFrame < temp)
		m_fTextFrame += fDeltaTime * 10;

	if (m_bIsPress == true)
	{
		if (FAILED(Update_UIList(fDeltaTime)))
			return E_FAIL;
		if (FAILED(Update_UIButtonList(fDeltaTime)))
			return E_FAIL;
		

	}

	return _int();
}


_int CQuest_Image::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;





	//렌더링 그룹에 넣어주는 역활
	CGameInstance* pInstance = GetSingle(CGameInstance);

	if (pInstance->Get_DIKeyState(DIK_U) & DIS_Down)
	{
		m_bIsPress = !m_bIsPress;
		m_fTextFrame = 0;
	}

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



	GetSingle(CGameInstance)->Render_Font(L"0123456789\nShe's gone\nOut of my life\nI was wrong\nI'm to blame\nI was so untrue\nI can't live without her love\nIn my life\nThere's just an empty space\nAll my dreams are lost\nI'm wasting away\nForgive me, girl", { 300.f,100.f }, { 20.f,30.f }, _float3(_float(rand() % 255), _float(rand() % 255), _float(rand() % 255)), (_uint)m_fTextFrame);

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


	CUI_Image* temp = (CUI_Image*)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x, m_vUIDesc.y + 50, 0.01f, 0.01f)));
	temp->Set_ImageName(L"Quest_2");
	m_UIList.emplace(L"Quest_Image_1", (CUI*)temp);


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

	CMyButton* temp = (CMyButton*)(Find_UI(TEXT("UI_ProtoType_Button"))->Clone(&_float4(m_vUIDesc.x - 76, m_vUIDesc.y - 115, 32, 32)));
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
	m_UIButtonList.emplace(L"Quest_Button_4", (CUI*)temp);


	return S_OK;
}
HRESULT CQuest_Image::Make_Bigger(const _tchar * pLayerTag)
{
	CUI_Image* temp=(CUI_Image*)Find_Image(pLayerTag);
	if(temp->Get_ImageBigger()== CUI_Image::BIGGER_ON)
		temp->Set_ImageBigger(CUI_Image::BIGGER_OFF);
	else if (!(temp->Get_ImageBigger() == CUI_Image::BIGGER_ON))
		temp->Set_ImageBigger(CUI_Image::BIGGER_ON);
	return S_OK;
}
HRESULT CQuest_Image::Update_UIList(_float fTimeDelta)
{
	for (auto pair : m_UIList)
	 (pair.second->Update(fTimeDelta));
	return S_OK;
}
HRESULT CQuest_Image::Update_UIButtonList(_float fTimeDelta)
{
	int hr;


	for (auto pair : m_UIButtonList) {
		hr = (pair.second->Update(fTimeDelta));
		if (hr != SHOP_END)
			break;
	}

	switch (hr)
	{
	case 101:
		m_BiggerTag = (L"Quest_Image_1");
		Make_Bigger(m_BiggerTag);
		break;
	case 102:
		m_BiggerTag = (L"Quest_Image_1");
		Make_Bigger(m_BiggerTag);
		break;
	case 103:
		m_BiggerTag = (L"Quest_Image_1");
		Make_Bigger(m_BiggerTag);
		break;
	case 104:
		m_BiggerTag = (L"Quest_Image_1");
		Make_Bigger(m_BiggerTag);
		break;
	default:/*m_iChosenSkill = SKILL_END;*/
		break;
	}




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
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STAGESELECT, TAG_CP(Prototype_Texture_UI), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	m_Player_Inventory = (CInventory*)(GetSingle(CGameInstance)->Get_Commponent_By_LayerIndex(SCENE_STATIC, TEXT("Layer_Player"), TEXT("Com_Inventory"), 0));

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
		MSGBOX("Fail to Create CQuest_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CQuest_Image::Clone(void * pArg)
{
	CQuest_Image* pInstance = new CQuest_Image((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CQuest_Clone");
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
