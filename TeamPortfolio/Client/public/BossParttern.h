
// 보스 행동 패턴 가상 함수
#include "MonsterParent.h"

// 패턴은 단적인 행동들의 집합으로 구현.

BEGIN(Client)

// 행동 인터페이스
class IPattern 
{
public:
	// 움직임과 공격에 대한 패턴 가상 정의
	virtual void Partern()PURE;
	//virtual void Partern()PURE;
	//virtual bool Partern()PURE;
};

// 기본 움직임 
class CBoss_Pattern_Move1 : public IPattern
{
public:
	// Pattern을(를) 통해 상속됨
	virtual void Partern() override;

};
class CBoss_Pattern_Move2 : public IPattern
{
public:
	// Pattern을(를) 통해 상속됨
	virtual void Partern() override;
};

// 기본 공격
class CBoss_Pattern_Attack1 : public IPattern
{
public:
	// Pattern을(를) 통해 상속됨
	virtual void Partern() override;
};

class CBoss_Pattern_Attack2 : public IPattern
{
public:
	// Pattern을(를) 통해 상속됨
	virtual void Partern() override;
};

// 특수 공격

END