#include "stdafx.h"
#include "..\public\BossParttern.h"
#include "..\public\BossMonster.h"
#include "Camera_Main.h"


_float3 CBoss_Action_Move::EaseingFloat3(EasingTypeID id, _float3 StartPos, _float3 EndPos, float curTime, float maxTime)
{
	_float3 newPos = _float3(0, 0, 0);
	newPos.x = GetSingle(CGameInstance)->Easing(id, StartPos.x, EndPos.x, curTime, maxTime);
	newPos.y = GetSingle(CGameInstance)->Easing(id, StartPos.y, EndPos.y, curTime, maxTime);
	newPos.z = GetSingle(CGameInstance)->Easing(id, StartPos.z, EndPos.z, curTime, maxTime);
	return newPos;

}

CBoss_Action_Move::CBoss_Action_Move(Action_Move_Desc desc)
	:IAction()
{
	// 패턴 정보 저장
	m_isActionEnd = false;
	memcpy(&mDesc,&desc,sizeof(Action_Move_Desc));
	
}

bool CBoss_Action_Move::InitAction()
{
	if (__super::InitAction() == false)
		return false;

	if (mDesc.mMonsterObject == nullptr)
		return false;

	mStartPos = mDesc.mMonsterObject->GetPos(); 	
	mEndPos = mDesc.mMonsterObject->GetScreenToWorld(mDesc.mEndScreenPos);
	mCurrentTimer = 0;

	return false;
	
}

void CBoss_Action_Move::Action(float timeDelta)
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
	_float3 newPos = EaseingFloat3(mDesc.mEasingType, mStartPos, mEndPos, mCurrentTimer, mDesc.mTimerMax);
	mDesc.mMonsterObject->SetPos(newPos);
	
}

CBoss_Pattern_Attack::CBoss_Pattern_Attack(Action_Attack_Desc desc)
{
	m_isActionEnd = false;
	memcpy(&mDesc, &desc, sizeof(Action_Attack_Desc));

}

bool CBoss_Pattern_Attack::InitAction()
{
	if (__super::InitAction() == false)
		return false;

		if (mDesc.mMonsterObject == nullptr)
			return false;

	mCurrentTimer = 0;
	mCurrentAttackCount = 0;

}

void CBoss_Pattern_Attack::Action(float timeDelta)
{
	InitAction();

	if (m_isActionEnd)
		return;

	// 공격 횟수만큼 ID로 총알을 쏜다.
	if (mCurrentAttackCount >= mDesc.mAttackCount)
	{
		m_isActionEnd = true;
		return;
	}


	mCurrentTimer += timeDelta;
	if (mCurrentTimer >= mDesc.mTimerMax)
	{
		mCurrentTimer = 0;
		mCurrentAttackCount++;

		// 타입에 따른 총알 생성 + 업데이트
		switch ( mDesc.meBuelletType)
		{
		case BULLETTYPE_PlayerTarget:
			((CBossMonster*)mDesc.mMonsterObject)->CreateObjectBullet_Target();

			break;
		case BULLETTYPE_Dir:
			((CBossMonster*)mDesc.mMonsterObject)->CreateObjectBullet_Target_Dir(mDesc.mDir);
			break;

		case BULLETTYPE_CamDir:
			mDesc.mDir = mDesc.mCameraMain->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_RIGHT);
			((CBossMonster*)mDesc.mMonsterObject)->CreateObjectBullet_Target_Dir(mDesc.mDir, mDesc.mBulletSpeed);
		default:
			break;
		}
	}


}
