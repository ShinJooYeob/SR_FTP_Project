#include "stdafx.h"
#include "..\public\BossParttern.h"

CBoss_Action_Move::CBoss_Action_Move(Action_Move_Desc desc)
{
	// ���� ���� ����
	m_isActionEnd = false;
	memcpy(&mDesc,&desc,sizeof(Action_Move_Desc));
}

void CBoss_Action_Move::Action()
{
	if (m_isActionEnd)
		return;

	// �ش� �������� �����̴� ����


}
