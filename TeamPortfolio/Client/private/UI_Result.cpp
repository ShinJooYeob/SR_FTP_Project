#include "stdafx.h"
#include "..\public\UI_Result.h"
#include "MyButton.h"
#include "UI_Image.h"

CUI_Result::CUI_Result(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CUI_Result::CUI_Result(const CUI_Result & rhs)
	: CUI(rhs), m_UIPrototypes(rhs.m_UIPrototypes)
{
	for (auto& pair : m_UIPrototypes)
	{
		Safe_AddRef(pair.second);
	}
}

HRESULT CUI_Result::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	//프로토타입 넣는중
	m_UIPrototypes.emplace(TEXT("ProtoType_GameObject_UI_Button"), CMyButton::Create(m_pGraphicDevice));

	return S_OK;
}

HRESULT CUI_Result::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;

	//g_iWinCX >> 1
	//g_iWinCY >> 1
	m_vUIDesc = _float4(g_iWinCX >> 1, g_iWinCY >> 1, g_iWinCX >> 1, 500);

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(999.f, 999.f, 0.f));
	

	if (FAILED(Ready_Layer_Button(TEXT("Layer_Button"))))
		return E_FAIL;

	//개 중요
	//if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
	//	return E_FAIL;


	m_ComTexture->Change_TextureLayer(TEXT("Result"));


	return S_OK;
}

_int CUI_Result::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	m_fFrame = fDeltaTime;

	if (m_bStopSwitch == false)
	{
		m_fTimer += m_fFrame;
	}


	if (m_fTimer > 1.f)
	{
		if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
			return E_FAIL;

		Button_Picking();

		if (FAILED(Update_UIButtonList(fDeltaTime)))
			return E_FAIL;



		m_bStopSwitch = true;
	}



	return _int();
}

_int CUI_Result::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;
	//RENDER_PRIORITY ,RENDER_UI

	//이걸 함으로써 렌더되지 않게 연산량을 줄여주자!!!
	//if (m_fTimer > 3.f)
	//{
		if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
			return E_FAIL;
	//}
	if (FAILED(LateUpdate_UIButtonList(fDeltaTime)))
		return E_FAIL;

	return _int();
}

