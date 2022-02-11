#pragma once
#pragma once

namespace Engine
{
	class CTagFinder
	{
	public:
		CTagFinder(const _tchar* pTag) : m_pTag(pTag) { }
		~CTagFinder() = default;

	public:
		template <typename T>
		bool operator () (T& Pair)
		{
			if (0 == lstrcmp(Pair.first, m_pTag))
				return true;
			return false;
		}

	private:
		const _tchar*			m_pTag = nullptr;
	};

	template <typename T>
	void Safe_Delete(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete pPointer;
			pPointer = nullptr;
		}
	}

	template <typename T>
	void Safe_Delete_Array(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete[] pPointer;
			pPointer = nullptr;
		}
	}

	template <typename T>
	_ulong Safe_AddRef(T& pInstance)
	{
		_ulong		dwRefCnt = 0;

		if (nullptr != pInstance)
			dwRefCnt = pInstance->AddRef();

		return dwRefCnt;
	}

	template <typename T>
	_ulong Safe_Release(T& pInstance)
	{
		_ulong		dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			dwRefCnt = pInstance->Release();
			pInstance = nullptr;
		}

		return dwRefCnt;
	}


}