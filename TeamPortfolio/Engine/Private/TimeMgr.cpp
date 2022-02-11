#include "..\Public\TimeMgr.h"
#include "Timer.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
{
}

_float CTimeMgr::Get_DeltaTime(const _tchar * tagTimer)
{
	CTimer*	pTimer = Find_Timer(tagTimer);

	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Get_DeltaTime();
}

HRESULT CTimeMgr::Add_Timer(const _tchar * tagTimer)
{
	if (nullptr != Find_Timer(tagTimer))
		return E_FAIL;

	CTimer*	pTimer = CTimer::Create();
	if (nullptr == pTimer)
		return E_FAIL;

	m_mapTimers.emplace(tagTimer, pTimer);

	return S_OK;
}

CTimer * CTimeMgr::Find_Timer(const _tchar * tagTimer)
{
	auto	iter = find_if(m_mapTimers.begin(), m_mapTimers.end(), CTagFinder(tagTimer));
	if (iter == m_mapTimers.end())
		return nullptr;

	return iter->second;
}

void CTimeMgr::Free()
{
	for (auto& Pair : m_mapTimers) 
		Safe_Release(Pair.second);

	m_mapTimers.clear();
	
}
