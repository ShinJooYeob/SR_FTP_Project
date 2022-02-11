#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	explicit CBase();
	virtual ~CBase() = default;

public:
	virtual _ulong AddRef();
	virtual _ulong Release(); 

private:
	_ulong m_dwRefCnt = 0;

public:
	//이제부터 Free는 참조된 맴버변수를 Release하고 동적할당된 것을 해제하는 기능 소멸자는 자신의 메모리를 반환하는 기능임
	virtual void Free()PURE;

};

END