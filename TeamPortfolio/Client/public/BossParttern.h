
// ���� �ൿ ���� ���� �Լ�
#include "MonsterParent.h"

// ������ ������ �ൿ���� �������� ����.

BEGIN(Client)

// �ൿ �������̽�
class IPattern 
{
public:
	// �����Ӱ� ���ݿ� ���� ���� ���� ����
	virtual void Partern()PURE;
	//virtual void Partern()PURE;
	//virtual bool Partern()PURE;
};

// �⺻ ������ 
class CBoss_Pattern_Move1 : public IPattern
{
public:
	// Pattern��(��) ���� ��ӵ�
	virtual void Partern() override;

};
class CBoss_Pattern_Move2 : public IPattern
{
public:
	// Pattern��(��) ���� ��ӵ�
	virtual void Partern() override;
};

// �⺻ ����
class CBoss_Pattern_Attack1 : public IPattern
{
public:
	// Pattern��(��) ���� ��ӵ�
	virtual void Partern() override;
};

class CBoss_Pattern_Attack2 : public IPattern
{
public:
	// Pattern��(��) ���� ��ӵ�
	virtual void Partern() override;
};

// Ư�� ����

END