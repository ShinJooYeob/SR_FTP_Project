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
	//QueryPerformanceCounter �� CPU ƽ �� CPU�� �����ϴ� �ð� ������ �޾ƿ´�.
	//QueryPerformanceFrequency �� CPU�� ���� �ֱ�(�����ϴ� �ð� ����)�� �޾ƿ´�.


	QueryPerformanceCounter(&m_CurrentTime);	

	//���ļ��� �������� �� 1�ʸ� �Ѿ��� ��� �������ش� �ʹ� ����ϰ� �����ϸ� Ÿ�̹��� ���߱� �ָ��� �� �ֱ� �����ΰ�???
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

