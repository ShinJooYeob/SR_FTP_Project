
// ���� �ൿ ���� ���� �Լ�
#include "MonsterParent.h"

// ������ ������ �ൿ���� �������� ����.

BEGIN(Client)

// �ൿ �������̽�
class IAction
{
protected:
	bool m_isActionEnd;

public:
	IAction() { m_isActionEnd = false; }

	// �����Ӱ� ���ݿ� ���� ���� ���� ����
	virtual void Action()PURE;
	bool IsPatternEnd() { return m_isActionEnd; }
};

// ������ ����
class CBoss_Action_Move : public IAction
{
	// ��ǥ ��ġ
	// ���� �ð�
	// ��¡ Ÿ��
public:
	typedef struct Action_Move_Desc
	{
		_float3 mStartPos;
		_float3 mEndPos;
		CTransform* mComTrans;
		float mMoveTime;
		float mTimerMax;
	};

	Action_Move_Desc mDesc;
public:
	// �����ڿ��� ���� ���� �ޱ�
	explicit CBoss_Action_Move(Action_Move_Desc desc);
	
	// Pattern��(��) ���� ��ӵ�
	virtual void Action() override;
};



// ȸ�� ����


// ���ư��� ����


// ����
class CBoss_Pattern_Attack : public IAction
{
	// �Ѿ� ����
	// �Ѿ� Ÿ��
	// �Ѿ� ����

public:
	// Pattern��(��) ���� ��ӵ�
	virtual void Action() override;
};



// �׷α�
class CBoss_Pattern_Groggy : public IAction
{
public:
	// Pattern��(��) ���� ��ӵ�
	virtual void Action() override;
};


END