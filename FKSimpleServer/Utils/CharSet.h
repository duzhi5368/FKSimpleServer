#pragma once
//-------------------------------------------------------------
#include "../Base/BaseDepend.h"
//-------------------------------------------------------------
class CCharSet
{
public:
	CCharSet();
	CCharSet(const char * pszTable);
	CCharSet(DWORD dwFlags[]);
	~CCharSet();

public:
	bool charIn(char c);
	void addChar(char c);
	void clear();

	CCharSet & operator +(CCharSet & charset);
	CCharSet & operator +=(CCharSet & charset);
	CCharSet & operator =(CCharSet & charset);
protected:
	DWORD	m_dwFlags[8];
};
//-------------------------------------------------------------
static CCharSet CharSetWhite(" \t");
static CCharSet CharSetCR("\r\n");
static CCharSet CharSetLowerCharacter("abcdefghijklmnopqrstuvwxyz");
static CCharSet CharSetUpperCharacter("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
static CCharSet CharSetDigit("0123456789");
static CCharSet CharSetOperator("+-*/\\()[]{}\";'<>,.?!~`#$%^&|_=");
//-------------------------------------------------------------