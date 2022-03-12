#include "stdafx.h"
#include "..\public\Com_Gun.h"
#include "BossMonster.h"

CCom_Gun::CCom_Gun(LPDIRECT3DDEVICE9 pGraphicDevice)
 : CComponent(pGraphicDevice)
{
}

CCom_Gun::CCom_Gun(const CCom_Gun & rhs)
	: CComponent(rhs)
{
}

void CCom_Gun::Set_MonsterObject(CBossMonster * monster)
{	
	mBossMonster = monster;	
}

HRESULT CCom_Gun::CreateBullet_Target(_float3 startOffset, _float3 targetPos, _float speed, E_BulletType_MOVE type)
{

	CBullet::BULLETDESC desc;
	desc.BulletType = type;

	_float3 TargetPos = startOffset - targetPos;
	TargetPos = TargetPos.Get_Nomalize();
	desc.MoveDir = TargetPos;
	desc.StartOffset = startOffset;
	desc.BulletSpeed = speed;

	Push_BackBulletList(desc);


	return S_OK;
}

HRESULT CCom_Gun::CreateBullet_Dir(_float3 startOffset, _float3 moveidr, _float speed, E_BulletType_MOVE type)
{
	CBullet::BULLETDESC desc;
	desc.BulletType = type;
	desc.MoveDir = moveidr;
	desc.StartOffset = startOffset;
	desc.BulletSpeed = speed;

	// ���⼭�� ������ �߰�
	Push_BackBulletList(desc);


	return S_OK;

}

HRESULT CCom_Gun::DestoryBullet_All()
{
	// ���ư��� ����
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

	mListBullets.clear();
	mStateGun = CCom_Gun::STATE_GUN_NONE;

	return S_OK;
}

HRESULT CCom_Gun::Update_CreateBullet(float deltatime)
{

	// 1. �ִϸ��̼� ����
	// 2. �ð��� �°� �Ѿ��� ��
	// 3. �ִϸ��̼��� ���������� ��ٸ�

	if (mBossMonster->Get_IsDying())
		return S_OK;

	switch (mStateGun)
	{
	case Client::CCom_Gun::STATE_GUN_NONE:
		if (mListBullets.empty() == false)
			mStateGun = STATE_GUN_START;
		break;
	case Client::CCom_Gun::STATE_GUN_START:
		mBossMonster->Start_AttackAniMaion(13.f);		
		mCurrentTimer = FIX_BulletTime;
		mStateGun = CCom_Gun::STATE_GUN_SHOOT;
		break;

	case Client::CCom_Gun::STATE_GUN_SHOOT:
		mCurrentTimer -= deltatime;
		if (mCurrentTimer<0 )
		{
			mCurrentTimer = FIX_BulletEndTime;
			CBullet::BULLETDESC desc = mListBullets.front();
			mListBullets.pop_front();
			GetSingle(CGameInstance)->PlaySound(TEXT("JH_Boss_Attack0.wav"), CHANNEL_OBJECT);
			// ��� ��ġ �ٽð��

			if (mBossMonster->Get_CameraLookState() == CCamera_Main::CameraLookStateID::Look_Front_Axis)
			{
				_float3 position = mBossMonster->GetPos() + mBossMonster->GetTransform()->Get_MatrixState(CTransform::STATE_RIGHT)*desc.StartOffset.x;
				position += mBossMonster->GetTransform()->Get_MatrixState(CTransform::STATE_UP)*desc.StartOffset.y;
				desc.StartOffset = position;
			}
			else if (mBossMonster->Get_CameraLookState() == CCamera_Main::CameraLookStateID::Look_Back_Axis)
			{

				_float3 position = mBossMonster->GetPos() + mBossMonster->GetTransform()->Get_MatrixState(CTransform::STATE_RIGHT)*(-desc.StartOffset.x);
				position += mBossMonster->GetTransform()->Get_MatrixState(CTransform::STATE_UP)*desc.StartOffset.y;
				desc.StartOffset = position;
			}
			else
				desc.StartOffset = mBossMonster->GetPos();

			FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(mDesc.mSceneID, TAG_LAY(Layer_Bullet), TAG_OP(Prototype_Bullet), &desc));
			mStateGun = CCom_Gun::STATE_GUN_RETIRE;

		}
		break;
	case Client::CCom_Gun::STATE_GUN_RETIRE:
		mCurrentTimer -= deltatime;
		if (mCurrentTimer<0)
		{
			mStateGun = STATE_GUN_NONE;
		}
		break;
	case Client::CCom_Gun::STATE_GUN_END:
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CCom_Gun::Push_BackBulletList(CBullet::BULLETDESC b)
{
	mListBullets.push_back(b);

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

	__super::Free();
}
