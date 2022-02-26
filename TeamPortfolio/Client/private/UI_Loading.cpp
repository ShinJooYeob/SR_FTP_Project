#include "stdafx.h"
#include "..\public\UI_Loading.h"
#include "UI_Image.h"

CUI_Loading::CUI_Loading(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CUI_Loading::CUI_Loading(const CUI_Loading & rhs)
	: CUI(rhs)//, m_UIPrototypes(rhs.m_UIPrototypes)
{
	//for (auto& pair : m_UIPrototypes)
	//{
	//	Safe_AddRef(pair.second);
	//}
}


HRESULT CUI_Loading::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	//m_UIPrototypes.emplace(TEXT("UI_ProtoType_Image"), CUI_Image::Create(m_pGraphicDevice));
}

HRESULT CUI_Loading::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_vUIDesc = _float4(g_iWinCX >> 1, g_iWinCY >> 1, g_iWinCX, g_iWinCY);
	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
		return E_FAIL;

	//CUI_Image* temp = (CUI_Image)(Find_UI(TEXT("UI_ProtoType_Image"))->Clone(&_float4(m_vUIDesc.x - 285, m_vUIDesc.y - 140, 100, 100)));
	//temp->Set_ImageName(L"Button2");
	//m_UIList.emplace(L"Image_1", (CUI)temp);



}

_int CUI_Loading::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	m_fFrame = fDeltaTime;
	return _int();
}

_int CUI_Loading::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;

	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;
	return _int();
}

_int CUI_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(m_fFrame)))
		return E_FAIL;
	//if (FAILED(m_ComTexture->Bind_Texture(0)))
	//	return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CUI_Loading::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

HRESULT CUI_Loading::Ready_Layer_UI_Image(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CUI_Loading::SetUp_Components()
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
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Loading_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;
}

HRESULT CUI_Loading::SetUp_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	return S_OK;
}

HRESULT CUI_Loading::Release_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

CUI * CUI_Loading::Find_UI(const _tchar * tagUI)
{
	return nullptr;
}

CUI * CUI_Loading::Find_Image(const _tchar * tagUIList)
{
	return nullptr;
}

CUI_Loading * CUI_Loading::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CUI_Loading* pInstance = new CUI_Loading(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CUI_Loading_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CUI_Loading::Clone(void * pArg)
{
	CUI_Loading* pInstance = new CUI_Loading((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CUI_Loading_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CUI_Loading::Free()
{
	__super::Free();

	//이건 프로토타입용
	//for (auto pair : m_UIPrototypes)
	//{
	//	Safe_Release(pair.second);

	//}
	//m_UIPrototypes.clear();

	////프로토타입을 오브젝트 내부에 만드니깐 내부에서 클론 이미지들을 없애준다.
	//for (auto& pair : m_UIList)
	//{
	//	Safe_Release(pair.second);

	//}
	//m_UIList.clear();

	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
