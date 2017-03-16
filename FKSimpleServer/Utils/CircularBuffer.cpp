#include "CircularBuffer.h"
//-------------------------------------------------------------
CCircularBuffer::CCircularBuffer(size_t unCapacity)
	: m_pBRegionPointer( nullptr )
	, m_unARegionSize( 0 )
	, m_unBRegionSize( 0 )
{
	m_pBuffer = new char[unCapacity];
	m_pBufferEnd = m_pBuffer + unCapacity;
	m_pARegionPointer = m_pBuffer;
}
//-------------------------------------------------------------
CCircularBuffer::~CCircularBuffer()
{
	delete [] m_pBuffer;
}
//-------------------------------------------------------------
bool CCircularBuffer::Peek(OUT char* pDestBuffer, size_t unBytes)const
{
	assert(m_pBuffer != nullptr);

	if (m_unARegionSize + m_unBRegionSize < unBytes)
		return false;

	size_t unCnt = unBytes;
	size_t unRead = 0;

	if (m_unARegionSize > 0)
	{
		unRead = (unCnt > m_unARegionSize) ? m_unARegionSize : unCnt;
		memcpy(pDestBuffer, m_pARegionPointer, unRead);
		unCnt -= unRead;
	}

	if (unCnt > 0 && m_unBRegionSize > 0)
	{
		assert(unCnt <= m_unBRegionSize);
		size_t unRead = unCnt;
		memcpy(pDestBuffer + unRead, m_pBRegionPointer, unRead);
		unCnt -= unRead;
	}

	assert(unCnt == 0);
	return true;
}
//-------------------------------------------------------------
bool CCircularBuffer::Read(OUT char* pDestBuffer, size_t unBytes)
{
	assert(m_pBuffer != nullptr);

	if (m_unARegionSize + m_unBRegionSize < unBytes)
		return false;

	size_t unCnt = unBytes;
	size_t unRead = 0;

	if (m_unBRegionSize > 0)
	{
		unRead = (unCnt > m_unARegionSize) ? m_unARegionSize : unCnt;
		memcpy(pDestBuffer, m_pARegionPointer, unRead);
		m_unARegionSize -= unRead;
		m_pARegionPointer += unRead;
		unCnt -= unRead;
	}

	if (unCnt > 0 && m_unBRegionSize > 0)
	{
		assert(unCnt <= m_unBRegionSize);

		size_t unRead = unCnt;
		memcpy(pDestBuffer + unRead, m_pARegionPointer, unRead);
		m_unBRegionSize -= unRead;
		m_pBRegionPointer += unRead;
		unCnt -= unRead;
	}

	assert(unCnt == 0);

	if (m_unARegionSize == 0)
	{
		if (m_unBRegionSize > 0)
		{
			if (m_pBRegionPointer != m_pBuffer)
			{
				memmove(m_pBuffer, m_pBRegionPointer, m_unBRegionSize);
			}

			m_pARegionPointer = m_pBuffer;
			m_unARegionSize = m_unBRegionSize;
			m_pBRegionPointer = nullptr;
			m_unBRegionSize = 0;
		}
		else
		{
			m_pBRegionPointer = nullptr;
			m_unBRegionSize = 0;
			m_pARegionPointer = m_pBuffer;
			m_unBRegionSize = 0;
		}
	}

	return true;
}
//-------------------------------------------------------------
bool CCircularBuffer::Write(const char* pData, size_t unBytes)
{
	assert(m_pBuffer != nullptr);

	if (m_pBRegionPointer != nullptr)
	{
		if (GetBFreeSpace() < unBytes)
			return false;

		memcpy(m_pBRegionPointer + m_unBRegionSize, pData, unBytes);
		m_unBRegionSize += unBytes;

		return true;
	}

	if (GetAFreeSpace() < GetSpaceBeforeA())
	{
		AllocateB();

		if (GetBFreeSpace() < unBytes)
			return false;

		memcpy(m_pBRegionPointer + m_unBRegionSize, pData, unBytes);
		m_unBRegionSize += unBytes;

		return true;
	}
	else
	{
		if (GetAFreeSpace() < unBytes)
			return false;

		memcpy(m_pARegionPointer + m_unARegionSize, pData, unBytes);
		m_unARegionSize += unBytes;

		return true;
	}
}
//-------------------------------------------------------------
void CCircularBuffer::Remove(size_t unLen)
{
	size_t unCnt = unLen;

	if (m_unARegionSize > 0)
	{
		size_t unRemove = (unCnt > m_unARegionSize) ? m_unARegionSize : unCnt;
		m_unARegionSize -= unRemove;
		m_pARegionPointer += unRemove;
		unCnt -= unRemove;
	}

	if (unCnt > 0 && m_unBRegionSize > 0)
	{
		size_t unRemove = (unCnt > m_unBRegionSize) ? m_unBRegionSize : unCnt;
		m_unBRegionSize -= unRemove;
		m_pBRegionPointer += unRemove;
		unCnt -= unRemove;
	}

	if (m_unARegionSize == 0)
	{
		if ( m_unBRegionSize > 0 )
		{ 
			if (m_pBRegionPointer != m_pBuffer)
				memmove(m_pBuffer, m_pBRegionPointer, m_unBRegionSize);

			m_pARegionPointer = m_pBuffer;
			m_unARegionSize = m_unBRegionSize;
			m_pBRegionPointer = nullptr;
			m_unBRegionSize = 0;
		}
		else
		{
			m_pBRegionPointer = nullptr;
			m_unBRegionSize = 0;
			m_pARegionPointer = m_pBuffer;
			m_unARegionSize = 0;
		}
	}
}
//-------------------------------------------------------------
size_t CCircularBuffer::GetFreeSpaceSize()
{
	if (m_pBRegionPointer != nullptr)
	{
		return GetBFreeSpace();
	}
	else
	{
		if (GetAFreeSpace() < GetSpaceBeforeA())
		{
			AllocateB();
			return GetBFreeSpace();
		}
		else
		{
			return GetAFreeSpace();
		}
	}
}
//-------------------------------------------------------------
size_t CCircularBuffer::GetStoredSize() const
{
	return m_unARegionSize + m_unBRegionSize;
}
//-------------------------------------------------------------
size_t CCircularBuffer::GetContiguiousSize() const
{
	if (m_unARegionSize > 0)
		return m_unARegionSize;
	else
		return m_unBRegionSize;
}
//-------------------------------------------------------------
void* CCircularBuffer::GetBuffer() const
{
	if (m_pBRegionPointer != nullptr)
		return m_pBRegionPointer + m_unBRegionSize;
	else
		return m_pARegionPointer + m_unARegionSize;
}
//-------------------------------------------------------------
void CCircularBuffer::Commit(size_t unLen)
{
	if (m_pBRegionPointer != nullptr)
		m_pBRegionPointer += unLen;
	else 
		m_pARegionPointer += unLen;
}
//-------------------------------------------------------------
void* CCircularBuffer::GetBufferStart() const
{
	if (m_unARegionSize > 0)
		return m_pARegionPointer;
	else
		return m_pBRegionPointer;
}
//-------------------------------------------------------------
void CCircularBuffer::AllocateB()
{
	m_pBRegionPointer = m_pBuffer;
}
//-------------------------------------------------------------
size_t CCircularBuffer::GetAFreeSpace() const
{
	return (m_pBufferEnd - m_pARegionPointer - m_unARegionSize);
}
//-------------------------------------------------------------
size_t CCircularBuffer::GetSpaceBeforeA() const
{
	return (m_pARegionPointer - m_pBuffer);
}
//-------------------------------------------------------------
size_t CCircularBuffer::GetBFreeSpace() const
{
	if (m_pBRegionPointer == nullptr)
		return 0;
	return (m_pARegionPointer - m_pBRegionPointer - m_unBRegionSize);
}
//-------------------------------------------------------------