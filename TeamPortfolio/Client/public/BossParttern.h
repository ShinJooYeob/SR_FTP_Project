
// ���� �ൿ ���� ���� �Լ�
#include "Base.h"
#include "Com_Gun.h"

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


	virtual void Free() {}
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
		_float2 mEndScreenPos;
		class CMonsterParent* mMonsterObject;
		float mTimerMax;
		EasingTypeID mEasingType;

	};

private:
	Action_Move_Desc mDesc;
	_float3 mStartPos;
	float mCurrentTimer;
	_float3 mEndPos;

public:
	// �����ڿ��� ���� ���� �ޱ�
	explicit CBoss_Action_Move(Action_Move_Desc desc);
	
	virtual bool InitAction()override;
	// Pattern��(��) ���� ��ӵ�
	virtual void Action(float timeDelta) override;

	static _float3 EaseingFloat3(EasingTypeID id, _float3 StartPos, _float3 EndPos, float curTime, float maxTime);



};


// ����
class CBoss_Pattern_Attack : public IAction
{
	// �Ѿ� ����
	// �Ѿ� Ÿ��
	// �Ѿ� ����

public:
	typedef struct Action_Attack_Desc
	{
		_uint mAttackCount; // ���� Ƚ��
		CCom_Gun::E_BulletType meBuelletType;
		CCom_Gun * mCom_Gun;
		float mTimerMax;

	};

private:
	Action_Attack_Desc mDesc;
	_float	mCurrentTimer;
	_int	mCurrentAttackCount;

public:
	// �����ڿ��� ���� ���� �ޱ�
	explicit CBoss_Pattern_Attack(Action_Attack_Desc desc);

	virtual bool InitAction()override;
	// Pattern��(��) ���� ��ӵ�
	virtual void Action(float timeDelta) override;

};

// ȸ�� ����


// ���ư��� ����




// �׷α�
class CBoss_Pattern_Groggy : public IAction
{
public:
	// Pattern��(��) ���� ��ӵ�
	virtual void Action(float timeDelta) override;
};


END