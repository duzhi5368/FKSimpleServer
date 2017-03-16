#pragma once
//-------------------------------------------------------------
#include "Exception.h"
//-------------------------------------------------------------
template<class TObject, int nAllocCount = 100>
class TObjectPool
{
public:
	static void* operator new(size_t unObjSize)
	{
		if (m_pFreeList == nullptr)
		{
			m_pFreeList = new uint8_t[sizeof(TObject) * nAllocCount];

			uint8_t* pNext	= m_pFreeList;
			uint8_t** ppCur = reinterpret_cast<uint8_t**>(m_pFreeList);

			for (int i = 0; i < nAllocCount - 1; ++i)
			{
				pNext += sizeof(TObject);
				*ppCur = pNext;
				ppCur = reinterpret_cast<uint8_t**>(pNext);
			}

			*ppCur = 0;
			m_nTotalAllocCount += nAllocCount;
		}

		uint8_t* pAvailable = m_pFreeList;
		m_pFreeList = *reinterpret_cast<uint8_t**>(pAvailable);
		++m_nCurrentUseCount;

		return pAvailable;
	}

	static void operator delete(void* pObj)
	{
		FK_CRASH_ASSERT(m_nCurrentUseCount > 0);

		--m_nCurrentUseCount;
		*reinterpret_cast<uint8_t**>(pObj) = m_pFreeList;
		m_pFreeList = static_cast<uint8_t*>(pObj);
	}
private:
	static uint8_t*	m_pFreeList;
	static int		m_nTotalAllocCount;
	static int		m_nCurrentUseCount;
};
//-------------------------------------------------------------
template<class TObject, int nAllocCount>
uint8_t* TObjectPool<TObject, nAllocCount>::m_pFreeList = nullptr;
template<class TObject, int nAllocCount>
int TObjectPool<TObject, nAllocCount>::m_nTotalAllocCount = 0;
template<class TObject, int nAllocCount>
int TObjectPool<TObject, nAllocCount>::m_nCurrentUseCount = 0;