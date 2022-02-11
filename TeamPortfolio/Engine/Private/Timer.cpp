#include "Timer.h"

CTimer::CTimer()
	: m_fDeltaTime(0.f)

{
	QueryPerformanceCounter(&m_CurrentTime);
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_OriginTime);
	QueryPerformanceFrequency(&m_CpuTick);
}

_float CTimer::Get_DeltaTime(void)
{
	//QueryPerformanceCounter 는 CPU 틱 즉 CPU가 진동하는 시간 간격을 받아온다.
	//QueryPerformanceFrequency 는 CPU의 진동 주기(진동하는 시간 간격)를 받아온다.


	QueryPerformanceCounter(&m_CurrentTime);	

	//주파수를 생신해준 지 1초를 넘었을 경우 갱신해준다 너무 빈번하게 갱신하면 타이밍을 맞추기 애매할 수 있기 때문인가???
	if (m_CurrentTime.QuadPart - m_OriginTime.QuadPart > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_OriginTime = m_CurrentTime;
	}

	m_fDeltaTime = _float(m_CurrentTime.QuadPart - m_OldTime.QuadPart) / m_CpuTick.QuadPart;

	m_OldTime = m_CurrentTime;

	return m_fDeltaTime;

}

CTimer * CTimer::Create()
{
	CTimer*	pInstance = new CTimer();

	return pInstance;
}

void CTimer::Free()
{
}

