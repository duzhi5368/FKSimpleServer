#include "CharSet.h"
//-------------------------------------------------------------
CCharSet::CCharSet()
{
	clear();
}
//-------------------------------------------------------------
CCharSet::CCharSet(const char * pszTable)
{
	clear();
	for (int i = 0; i < (int)strlen(pszTable); i++)
		addChar(pszTable[i]);
}
//-------------------------------------------------------------
CCharSet::CCharSet(DWORD dwFlags[])
{
	for (int i = 0; i < 8; i++)
	{
		m_dwFlags[i] = dwFlags[i];
	}
}
//-------------------------------------------------------------
CCharSet::~CCharSet()
{
}
//-------------------------------------------------------------
bool CCharSet::charIn(char c)
{
	BYTE	bc = (BYTE)c;
	int index = bc >> 5;
	int ptr = bc & 31;
	if (m_dwFlags[index] & (1 << ptr))
		return true;
	return false;
}
//-------------------------------------------------------------
void CCharSet::addChar(char c)
{
	BYTE	bc = (BYTE)c;
	int index = bc >> 5;
	int ptr = bc & 31;
	m_dwFlags[index] |= 1 << ptr;
}
//-------------------------------------------------------------
void CCharSet::clear()
{
	memset(m_dwFlags, 0, sizeof(m_dwFlags));
}
//-------------------------------------------------------------
CCharSet& CCharSet::operator +(CCharSet & charset)
{
	DWORD dwFlags[8];
	for (int i = 0; i < 8; i++)
	{
		dwFlags[i] = m_dwFlags[i] | charset.m_dwFlags[i];
	}
	CCharSet tmp = CCharSet(dwFlags);
	return tmp;
}
//-------------------------------------------------------------
CCharSet& CCharSet::operator +=(CCharSet & charset)
{
	for (int i = 0; i < 8; i++)
	{
		m_dwFlags[i] |= charset.m_dwFlags[i];
	}
	return (*this);
}
//-------------------------------------------------------------
CCharSet& CCharSet::operator =(CCharSet & charset)
{
	for (int i = 0; i < 8; i++)
	{
		m_dwFlags[i] = charset.m_dwFlags[i];
	}
	return (*this);
}
//-------------------------------------------------------------