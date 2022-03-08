
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

// 움직임 패턴
class CBoss_Action_Move : public IAction
{
	// 목표 위치
	// 도달 시간
	// 이징 타입
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
	// 생성자에서 패턴 정보 받기
	explicit CBoss_Action_Move(Action_Move_Desc desc);
	
	virtual bool InitAction()override;
	// Pattern을(를) 통해 상속됨
	virtual void Action(float timeDelta) override;

	static _float3 EaseingFloat3(EasingTypeID id, _float3 StartPos, _float3 EndPos, float curTime, float maxTime);



};


// 공격
class CBoss_Pattern_Attack : public IAction
{
	// 총알 방향
	// 총알 타입
	// 총알 개수

public:
	typedef struct Action_Attack_Desc
	{
		_uint mAttackCount; // 공격 횟수
		CCom_Gun::E_BulletType meBuelletType;
		CCom_Gun * mCom_Gun;
		float mTimerMax;

	};

private:
	Action_Attack_Desc mDesc;
	_float	mCurrentTimer;
	_int	mCurrentAttackCount;

public:
	// 생성자에서 패턴 정보 받기
	explicit CBoss_Pattern_Attack(Action_Attack_Desc desc);

	virtual bool InitAction()override;
	// Pattern을(를) 통해 상속됨
	virtual void Action(float timeDelta) override;

};

// 회전 패턴


// 날아가는 패턴




// 그로기
class CBoss_Pattern_Groggy : public IAction
{
public:
	// Pattern을(를) 통해 상속됨
	virtual void Action(float timeDelta) override;
};


END