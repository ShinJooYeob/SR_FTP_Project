#include "..\Public\Base.h"

using namespace Engine;

CBase::CBase()
	:m_dwRefCnt(0)
{
}

_ulong CBase::AddRef()
{
	return ++m_dwRefCnt; 
}

_ulong CBase::Release()
{
	if (m_dwRefCnt > 0) {
		return m_dwRefCnt--;
	}
	else {

		Free();
		delete this;

		return 0;
	}
}

