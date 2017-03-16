#pragma once
//-------------------------------------------------------------
#include "StringFunc.h"
//-------------------------------------------------------------
template <UINT MaxCount>
class TStringsExtracter
{
public:
	TStringsExtracter()
	{
		clear();
	}

	TStringsExtracter(char * pszString, int delim)
	{
		ExtractString(pszString, delim);
	}

	TStringsExtracter(char * pszString, const char * pszDelimTable = "/", const char * pszWhiteTable = " \t", BOOL	bKeepEmpty = TRUE, BOOL bKeepString = TRUE)
	{
		ExtractString(pszString, pszDelimTable, pszWhiteTable, bKeepEmpty, bKeepString);
	}

	void ExtractString(char * pszString, const char * pszDelimTable = "/", const char * pszWhiteTable = " \t", BOOL	bKeepEmpty = TRUE, BOOL bKeepString = TRUE)
	{
		clear();
		m_nStringCount = (UINT)ExtractStrings(pszString, xCharSet(pszWhiteTable), xCharSet(pszDelimTable), m_pStrings, MaxCount, bKeepEmpty, bKeepString);
	}

	void ExtractString(char * pszString, int delim)
	{
		clear();
		m_nStringCount = SearchParam(pszString, m_pStrings, MaxCount, delim);
	}

	void clear()
	{
		memset(m_pStrings, 0, sizeof(m_pStrings));
		m_nStringCount = 0;
	}

	UINT getCount(){ return m_nStringCount; }
	char * getString(UINT ptr){ if (ptr > m_nStringCount)return NULL; return m_pStrings[ptr]; }
	char * operator [](UINT ptr){ return getString(ptr); }

protected:
	UINT	m_nStringCount;
	char * m_pStrings[MaxCount];
};