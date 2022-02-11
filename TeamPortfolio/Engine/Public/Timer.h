#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer final : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer() = default;
public:
	_float Get_DeltaTime(void);


private:

	LARGE_INTEGER		m_CurrentTime;				//�Լ��� ���۵� ���� ���� �ð��� �޾ƿ� ����
	LARGE_INTEGER		m_OldTime;					//���� �������� �ð��� �����س��� ����
	LARGE_INTEGER		m_OriginTime;				//���ļ� ���� Ÿ�̹��� ������ ����
	LARGE_INTEGER		m_CpuTick;					//CPU ���ļ��� ��ȯ => 1�ʴ� PerformanceCounter�� �ö󰡴� ȸ���� ��ȯ���ִ� �� 

	_float				m_fDeltaTime;

public:
	static CTimer* Create();
	virtual void Free() override;
};

END

/*

Ŭ���̶� ��ǻ�ʹ� ���� ���ڸ� ����� CPU ���� ��� ���� ��ǰ���� �۾��ϴµ�
�� ���ڸ� ���� Ȥ�� Ŭ�� ƽ�̶�� �θ��� �������� �������� ��ǰ�� ����Ǿ� �ִµ�
���κ����� Ŭ���� ƽ�� ���������� �����͸� �ۼ����Ѵ�.
//https://brunch.co.kr/@younggiseo/190
//http://egloos.zum.com/NeoTrinity/v/943773

//QueryPerformanceCounter �� CPU�� OS�� ���۵ǰ������� ���ݱ��� ��� �����ߴ����� �޾ƿ´�.
//�׷��� old���� now�� ���� ���� �����ӿ��� ���� �����ӻ��̿� �� ����� �����ߴ����� �� �� �ִ°��̴�.

//QueryPerformanceFrequency �� CPU�� ���� �ֱ�(�����ϴ� �ð� ����)�� �� ƽ�� �ð� ������ �޾ƿ��� ��.
//								�� CPU�� ���� �ֱ⸦ �޾ƿ��� ���̱� ������ �ش� ��⿡�� üũ�� �� �ִ� �ּ� �ð� �����̴�.
*/