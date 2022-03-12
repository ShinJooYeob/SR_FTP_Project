
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

//////////////// �����̴� ���� //////////////////////////////////////

class CBoss_Action_Move_Local : public IAction
{
	// ��ǥ ��ġ
	// ���� �ð�
	// ��¡ Ÿ��
public:
	typedef struct Action_MoveLocal_Desc
	{
		_float3 mEndLocalPos;
		class CBossMonster* mMonsterObject;
		float mTimerMax;
		EasingTypeID mEasingType;
	};

private:
	Action_MoveLocal_Desc mDesc;
	_float3 mStartLocalPos;
	float mCurrentTimer;

public:
	// �����ڿ��� ���� ���� �ޱ�
	explicit CBoss_Action_Move_Local(Action_MoveLocal_Desc desc);
	
	virtual bool InitAction()override;
	// Pattern��(��) ���� ��ӵ�
	virtual void Action(float timeDelta) override;

	static _float3 EaseingFloat3(EasingTypeID id, _float3 StartPos, _float3 EndPos, float curTime, float maxTime);
	static _float3 EaseingFloat2(EasingTypeID id, _float3 StartPos, _float3 EndPos, float curTime, float maxTime);

};

// ȸ�� ����
//class CBoss_Pattern_Move_Rotation : public IAction
//{
//public:
//	// �����ڿ��� ���� ���� �ޱ�
//	explicit CBoss_Pattern_Move_Rotation(int desc);
//
//	virtual bool InitAction()override;
//	// Pattern��(��) ���� ��ӵ�
//	virtual void Action(float timeDelta) override;
//};


// Dir�� Force�� ����
//class CBoss_Pattern_Move_DirMove : public IAction
//{
//private:
//	class CCom_Gun* mGunComponent;
//	_float3 mStartPosition;
//	_float	mCurrentTimer;
//	_int	mCurrentAttackCount;
//
//public:
//	// �����ڿ��� ���� ���� �ޱ�
//	explicit CBoss_Pattern_Move_DirMove(int desc);
//
//	virtual bool InitAction()override;
//	// Pattern��(��) ���� ��ӵ�
//	virtual void Action(float timeDelta) override;
//};



//////////////// ���� ���� //////////////////////////////////////
typedef struct Action_BulletCommon
{
	_uint mAttackCount; 
	_uint mBulletSpeed;
	_float3 mBulletSpawnOffset;
	_float3 mEndPos;
	E_BulletType_MOVE	meBulletType_Move;
	float mDealyTimeMax;
	class CBossMonster* mMonsterObject;

};

class CBoss_Pattern_Attack_WorldDir : public IAction
{
private:
	Action_BulletCommon mDesc;

	class CCom_Gun* mGunComponent;
	_float3 mStartPosition;
	_float	mCurrentTimer;
	_int	mCurrentAttackCount;

public:
	// �����ڿ��� ���� ���� �ޱ�
	explicit CBoss_Pattern_Attack_WorldDir(Action_BulletCommon desc);

	virtual bool InitAction()override;
	// Pattern��(��) ���� ��ӵ�
	virtual void Action(float timeDelta) override;
};

// ���� �������� 
class CBoss_Pattern_Attack_LocalDir : public IAction
{
public:

	_float mAngle;
private:
	Action_BulletCommon mDescBullet;

	class CCom_Gun* mGunComponent;
	_float3 mStartPosition;
	_float	mCurrentTimer;
	_int	mCurrentAttackCount;

public:
	// �����ڿ��� ���� ���� �ޱ�
	explicit CBoss_Pattern_Attack_LocalDir(Action_BulletCommon desc);

	virtual bool InitAction()override;
	// Pattern��(��) ���� ��ӵ�
	virtual void Action(float timeDelta) override;
};

// �÷��̾� ��ġ�� LEFT / RIGHT �ѹ���
class CBoss_Pattern_Attack_PlayerTarget : public IAction
{
private:
	Action_BulletCommon mDescBullet;

	
	class CCom_Gun* mGunComponent;
	class CTransform* mPlayerTransformComponent;

	_float3 mStartPosition;
	_float	mCurrentTimer;
	_int	mCurrentAttackCount;

public:
	// �����ڿ��� ���� ���� �ޱ�
	explicit CBoss_Pattern_Attack_PlayerTarget(Action_BulletCommon desc);

	virtual bool InitAction()override;
	// Pattern��(��) ���� ��ӵ�
	virtual void Action(float timeDelta) override;

	void SetTarget(class CTransform* com)
	{
		mPlayerTransformComponent = com;
	}
};

///////////////////////// ��Ÿ �ൿ /////////////////////////
class CBoss_Pattern_Dealy : public IAction
{
public:
private:	
	_float	mDealyTimer;
	_float	mCurrentTimer;
	
public:
	// �����ڿ��� ���� ���� �ޱ�
	explicit CBoss_Pattern_Dealy(int DealyTime);

	virtual bool InitAction()override;
	// Pattern��(��) ���� ��ӵ�
	virtual void Action(float timeDelta) override;

};




END