_int CUI_Result::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	//if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(m_fFrame)))
	//	return E_FAIL;
	if (FAILED(m_ComTexture->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;


	//잊지마라 폰트 매니저는 렌더에서만 사용가능하다.
	_tchar tempArr[64];
	_itow_s(m_fTimer, tempArr, 10);

	wstring TempString = wstring(tempArr) + L"Sec " + L"Eunhyek";

	GetSingle(CGameInstance)->Render_UI_Font(TempString, { 400.f,200.f }, { 20.f,30.f }, _float3(83, 250, 120));

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CUI_Result::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

HRESULT CUI_Result::Ready_Layer_Button(const _tchar * pLayerTag)
{
	CMyButton* temp = (CMyButton*)(Find_UI(TEXT("ProtoType_GameObject_UI_Button"))->Clone(&_float4(999.f, 999.f, 250, 100)));
	temp->Set_ButtonName(L"Button_Result_Start");
	m_UIButtonList.emplace(L"Button_Result_Start", (CUI*)temp);

	temp = (CMyButton*)(Find_UI(TEXT("ProtoType_GameObject_UI_Button"))->Clone(&_float4(999.f, 999.f, 250, 100)));
	temp->Set_ButtonName(L"Button_Result_Cancel");
	m_UIButtonList.emplace(L"Button_Result_Cancel", (CUI*)temp);

	//m_vButtonDesc = { g_iWinCX >> 1,g_iWinCY >> 1, 250 ,100 };

	return S_OK;
}

HRESULT CUI_Result::Update_UIButtonList(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	int hr;
	for (auto pair : m_UIButtonList) {
		hr = (pair.second->Update(fTimeDelta));
		if (hr != SHOP_END)  
			//스위치는 for문 밖에다 쓰도록 하자!
		switch (hr)
		{
		case RESULT_START:
		{
			if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Press)
			{
				POINT ptMouse;
				GetCursorPos(&ptMouse);
				ScreenToClient(g_hWnd, &ptMouse);
				m_isClicked = false;
				if (PtInRect(&m_fStartButton, ptMouse))
				{
					m_isClicked = true;
					CMyButton* ButtonStart = (CMyButton*)Find_Button(L"Button_Result_Start");
					CTexture* ButtonTexture = (CTexture*)ButtonStart->Get_Component(TEXT("Com_Texture"));
					ButtonTexture->Change_TextureLayer(L"Button_Result_Start3");
				}
			}
			if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Up)
			{
				//여기에 기능구현하면 될 듯
			}
			//CMyButton* ButtonStart = (CMyButton*)Find_Button(L"Button_Result_Start");
			
			//이거 되니깐 같이 이동시키면 될 듯 기억하셈
			//ButtonStart->Set_UI_Transform
			//내가 원하는 기능
			break;
		}
		case RESULT_CANCEL:
		{
			break;
		}
		default:
		{
			break;
		}
		}
		
	}


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CUI_Result::LateUpdate_UIButtonList(_float fTimeDelta)
{

	for (auto pair : m_UIButtonList)
	{
		if (FAILED(pair.second->LateUpdate(fTimeDelta)))
			return E_FAIL;
	}
	return S_OK;
}

CUI * CUI_Result::Find_UI(const _tchar * tagUI)
{
	auto iter = find_if(m_UIPrototypes.begin(), m_UIPrototypes.end(), CTagFinder(tagUI));

	if (iter == m_UIPrototypes.end())
		return nullptr;

	return iter->second;
}

CUI * CUI_Result::Find_Button(const _tchar * tagUIList)
{
	auto iter = find_if(m_UIButtonList.begin(), m_UIButtonList.end(), CTagFinder(tagUIList));

	if (iter == m_UIButtonList.end())
		return nullptr;

	return iter->second;
}

HRESULT CUI_Result::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	//m_eNowSceneNum = SCENEID::SCENE_LOADING;

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_UI_Result_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Result::SetUp_RenderState()
{


	//m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_XRGB(255, 255, 255));
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	return S_OK;
}

HRESULT CUI_Result::Release_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

void CUI_Result::Button_Picking()
{
	CMyButton* ButtonStart = (CMyButton*)Find_Button(L"Button_Result_Start");
	CTransform* ButtonTransform = (CTransform*)ButtonStart->Get_Component(TEXT("Com_Transform"));
	//ButtonTransform->Set_MatrixState(CTransform::STATE_POS, _float3(100.f, 200.f, 0));
	m_vButtonDesc = { (g_iWinCX >> 1) - 150,(g_iWinCY >> 1) + 150, 250 ,100 };
	_float4 vUIDesc = m_vButtonDesc;

	RECT TEST;
	TEST.top = LONG(vUIDesc.y - vUIDesc.w *0.5f);
	TEST.bottom = LONG(vUIDesc.y + vUIDesc.w *0.5f);
	TEST.right = LONG(vUIDesc.x + vUIDesc.z*0.5f);
	TEST.left = LONG(vUIDesc.x - vUIDesc.z*0.5f);
	m_fStartButton = TEST;
	ButtonStart->Set_Rect(m_vButtonDesc, TEST);
	ButtonStart->Set_UI_Transform(ButtonTransform, m_vButtonDesc);

	CMyButton* ButtonCancel = (CMyButton*)Find_Button(L"Button_Result_Cancel");
	ButtonTransform = (CTransform*)ButtonCancel->Get_Component(TEXT("Com_Transform"));
	//ButtonTransform->Set_MatrixState(CTransform::STATE_POS, _float3(200.f, -100.f, 0));
	m_vButtonDesc = { (g_iWinCX >> 1) + 150,(g_iWinCY >> 1) + 150, 250 ,100 };
	vUIDesc = m_vButtonDesc;
	TEST.top = LONG(vUIDesc.y - vUIDesc.w *0.5f);
	TEST.bottom = LONG(vUIDesc.y + vUIDesc.w *0.5f);
	TEST.right = LONG(vUIDesc.x + vUIDesc.z*0.5f);
	TEST.left = LONG(vUIDesc.x - vUIDesc.z*0.5f);
	ButtonCancel->Set_Rect(m_vButtonDesc, TEST);
	ButtonCancel->Set_UI_Transform(ButtonTransform, m_vButtonDesc);
}

CUI_Result * CUI_Result::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CUI_Result* pInstance = new CUI_Result(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CUI_Result_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CUI_Result::Clone(void * pArg)
{
	CUI_Result* pInstance = new CUI_Result((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CUI_Result_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CUI_Result::Free()
{
	__super::Free();

	for (auto& pair : m_UIButtonList)
	{
		Safe_Release(pair.second);

	}
	m_UIButtonList.clear();

	for (auto& pair : m_UIPrototypes)
	{
		Safe_Release(pair.second);
	}
	m_UIPrototypes.clear();

	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
