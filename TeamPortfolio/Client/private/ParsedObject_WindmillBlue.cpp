#include "stdafx.h"
#include "..\public\ParsedObject_WindmillBlue.h"

CParsedObject_WindmillBlue::CParsedObject_WindmillBlue(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParsedObject(pGraphic_Device)
{
}

CParsedObject_WindmillBlue::CParsedObject_WindmillBlue(const CParsedObject_WindmillBlue & rhs)
	: CParsedObject(rhs)
{
}

HRESULT CParsedObject_WindmillBlue::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CParsedObject_WindmillBlue::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		memcpy(&m_WindmillBlue, pArg, sizeof(WINDMILLBLUEDESC));
		m_Layer_Tag = (TEXT("Layer_WindmillBlue"));
		m_ComTransform->Scaled(_float3(1.f, 1.f, 1.f));
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, m_WindmillBlue.fTransform);
	}

	FAILED_CHECK(m_ComTexture->Change_TextureLayer(TEXT("WindmillBlue")));

	m_ComTransform->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(m_WindmillBlue.Angle));

	return S_OK;
}

_int CParsedObject_WindmillBlue::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_ComTransform->Turn_CW(_float3(0, 0, 1), fTimeDelta);

	return _int();
}

_int CParsedObject_WindmillBlue::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;


	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS),6))
		m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CParsedObject_WindmillBlue::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;


	if (FAILED(m_ComTexture->Bind_Texture()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	FAILED_CHECK(m_ComVIBuffer->Render());

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CParsedObject_WindmillBlue::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CParsedObject_WindmillBlue::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

	return _int();
}


HRESULT CParsedObject_WindmillBlue::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_Parsed"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	/* For.Com_VIBuffer_Cube */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_WindmillBlue"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;



	///////////////////////////////////////////////////////
	/* For.Com_Collision */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	//////////////////////////////////////////////
	return S_OK;
}

HRESULT CParsedObject_WindmillBlue::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CParsedObject_WindmillBlue::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	///////////////////////////
	///////////////////////////////

	return S_OK;
}

CParsedObject_WindmillBlue * CParsedObject_WindmillBlue::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParsedObject_WindmillBlue* pInstance = new CParsedObject_WindmillBlue(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject_WindmillBlue");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CParsedObject_WindmillBlue::Clone(void * pArg)
{
	CParsedObject_WindmillBlue* pInstance = new CParsedObject_WindmillBlue((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject_WindmillBlue");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CParsedObject_WindmillBlue::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
