#include "stdafx.h"
#include "..\public\Object_PortalCube_A.h"
#include "Object_PortalCube_B.h"


CObject_PortalCube_A::CObject_PortalCube_A(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_PortalCube_A::CObject_PortalCube_A(const CObject_PortalCube_A & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_PortalCube_A::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObject_PortalCube_A::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (pArg == nullptr)
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;



	POTALDESC tDesc;
	memcpy(&tDesc, pArg, sizeof(POTALDESC));

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, tDesc.vPos_A_Cube);


	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(tDesc.iNowScene, TEXT("Layer_Potal"), TEXT("Prototype_GameObject_Object_PortalCube_B"), &tDesc.vPos_B_Cube))
		return E_FAIL;


	CObject_PortalCube_B* pTargetCube = (CObject_PortalCube_B*)(GetSingle(CGameInstance)->Get_ObjectList_from_Layer(m_eNowSceneNum, TEXT("Layer_Potal"))->back());

	if (pTargetCube == nullptr || lstrcmp(TEXT("Layer_Potal"), pTargetCube->Get_Layer_Tag()))
		return E_FAIL;

	m_pTargetCube = pTargetCube;
	if(FAILED(pTargetCube->Set_TargetCube(this)))
		return E_FAIL;


	m_Layer_Tag = TEXT("Layer_Potal");

	return S_OK;
}

_int CObject_PortalCube_A::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);

	if (m_Seconds > 0 && m_Seconds < 3)
	{
		m_Seconds += fTimeDelta;
		if (m_Seconds > 3.f)
			m_Seconds = 0;
	}

	return _int();
}

_int CObject_PortalCube_A::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;

	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CObject_PortalCube_A::Render()
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

_int CObject_PortalCube_A::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_PortalCube_A::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	if (!m_Seconds &&!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_Player")))
	{
		Object_Transfer(pDestObjects, fDeltaTime);
	}

	return _int();
}

_int CObject_PortalCube_A::Object_Transfer(CGameObject * pDestObjects, _float fDeltaTime)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


		if (m_pTargetCube == nullptr || pDestObjects == nullptr)
			return -1;

		m_Seconds += fDeltaTime;
		m_pTargetCube->UsedPotal(fDeltaTime);

		CTransform* Player = (CTransform*)(pDestObjects->Get_Component(TAG_COM(Com_Transform)));
		CTransform* PortalCube_B = (CTransform*)(m_pTargetCube->Get_Component(TAG_COM(Com_Transform)));
		_float3 PortalCube_B_Pos = PortalCube_B->Get_MatrixState(CTransform::STATE_POS);
		//PortalCube_B_Pos.y += 1.f;
		Player->Set_MatrixState(CTransform::STATE_POS, PortalCube_B_Pos);



	RELEASE_INSTANCE(CGameInstance);

	return _int();
}


HRESULT CObject_PortalCube_A::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Texture_Shop), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
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

HRESULT CObject_PortalCube_A::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CObject_PortalCube_A::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	///////////////////////////
	m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	///////////////////////////////

	return S_OK;
}

CObject_PortalCube_A * CObject_PortalCube_A::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_PortalCube_A* pInstance = new CObject_PortalCube_A(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_PortalCube_A");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_PortalCube_A::Clone(void * pArg)
{
	CObject_PortalCube_A* pInstance = new CObject_PortalCube_A((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_PortalCube_A");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_PortalCube_A::Free()
{
	__super::Free();


	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
