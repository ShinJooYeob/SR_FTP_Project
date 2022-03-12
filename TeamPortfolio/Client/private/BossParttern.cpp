#include "stdafx.h"
#include "..\public\BossParttern.h"
#include "..\public\BossMonster.h"
#include "Camera_Main.h"


_float3 CBoss_Action_Move_Local::EaseingFloat3(EasingTypeID id, _float3 StartPos, _float3 EndPos, float curTime, float maxTime)
{
	_float3 newPos = _float3(0, 0, 0);
	newPos.x = GetSingle(CGameInstance)->Easing(id, StartPos.x, EndPos.x, curTime, maxTime);
	newPos.y = GetSingle(CGameInstance)->Easing(id, StartPos.y, EndPos.y, curTime, maxTime);
	newPos.z = GetSingle(CGameInstance)->Easing(id, StartPos.z, EndPos.z, curTime, maxTime);
	return newPos;

}

_float3 CBoss_Action_Move_Local::EaseingFloat2(EasingTypeID id, _float3 StartPos, _float3 EndPos, float curTime, float maxTime)
{
	_float3 newPos = _float3(0, 0, 0);
	newPos.x = GetSingle(CGameInstance)->Easing(id, StartPos.x, EndPos.x, curTime, maxTime);
	newPos.y = GetSingle(CGameInstance)->Easing(id, StartPos.y, EndPos.y, curTime, maxTime);
	return newPos;
}

CBoss_Action_Move_Local::CBoss_Action_Move_Local(Action_MoveLocal_Desc desc)
	:IAction()
{
	// 패턴 정보 저장
	m_isActionEnd = false;
	memcpy(&mDesc,&desc,sizeof(Action_MoveLocal_Desc));
	
}

bool CBoss_Action_Move_Local::InitAction()
{
	if (__super::InitAction() == false)
		return false;

	if (mDesc.mMonsterObject == nullptr)
		return false;

	mStartLocalPos = mDesc.mMonsterObject->GetLocalPos();
	mCurrentTimer = 0;

	return false;
	
}

void CBoss_Action_Move_Local::Action(float timeDelta)
{
	InitAction();

	if (m_isActionEnd)
		return;

	// 해당 방향으로 움직이는 패턴
	if (mCurrentTimer > mDesc.mTimerMax)
	{
		m_isActionEnd = true;
		return;

	}
	mCurrentTimer += timeDelta;
	_float3 NewLocalPosition = EaseingFloat2(mDesc.mEasingType, mStartLocalPos, mDesc.mEndLocalPos, mCurrentTimer, mDesc.mTimerMax);
	mDesc.mMonsterObject->SetLocalPos(NewLocalPosition);
	
}

CBoss_Pattern_Attack_WorldDir::CBoss_Pattern_Attack_WorldDir(Action_BulletCommon desc)
{
	m_isActionEnd = false;
	memcpy(&mDesc, &desc, sizeof(Action_BulletCommon));

}

bool CBoss_Pattern_Attack_WorldDir::InitAction()
{
	if (__super::InitAction() == false)
		return false;

	if (mDesc.mMonsterObject == nullptr)
		return false;

	mCurrentTimer = 0;
	mCurrentAttackCount = 0;
	mStartPosition = mDesc.mMonsterObject->GetPos();
	mGunComponent = (CCom_Gun*)mDesc.mMonsterObject->Get_Component(TAG_COM(Com_Gun));
	return false;

}

void CBoss_Pattern_Attack_WorldDir::Action(float timeDelta)
{
	InitAction();

	if (m_isActionEnd)
		return;

	if (mCurrentAttackCount >= mDesc.mAttackCount)
	{
		m_isActionEnd = true;
		return;
	}
	if (mGunComponent == nullptr)
	{
		m_isActionEnd = true;
		return;
	}

	mStartPosition = mDesc.mBulletSpawnOffset;

	mCurrentTimer += timeDelta;
	if (mCurrentTimer >= mDesc.mDealyTimeMax)
	{
		mCurrentTimer = 0;
		mCurrentAttackCount++;

		mGunComponent->CreateBullet_Dir(
			mStartPosition,
			mDesc.mEndPos,
			mDesc.mBulletSpeed,
			mDesc.meBulletType_Move);
	}


}

CBoss_Pattern_Attack_LocalDir::CBoss_Pattern_Attack_LocalDir(Action_BulletCommon desc)
{
	m_isActionEnd = false;
	memcpy(&mDescBullet, &desc, sizeof(Action_BulletCommon));
}

