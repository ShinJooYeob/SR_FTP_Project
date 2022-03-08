
// 보스 행동 패턴 가상 함수
#include "MonsterParent.h"

// 패턴은 단적인 행동들의 집합으로 구현.

BEGIN(Client)

// 행동 인터페이스
class IAction
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
		_float3 mStartPos;
		_float3 mEndPos;
		CMonsterParent* mMonsterObject;
		float mCurrentTimer;
		float mTimerMax;
		EasingTypeID mEasingType;

	};

	Action_Move_Desc mDesc;
public:
	// 생성자에서 패턴 정보 받기
	explicit CBoss_Action_Move(Action_Move_Desc desc);
	
	virtual bool InitAction()override;
	// Pattern을(를) 통해 상속됨
	virtual void Action(float timeDelta) override;

	static _float3 EaseingFloat3(EasingTypeID id, _float3 StartPos, _float3 EndPos, float curTime, float maxTime);
};



// 회전 패턴


// 날아가는 패턴


// 공격
class CBoss_Pattern_Attack : public IAction
{
	// 총알 방향
	// 총알 타입
	// 총알 개수

public:
	// Pattern을(를) 통해 상속됨
	virtual void Action(float timeDelta) override;
};



// 그로기
class CBoss_Pattern_Groggy : public IAction
{
public:
	// Pattern을(를) 통해 상속됨
	virtual void Action(float timeDelta) override;
};


END