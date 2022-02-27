#include "stdafx.h"
#include "..\public\LobyPlayer.h"



CLobyPlayer::CLobyPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CLobyPlayer::CLobyPlayer(const CLobyPlayer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLobyPlayer::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLobyPlayer::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_ComTransform->Scaled(_float3(3.f, 3.5f, 3.5f));
	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(0, 2.f, -4.5f));

	return S_OK;
}

_int CLobyPlayer::Update(_float fTimeDelta)
{

	if (0 > __super::Update(fTimeDelta))
		return -1;
	m_fFrame = fTimeDelta;

	return _int();
}

_int CLobyPlayer::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;


		m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);


	return _int();
}

_int CLobyPlayer::Render()
{
	if (nullptr == m_ComVIBuffer || m_ComTransform == nullptr)
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix_Look_Camera()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(m_fFrame)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return _int();
}

_int CLobyPlayer::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}



HRESULT CLobyPlayer::SetUp_Components()
{	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_LobyPlayerTexture"), TAG_COM(Com_Texture), (CComponent**)&m_ComTexture)))
		return E_FAIL;



	return S_OK;
}



HRESULT CLobyPlayer::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 130);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

	return S_OK;
}

HRESULT CLobyPlayer::Release_RenderState()
{
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	return S_OK;
}

CLobyPlayer * CLobyPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CLobyPlayer* pInstance = new CLobyPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CLobyPlayer");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CLobyPlayer::Clone(void * pArg)
{
	CLobyPlayer* pInstance = new CLobyPlayer((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CLobyPlayer");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CLobyPlayer::Free()
{
	__super::Free();

	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
