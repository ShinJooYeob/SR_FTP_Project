#pragma once
#include "Base.h"

BEGIN(Engine)
class CTimer;

class CTimeMgr :public CBase
{
	DECLARE_SINGLETON(CTimeMgr);
private:
	explicit CTimeMgr();
	virtual ~CTimeMgr() = default;

public:
	_float	Get_DeltaTime(const _tchar* tagTimer);

public:
	HRESULT Add_Timer(const _tchar* tagTimer);


private:
	map<const _tchar*, CTimer*>			m_mapTimers;
	typedef map<const _tchar*, CTimer*>	TIMERS;

private:
	CTimer* Find_Timer(const _tchar* tagTimer);

public:
	virtual void Free() override;
};

END