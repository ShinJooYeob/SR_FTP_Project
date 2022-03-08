#include "stdafx.h"
#include "..\public\BossParttern.h"

CBoss_Action_Move::CBoss_Action_Move(Action_Move_Desc desc)
{
	// 패턴 정보 저장
	m_isActionEnd = false;
	memcpy(&mDesc,&desc,sizeof(Action_Move_Desc));
}

void CBoss_Action_Move::Action()
{
	if (m_isActionEnd)
		return;

	// 해당 방향으로 움직이는 패턴


}
