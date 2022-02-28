#include "stdafx.h"
#include "..\public\LobySkyBox.h"
#include "Camera_Main.h"

CLobySkyBox::CLobySkyBox(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CLobySkyBox::CLobySkyBox(const CLobySkyBox & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLobySkyBox::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLobySkyBox::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_ComTransform->Scaled(_float3(32.f, 32.f/g_iWinCX*g_iWinCY, 32.f));

	return S_OK;
}

_int CLobySkyBox::Update(_float fTimeDelta)
{
	if (m_eNowSceneNum == SCENE_LOADING)
		return S_FALSE;
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_ComTransform->Turn_CW(_float3(0, 1, 0), fTimeDelta);

	m_fTexttureFrame += fTimeDelta * 0.25f;
	if (m_fTexttureFrame > 6 )
		m_fTexttureFrame = 0;

	return _int();
}

_int CLobySkyBox::LateUpdate(_float fTimeDelta)
{
	if (m_eNowSceneNum == SCENE_LOADING)
		return S_FALSE;

	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;


	_Matrix		ViewMatrix;
	m_pGraphicDevice->GetTransform(D3DTS_VIEW, &ViewMatrix);
	ViewMatrix = ViewMatrix.InverseMatrix();

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, *(_float3*)&ViewMatrix.m[3][0]);
	

	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

	return _int();
}

_int CLobySkyBox::Render()
{


	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	FAILED_CHECK(Late_SetUp_RenderState());


	//m_ComTransform->Scaled(_float3(32.f, 32.f / g_iWinCX*g_iWinCY , 32.f ));


	if (FAILED(m_ComTexture->Bind_Texture(m_fTexttureFrame)))// 몇번째 인덱스인지 명시 0부터 시작
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	//m_ComTransform->Scaled(_float3(32.f - 1.f, 32.f / g_iWinCX*g_iWinCY - 1.f, 32.f - 1.f));
	if (m_fTexttureFrame > 5)
	{
		if (FAILED(m_ComTexture->Bind_Texture(0)))// 몇번째 인덱스인지 명시 0부터 시작
			return E_FAIL;
	}
	else {
		if (FAILED(m_ComTexture->Bind_Texture(m_fTexttureFrame + 1)))// 몇번째 인덱스인지 명시 0부터 시작
			return E_FAIL;
	}



	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	
	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

_int CLobySkyBox::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}


HRESULT CLobySkyBox::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(3.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Texture_SkyBox"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;


	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;


	m_MainCamera = (CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	if (m_MainCamera == nullptr)
		return E_FAIL;

	Safe_AddRef(m_MainCamera);

	return S_OK;
}

HRESULT CLobySkyBox::Late_SetUp_RenderState()
{

	m_MainCamera->Set_ProjectMatrix(false);
	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);


	return S_OK;
}

HRESULT CLobySkyBox::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	_uint Alpha = (_uint)(((_float)m_fTexttureFrame - (_uint)m_fTexttureFrame) * 255);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(Alpha, 255, 255, 255));



	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CLobySkyBox::Release_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;


	m_MainCamera->Set_ProjectMatrix(true);


	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);


	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CLobySkyBox * CLobySkyBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CLobySkyBox* pInstance = new CLobySkyBox(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CLobySkyBox");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CLobySkyBox::Clone(void * pArg)
{
	CLobySkyBox* pInstance = new CLobySkyBox((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CLobySkyBox");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CLobySkyBox::Free()
{
	__super::Free();

	Safe_Release(m_MainCamera);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}