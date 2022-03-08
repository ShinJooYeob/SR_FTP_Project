#include "stdafx.h"
#include "..\public\Com_Gun.h"
#include "..\public\Bullet.h"

CCom_Gun::CCom_Gun(LPDIRECT3DDEVICE9 pGraphicDevice)
 : CComponent(pGraphicDevice)
{
}

CCom_Gun::CCom_Gun(const CCom_Gun & rhs)
	: CComponent(rhs)
{
}

HRESULT CCom_Gun::CreateBullet_Target(_uint sceneid, _uint count)
{
	for (int i=0;i<count;i++)
	{
		GetSingle(CGameInstance)->Add_GameObject_To_Layer(sceneid, TAG_LAY(Layer_Bullet), TAG_OP(Prototype_Bullet), nullptr);
	}


	return S_OK;
}

HRESULT CCom_Gun::Update_Bullet_MoveTarget(_uint sceneid,_float3 targetPos,_float DeltaTime)
{
	// 날아가게 설정
	list<CGameObject*>* bulletList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(sceneid, TAG_LAY(Layer_Bullet));
	for (auto obj : *bulletList)
	{
		CTransform* trans = (CTransform*)obj->Get_Component(TAG_COM(Com_Transform));
		_float3 curpos = trans->Get_MatrixState(CTransform::STATE_POS);
		_float3 dir = targetPos - curpos;
		dir = dir.Get_Nomalize();
		trans->MovetoTarget(dir, DeltaTime);
	}

	return S_OK;
}

HRESULT CCom_Gun::DestoryBullet_All(_uint sceneid)
{
	// 날아가게 설정
	list<CGameObject*>* bulletList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(sceneid, TAG_LAY(Layer_Bullet));
	for (auto obj : *bulletList)
	{
		obj->DIED();
	}

	return S_OK;
}

HRESULT CCom_Gun::Initialize_Prototype(void * pArg)
{
	return S_OK;
}

HRESULT CCom_Gun::Initialize_Clone(void * pArg)
{
	return S_OK;
}

CCom_Gun * CCom_Gun::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CCom_Gun* pInstance = new CCom_Gun(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Create CCom_Gun Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCom_Gun::Clone(void * pArg)
{
	CCom_Gun* pInstance = new CCom_Gun((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Create CCom_Gun");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCom_Gun::Free()
{
}
