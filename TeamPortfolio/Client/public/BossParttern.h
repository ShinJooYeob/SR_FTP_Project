
// ���� �ൿ ���� ���� �Լ�
#include "MonsterParent.h"
#include "Base.h"

// ������ ������ �ൿ���� �������� ����.

BEGIN(Client)

// #TODO ���� ����

// �ൿ �������̽�
class IAction : public CBase
{
protected:
	bool m_isActionEnd;
	bool m_isActionInit;

public:
	explicit IAction()
	{
		m_isActionEnd = false; m_isActionInit = false;
	}

	virtual bool InitAction()
	{
		if (m_isActionInit == false)
		{
			m_isActionInit = true;
			return true;
		}

		return false;		
	}

	// �����Ӱ� ���ݿ� ���� ���� ���� ����
	virtual void Action(float timeDelta)PURE;
	bool IsPatternEnd() { return m_isActionEnd; }


	virtual void Free() PURE;
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
		_float2 mEndScreenPos;
		_float3 mEndPos;
		CMonsterParent* mMonsterObject;
		float mCurrentTimer;
		float mTimerMax;
		EasingTypeID mEasingType;

	};
	Action_Move_Desc mDesc;

public:
	// �����ڿ��� ���� ���� �ޱ�
	explicit CBoss_Action_Move(Action_Move_Desc desc);
	
	virtual bool InitAction()override;
	// Pattern��(��) ���� ��ӵ�
	virtual void Action(float timeDelta) override;

	static _float3 EaseingFloat3(EasingTypeID id, _float3 StartPos, _float3 EndPos, float curTime, float maxTime);


	virtual void Free()override;

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
	virtual void Action(float timeDelta) override;
};



// �׷α�
class CBoss_Pattern_Groggy : public IAction
{
public:
	// Pattern��(��) ���� ��ӵ�
	virtual void Action(float timeDelta) override;
};


END