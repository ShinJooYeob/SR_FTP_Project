#include "stdafx.h"
#include "..\public\BossParttern.h"

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

	mDesc.mStartPos = mDesc.mMonsterObject->GetPos(); 
	mDesc.mCurrentTimer = 0;

	return false;
	
}

void CBoss_Action_Move::Action(float timeDelta)
{
	InitAction();

	if (m_isActionEnd)
		return;

	// 해당 방향으로 움직이는 패턴
	if (mDesc.mCurrentTimer > mDesc.mTimerMax)
	{
		m_isActionEnd = true;
		return;

	}
	mDesc.mCurrentTimer += timeDelta;
	_float3 newPos = EaseingFloat3(mDesc.mEasingType, mDesc.mStartPos, mDesc.mEndPos, mDesc.mCurrentTimer, mDesc.mTimerMax);
	mDesc.mMonsterObject->SetPos(newPos);
	
}

