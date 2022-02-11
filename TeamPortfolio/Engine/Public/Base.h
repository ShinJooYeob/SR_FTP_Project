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
	//�������� Free�� ������ �ɹ������� Release�ϰ� �����Ҵ�� ���� �����ϴ� ��� �Ҹ��ڴ� �ڽ��� �޸𸮸� ��ȯ�ϴ� �����
	virtual void Free()PURE;

};

END