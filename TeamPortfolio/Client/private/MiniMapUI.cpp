#include "stdafx.h"
#include "..\public\MiniMapUI.h"





CMiniMapUI::CMiniMapUI(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

HRESULT CMiniMapUI::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	_float4 UIDesc;
	if (pArg != nullptr)
		memcpy(&UIDesc, pArg, sizeof(_float4));

	if (FAILED(Set_UI_Transform(m_ComTransform, UIDesc)))
		return E_FAIL;

	m_fDepth = 0;

	return S_OK;
}

_int CMiniMapUI::Update(_float fDeltaTime)
{
	return _int();
}

_int CMiniMapUI::LateUpdate(_float fDeltaTime)
{
	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return _int();
}

_int CMiniMapUI::Render()
{

	if (FAILED(__super::Render()))
		return E_FAIL;




	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;


	if (FAILED(SetUp_RenderState()))
		return E_FAIL;



	//마스킹용 텍스처 바인드(1)번에
	if (FAILED(m_ComTexture->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;
	

	_float3 vPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
	vPos.z += 0.001f;
	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vPos);

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;



	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	//그리기용 텍스처 0번에 바인드
	if (FAILED(m_pGraphicDevice->SetTexture(0, m_ComRenderer->Get_MiniMapTex())))
		return E_FAIL;
	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;


	vPos.z -= 0.001f;
	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vPos);


	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return _int();
}

_int CMiniMapUI::LateRender()
{
	return _int();
}

HRESULT CMiniMapUI::Reset_MiniMapSize(_float2 vUIDesc)
{
	_float4 vUITransform = { g_iWinCX - (vUIDesc.x * 0.5f),(vUIDesc.y*0.5f),vUIDesc.x,vUIDesc.y };

	if (FAILED(Set_UI_Transform(m_ComTransform, vUITransform)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMiniMapUI::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));


	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_MiniMap), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMiniMapUI::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESS);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	return S_OK;
}

HRESULT CMiniMapUI::Release_RenderState()
{

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	return S_OK;
}

CMiniMapUI * CMiniMapUI::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CMiniMapUI* pInstance = new CMiniMapUI(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CMiniMapUI");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CMiniMapUI::Clone(void * pArg)
{
	MSGBOX("미니맵UI는 클론할수없습니다");

	return nullptr;
}
void CMiniMapUI::Free()
{
	__super::Free();


	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);

}