bool CBoss_Pattern_Attack_LocalDir::InitAction()
{
	if (__super::InitAction() == false)
		return false;

	if (mDescBullet.mMonsterObject == nullptr)
		return false;

	mCurrentTimer = 0;
	mCurrentAttackCount = 0;
	mStartPosition = mDescBullet.mBulletSpawnOffset;
	mGunComponent = (CCom_Gun*)mDescBullet.mMonsterObject->Get_Component(TAG_COM(Com_Gun));
	return false;

}

void CBoss_Pattern_Attack_LocalDir::Action(float timeDelta)
{
	InitAction();

	if (m_isActionEnd)
		return;

	if (mCurrentAttackCount >= mDescBullet.mAttackCount)
	{
		m_isActionEnd = true;
		return;
	}
	if (mGunComponent == nullptr)
	{
		m_isActionEnd = true;
		return;
	}

	// 각도 계산
	mStartPosition = mDescBullet.mBulletSpawnOffset;


	_float3 RightVec = mDescBullet.mMonsterObject->GetTransform()->Get_MatrixState(CTransform::STATE_RIGHT);
	_float3 LookVec = mDescBullet.mMonsterObject->GetTransform()->Get_MatrixState(CTransform::STATE_LOOK);

	_float rad = D3DXToRadian(mAngle);

	_Matrix AxisRotateMatrix;
	D3DXMatrixRotationAxis(&AxisRotateMatrix, &LookVec, rad);
	RightVec = RightVec.DirVector_Matrix(AxisRotateMatrix);


	mDescBullet.mEndPos = RightVec;

	mCurrentTimer += timeDelta;
	if (mCurrentTimer >= mDescBullet.mDealyTimeMax)
	{
		mCurrentTimer = 0;
		mCurrentAttackCount++;

		mGunComponent->CreateBullet_Dir(
			mStartPosition,
			mDescBullet.mEndPos,
			mDescBullet.mBulletSpeed,
			mDescBullet.meBulletType_Move);
	}

}

CBoss_Pattern_Attack_PlayerTarget::CBoss_Pattern_Attack_PlayerTarget(Action_BulletCommon desc)
{
	m_isActionEnd = false;
	memcpy(&mDescBullet, &desc, sizeof(Action_BulletCommon));
}

bool CBoss_Pattern_Attack_PlayerTarget::InitAction()
{
	if (__super::InitAction() == false)
		return false;

	if (mDescBullet.mMonsterObject == nullptr)
		return false;
	
	mCurrentTimer = 0;
	mCurrentAttackCount = 0;
	mStartPosition = mDescBullet.mBulletSpawnOffset;
	mGunComponent = (CCom_Gun*)mDescBullet.mMonsterObject->Get_Component(TAG_COM(Com_Gun));
	return false;
}

void CBoss_Pattern_Attack_PlayerTarget::Action(float timeDelta)
{
	InitAction();
	
	if (mCurrentAttackCount > mDescBullet.mAttackCount)
	{
		m_isActionEnd = true;
		return;
	}

	if (mGunComponent == nullptr ||
		mPlayerTransformComponent == nullptr)
	{
		m_isActionEnd = true;
		return;
	}

	// 플레이어 타겟 위치에 따라 LEFT / RIGHT 중 하나만 쏜다.
	_float3 PlayerPosition = mPlayerTransformComponent->Get_MatrixState(CTransform::STATE_POS);
	_float3 Position = mDescBullet.mMonsterObject->GetPos();
	_float3 EndVec = PlayerPosition - Position;

	EndVec = EndVec.Get_Nomalize();
	if (EndVec.x < 0)
		// Left
	{
		mDescBullet.mEndPos = LEFTVEC;
	}
	else
	{
		// Right
		mDescBullet.mEndPos = RIGHTVEC;
	}

	mCurrentTimer += timeDelta;
	if (mCurrentTimer >= mDescBullet.mDealyTimeMax)
	{
		mCurrentTimer = 0;
		mCurrentAttackCount++;

		mGunComponent->CreateBullet_Dir(
			mStartPosition,
			mDescBullet.mEndPos,
			mDescBullet.mBulletSpeed,
			mDescBullet.meBulletType_Move);
	}




}

CBoss_Pattern_Dealy::CBoss_Pattern_Dealy(int DealyTime)
{
	mDealyTimer = DealyTime;
	m_isActionEnd = false;
}

bool CBoss_Pattern_Dealy::InitAction()
{
	if (__super::InitAction() == false)
		return false;
	mCurrentTimer = 0;
	
	return false;
}

void CBoss_Pattern_Dealy::Action(float timeDelta)
{
	InitAction();

	if (m_isActionEnd)
		return;

	mCurrentTimer += timeDelta;
	if (mCurrentTimer >= mDealyTimer)
		m_isActionEnd = true;
	return;
}
