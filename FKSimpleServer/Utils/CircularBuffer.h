#pragma once
//-------------------------------------------------------------
#include "../Base/BaseDepend.h"
//-------------------------------------------------------------
class CCircularBuffer
{
public:
	CCircularBuffer(size_t unCapacity);
	virtual ~CCircularBuffer();
public:
	bool	Peek(OUT char* pDestBuffer, size_t unBytes)const;
	bool	Read(OUT char* pDestBuffer, size_t unBytes);
	bool	Write(const char* pData, size_t unBytes);
	void	Remove(size_t unLen);

	size_t	GetFreeSpaceSize();
	size_t	GetStoredSize() const;
	size_t	GetContiguiousSize() const;
	void*	GetBuffer() const;
	void	Commit(size_t unLen);
	void*	GetBufferStart() const;
private:
	void	AllocateB();
	size_t	GetAFreeSpace() const;
	size_t	GetSpaceBeforeA() const;
	size_t	GetBFreeSpace() const;
private:
	char*	m_pBuffer;
	char*	m_pBufferEnd;
	char*	m_pARegionPointer;
	size_t	m_unARegionSize;
	char*	m_pBRegionPointer;
	size_t	m_unBRegionSize;
};