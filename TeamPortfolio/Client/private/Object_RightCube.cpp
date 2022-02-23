#include "stdafx.h"
#include "..\public\Object_RightCube.h"


CObject_RightCube::CObject_RightCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_RightCube::CObject_RightCube(const CObject_RightCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_RightCube::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObject_RightCube::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_ComTransform->Scaled(_float3(1.f, 1.f, 1.f));

	if (pArg != nullptr) {
		_float3 vSettingPoint;
		memcpy(&vSettingPoint, pArg, sizeof(_float3));
		memcpy(&m_fTempPos, pArg, sizeof(_float3)); //원래 자리로 돌아가기 위한 템프 포지션
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSettingPoint);
	}


	//m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(-1.f, 2.f, 2.f));

	return S_OK;
}

_int CObject_RightCube::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_float3 RightPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
	if (m_bCollisionSwitch == true)
	{
		m_bCollisionSwitch = false;
	}
	else if (m_fTempPos.x < RightPos.x)
	{
		//속도조절 가능
		m_fTimer = fTimeDelta * 1.2f;
		_float3 RightCubePos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(RightCubePos.x - m_fTimer, RightCubePos.y , RightCubePos.z));
	}

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CObject_RightCube::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;

	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CObject_RightCube::Render()
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

_int CObject_RightCube::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_RightCube::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_Cube")))
	{
		//오른쪽으로 움직임
		Collision_RightMoving(pDestObjects, fCollision_Distance, fDeltaTime);
	}

	return _int();
}

_int CObject_RightCube::Collision_RightMoving(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	m_bCollisionSwitch = true;

	_float3 RisingCubePos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);

	//속도 조절 가능
	m_fTimer = fDeltaTime * 1.2f;
	//_float TempAngle = GetSingle(CGameInstance)->Easing(TYPE_BounceOut, m_RotAngle, m_RotAngle + 90, seconds - 3.f, 2.0f);

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(RisingCubePos.x + m_fTimer, RisingCubePos.y , RisingCubePos.z));

	CTransform* DestTransform = (CTransform*)pDestObjects->Get_Component(TAG_COM(Com_Transform));

	_float3 DestPos = DestTransform->Get_MatrixState(CTransform::STATE_POS);


	DestTransform->Set_MatrixState(CTransform::STATE_POS, _float3(RisingCubePos.x - m_fTimer, DestPos.y, DestPos.z));

	return _int();
}


HRESULT CObject_RightCube::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	///* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(SCENE_STAGE2, TEXT("Prototype_Component_Object_RightCube_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
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

HRESULT CObject_RightCube::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CObject_RightCube::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	///////////////////////////
	m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	///////////////////////////////

	return S_OK;
}

CObject_RightCube * CObject_RightCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_RightCube* pInstance = new CObject_RightCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_RightCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_RightCube::Clone(void * pArg)
{
	CObject_RightCube* pInstance = new CObject_RightCube((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_RightCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_RightCube::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
