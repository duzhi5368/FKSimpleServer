#pragma once
//-------------------------------------------------------------
#include "../Base/BaseDepend.h"
#include "../Base/ServerConfig.h"
//-------------------------------------------------------------
template< typename TElem, int nQSize >
class TProducerConsumerQueue
{
public:
	TProducerConsumerQueue()
		: m_unOccupiedSize(0)
		, m_unQueueOffset(0)
	{
		memset(m_QueueArray, 0, sizeof(m_QueueArray));
		InitializeConditionVariable(&m_NotFull);
		InitializeConditionVariable(&m_NotEmpty);
		InitializeSRWLock(&m_SRWLock);
	}
	~TProducerConsumerQueue(){
		memset(m_QueueArray, 0, sizeof(m_QueueArray));
	}
public:
	bool	Produce(const TElem& item, bool bWaitUntilConsume = true)
	{
		AcquireSRWLockExclusive(&m_SRWLock);
		while (m_unOccupiedSize == nQSize)
		{
			if (bWaitUntilConsume)
			{
				SleepConditionVariableSRW(&m_NotFull, &m_SRWLock, INFINITE, 0);
			}
			else
			{
				ReleaseSRWLockExclusive(&m_SRWLock);
				return false;
			}
		}
		m_QueueArray[(m_unQueueOffset + m_unOccupiedSize) % nQSize] = item;
		++m_unOccupiedSize;

		ReleaseSRWLockExclusive(&m_SRWLock);
		WakeConditionVariable(&m_NotEmpty);
		return true;
	}
	bool Consume(TElem& item, bool bWaitUntilProduce = true)
	{
		AcquireSRWLockExclusive(&m_SRWLock);
		while (m_unOccupiedSize == 0)
		{
			if (bWaitUntilProduce)
			{
				SleepConditionVariableSRW(&m_NotEmpty, &m_SRWLock, INFINITE, 0);
			}
			else
			{
				ReleaseSRWLockExclusive(&m_SRWLock);
				return false;
			}
		}

		item = m_QueueArray[m_unQueueOffset];
		--m_unOccupiedSize;

		if (++m_unQueueOffset == nQSize)
			m_unQueueOffset = 0;

		ReleaseSRWLockExclusive(&m_SRWLock);
		WakeConditionVariable(&m_NotFull);
		return true;
	}
private:
	TElem				m_QueueArray[nQSize];
	uint32_t			m_unOccupiedSize;
	uint32_t			m_unQueueOffset;
	CONDITION_VARIABLE	m_NotFull;
	CONDITION_VARIABLE	m_NotEmpty;
	SRWLOCK				m_SRWLock;
};
//-------------------------------------------------------------
typedef TProducerConsumerQueue<SOCKET, MAX_PENDING_ACCEPT_SOCKET_NUM> PendingAcceptList;
//-------------------------------------------------------------