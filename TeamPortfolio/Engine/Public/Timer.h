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

	LARGE_INTEGER		m_CurrentTime;				//함수가 시작될 때의 현제 시각을 받아올 변수
	LARGE_INTEGER		m_OldTime;					//이전 프레임의 시간을 저장해놓는 변수
	LARGE_INTEGER		m_OriginTime;				//주파수 갱신 타이밍을 저장할 변수
	LARGE_INTEGER		m_CpuTick;					//CPU 주파수를 반환 => 1초당 PerformanceCounter가 올라가는 회수를 반환해주는 것 

	_float				m_fDeltaTime;

public:
	static CTimer* Create();
	virtual void Free() override;
};

END

/*

클록이란 컴퓨터는 일정 박자를 만들어 CPU 안의 모든 구성 부품들이 작업하는데
이 박자를 펄즈 혹은 클록 틱이라고 부르며 버스에서 여러개의 부품이 연결되어 있는데
메인보드의 클록이 틱을 보낼때마다 데이터를 송수신한다.
//https://brunch.co.kr/@younggiseo/190
//http://egloos.zum.com/NeoTrinity/v/943773

//QueryPerformanceCounter 는 CPU가 OS가 시작되고나서부터 지금까지 몇번 진동했는지를 받아온다.
//그래서 old에서 now를 빼면 이전 프레임에서 현재 프레임사이에 총 몇번을 진동했는지를 알 수 있는것이다.

//QueryPerformanceFrequency 는 CPU의 진동 주기(진동하는 시간 간격)를 즉 틱의 시간 간격을 받아오는 것.
//								즉 CPU의 펄즈 주기를 받아오는 것이기 때문에 해당 기기에서 체크할 수 있는 최소 시간 간격이다.
*/