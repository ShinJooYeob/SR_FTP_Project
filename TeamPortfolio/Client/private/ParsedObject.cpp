#include "stdafx.h"
#include "..\public\ParsedObject.h"


CParsedObject::CParsedObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CParsedObject::CParsedObject(const CParsedObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CParsedObject::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CParsedObject::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_ComTransform->Scaled(_float3(1.f, 1.f, 1.f));
	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(-3,0,0));


	return S_OK;
}

_int CParsedObject::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;



	return _int();
}

_int CParsedObject::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;


	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS)))
		m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CParsedObject::Render()
{
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

_int CParsedObject::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CParsedObject::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

	return _int();
}


HRESULT CParsedObject::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_Penguine"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	/* For.Com_VIBuffer_Cube */
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_VIBuffer_Penguine"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
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

HRESULT CParsedObject::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CParsedObject::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	///////////////////////////
	///////////////////////////////

	return S_OK;
}

CParsedObject * CParsedObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParsedObject* pInstance = new CParsedObject(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CParsedObject::Clone(void * pArg)
{
	CParsedObject* pInstance = new CParsedObject((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CParsedObject::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
