#include "stdafx.h"
#include "..\public\Collision_Object.h"



CCollision_Object::CCollision_Object(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CCollision_Object::CCollision_Object(const CCollision_Object & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCollision_Object::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCollision_Object::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (pArg == nullptr)
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;


		_float3 vSettingPoint;
		memcpy(&vSettingPoint, pArg, sizeof(_float3));
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSettingPoint);

		m_PlayerTransform = nullptr;
		m_PlayerTransform = (CTransform*)(GetSingle(CGameInstance)->Get_Commponent_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));
		if (m_PlayerTransform == nullptr)
			return E_FAIL;

		Safe_AddRef(m_PlayerTransform);
	

	return S_OK;
}

_int CCollision_Object::Update(_float fTimeDelta)
{

	if (0 > __super::Update(fTimeDelta))
		return -1;

	if (m_PlayerTransform->Get_MatrixState(CTransform::STATE_POS).Get_Distance(m_ComTransform->Get_MatrixState(CTransform::STATE_POS)) < 3.f)
		m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);



	return _int();
}

_int CCollision_Object::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	return _int();
}

_int CCollision_Object::Render()
{


	return _int();
}

_int CCollision_Object::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CCollision_Object::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

	return _int();
}

HRESULT CCollision_Object::SetUp_Components()
{	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collision), TAG_COM(Com_Collision), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;


	return S_OK;
}



CCollision_Object * CCollision_Object::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CCollision_Object* pInstance = new CCollision_Object(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CCollision_Object");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CCollision_Object::Clone(void * pArg)
{
	CCollision_Object* pInstance = new CCollision_Object((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CCollision_Object");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CCollision_Object::Free()
{
	__super::Free();

	Safe_Release(m_PlayerTransform);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
}
