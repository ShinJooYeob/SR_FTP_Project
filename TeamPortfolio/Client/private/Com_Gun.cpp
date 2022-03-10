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

HRESULT CCom_Gun::CreateBullet_Target(_float3 startPos, _float3 targetPos, _float speed, E_BulletType_MOVE type)
{

	CBullet::BULLETDESC desc;
	desc.BulletType = type;

	_float3 TargetPos = startPos - targetPos;
	TargetPos = TargetPos.Get_Nomalize();
	desc.MoveDir = TargetPos;
	desc.StartPos = startPos;
	desc.BulletSpeed = speed;

	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(mDesc.mSceneID, TAG_LAY(Layer_Bullet), TAG_OP(Prototype_Bullet), &desc));

	return S_OK;
}

HRESULT CCom_Gun::CreateBullet_Dir(_float3 startPos, _float3 moveidr, _float speed, E_BulletType_MOVE type)
{
	CBullet::BULLETDESC desc;
	desc.BulletType = type;
	desc.MoveDir = moveidr;
	desc.StartPos = startPos;
	desc.BulletSpeed = speed;

	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(mDesc.mSceneID, TAG_LAY(Layer_Bullet), TAG_OP(Prototype_Bullet), &desc));

	return S_OK;

}

HRESULT CCom_Gun::DestoryBullet_All()
{
	// 날아가게 설정
	list<CGameObject*>* bulletList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(mDesc.mSceneID, TAG_LAY(Layer_Bullet));
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
	if (pArg != nullptr)
	{
		memcpy(&mDesc, pArg, sizeof(tagGunDesc));		
	}

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
