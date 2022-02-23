#include "stdafx.h"
#include "..\public\Object_PortalCube_B.h"


CObject_PortalCube_B::CObject_PortalCube_B(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_PortalCube_B::CObject_PortalCube_B(const CObject_PortalCube_B & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_PortalCube_B::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObject_PortalCube_B::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_ComTransform->Scaled(_float3(1.f, 1.f, 1.f));

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS,_float3(5.f,0.f,15.f));

	return S_OK;
}

_int CObject_PortalCube_B::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (Collision == true)
	{
		Collision = false;
	}
	else
	{
		Seconds = 0;
	}
	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CObject_PortalCube_B::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;

	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CObject_PortalCube_B::Render()
{
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;


	if (FAILED(m_ComTexture->Bind_Texture()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_ComVIBuffer->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CObject_PortalCube_B::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_PortalCube_B::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_Cube")))
	{
		Object_Transfer(fDeltaTime);
	}

	return _int();
}

_int CObject_PortalCube_B::Object_Transfer(_float fDeltaTime)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	Collision = true;

	Seconds += fDeltaTime;

	if (Seconds > 2)
	{

		CTransform* Player = (CTransform*)pGameInstance->Get_Commponent_By_LayerIndex(SCENE_STAGE2, TEXT("Layer_Cube"), TAG_COM(Com_Transform));

		CTransform* PortalCube_A = (CTransform*)pGameInstance->Get_Commponent_By_LayerIndex(SCENE_STAGE2, TEXT("Layer_PortalCube_A"), TAG_COM(Com_Transform));

		_float3 PortalCube_A_Pos = PortalCube_A->Get_MatrixState(CTransform::STATE_POS);

		PortalCube_A_Pos.y += 1.f;

		Player->Set_MatrixState(CTransform::STATE_POS, PortalCube_A_Pos);

		Seconds = 0;
	};


	RELEASE_INSTANCE(CGameInstance);
	return _int();
}


HRESULT CObject_PortalCube_B::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(SCENE_STAGE2, TEXT("Prototype_Component_Object_PortalCube_B_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer_Cube */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;


	///////////////////////////////////////////////////////
	/* For.Com_Collision */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	//////////////////////////////////////////////
	return S_OK;
}

HRESULT CObject_PortalCube_B::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CObject_PortalCube_B::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	///////////////////////////
	m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	///////////////////////////////

	return S_OK;
}

CObject_PortalCube_B * CObject_PortalCube_B::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_PortalCube_B* pInstance = new CObject_PortalCube_B(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_PortalCube_B");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_PortalCube_B::Clone(void * pArg)
{
	CObject_PortalCube_B* pInstance = new CObject_PortalCube_B((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_PortalCube_B");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_PortalCube_B::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
