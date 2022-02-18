#include "stdafx.h"
#include "..\public\Inventory_Main.h"
#include "Camera_Main.h"
#include "Camera.h"




Inventory_Main::Inventory_Main(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
}

Inventory_Main::Inventory_Main(const Inventory_Main & rhs)
	: CGameObject(rhs)
{
}

HRESULT Inventory_Main::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT Inventory_Main::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	return S_OK;
}

_int Inventory_Main::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;

	/*

	CGameInstance* pInstance = GetSingle(CGameInstance);
*/
	
	


	return _int();
}

_int Inventory_Main::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;


	


	//렌더링 그룹에 넣어주는 역활
	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return _int();
}

_int Inventory_Main::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL; 


	if (FAILED(m_ComTransform->Bind_WorldMatrix_Look_Camera()))
		return E_FAIL;
	 
	
	 //if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		//return E_FAIL;
	
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

_int Inventory_Main::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

HRESULT Inventory_Main::SetUp_Components()
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



HRESULT Inventory_Main::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	return S_OK;
}

HRESULT Inventory_Main::Release_RenderState()
{
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}


Inventory_Main * Inventory_Main::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	Inventory_Main* pInstance = new Inventory_Main(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CPlayer_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * Inventory_Main::Clone(void * pArg)
{
	Inventory_Main* pInstance = new Inventory_Main((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CPlayer_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void Inventory_Main::Free()
{
	__super::Free();


	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
