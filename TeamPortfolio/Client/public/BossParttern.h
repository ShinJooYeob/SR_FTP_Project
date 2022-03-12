
// 보스 행동 패턴 가상 함수
#include "Base.h"
#include "Com_Gun.h"

// 패턴은 단적인 행동들의 집합으로 구현.

BEGIN(Client)

// #TODO 몬스터 패턴


// 행동 인터페이스
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

	// 움직임과 공격에 대한 패턴 가상 정의
	virtual void Action(float timeDelta)PURE;
	bool IsPatternEnd() { return m_isActionEnd; }

	virtual void Free() {}
};

//////////////// 움직이는 패턴 //////////////////////////////////////

class CBoss_Action_Move_Local : public IAction
{
	// 목표 위치
	// 도달 시간
	// 이징 타입
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
	// 생성자에서 패턴 정보 받기
	explicit CBoss_Action_Move_Local(Action_MoveLocal_Desc desc);
	
	virtual bool InitAction()override;
	// Pattern을(를) 통해 상속됨
	virtual void Action(float timeDelta) override;

	static _float3 EaseingFloat3(EasingTypeID id, _float3 StartPos, _float3 EndPos, float curTime, float maxTime);
	static _float3 EaseingFloat2(EasingTypeID id, _float3 StartPos, _float3 EndPos, float curTime, float maxTime);

};

// 회전 패턴
//class CBoss_Pattern_Move_Rotation : public IAction
//{
//public:
//	// 생성자에서 패턴 정보 받기
//	explicit CBoss_Pattern_Move_Rotation(int desc);
//
//	virtual bool InitAction()override;
//	// Pattern을(를) 통해 상속됨
//	virtual void Action(float timeDelta) override;
//};


// Dir로 Force로 날라감
//class CBoss_Pattern_Move_DirMove : public IAction
//{
//private:
//	class CCom_Gun* mGunComponent;
//	_float3 mStartPosition;
//	_float	mCurrentTimer;
//	_int	mCurrentAttackCount;
//
//public:
//	// 생성자에서 패턴 정보 받기
//	explicit CBoss_Pattern_Move_DirMove(int desc);
//
//	virtual bool InitAction()override;
//	// Pattern을(를) 통해 상속됨
//	virtual void Action(float timeDelta) override;
//};



//////////////// 공격 패턴 //////////////////////////////////////
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
	// 생성자에서 패턴 정보 받기
	explicit CBoss_Pattern_Attack_WorldDir(Action_BulletCommon desc);

	virtual bool InitAction()override;
	// Pattern을(를) 통해 상속됨
	virtual void Action(float timeDelta) override;
};

// 로컬 방향으로 
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
	// 생성자에서 패턴 정보 받기
	explicit CBoss_Pattern_Attack_LocalDir(Action_BulletCommon desc);

	virtual bool InitAction()override;
	// Pattern을(를) 통해 상속됨
	virtual void Action(float timeDelta) override;
};

// 플레이어 위치로 LEFT / RIGHT 한방향
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
	// 생성자에서 패턴 정보 받기
	explicit CBoss_Pattern_Attack_PlayerTarget(Action_BulletCommon desc);

	virtual bool InitAction()override;
	// Pattern을(를) 통해 상속됨
	virtual void Action(float timeDelta) override;

	void SetTarget(class CTransform* com)
	{
		mPlayerTransformComponent = com;
	}
};

///////////////////////// 기타 행동 /////////////////////////
class CBoss_Pattern_Dealy : public IAction
{
public:
private:	
	_float	mDealyTimer;
	_float	mCurrentTimer;
	
public:
	// 생성자에서 패턴 정보 받기
	explicit CBoss_Pattern_Dealy(int DealyTime);

	virtual bool InitAction()override;
	// Pattern을(를) 통해 상속됨
	virtual void Action(float timeDelta) override;

};




END