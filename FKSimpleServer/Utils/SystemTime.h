#pragma once
//------------------------------------------------------------------------
#include "../Base/BaseDepend.h"
//------------------------------------------------------------------------
class CSystemTime
{
private:
	SYSTEMTIME	m_stTime;
public:
	WORD	GetYear(){ return m_stTime.wYear; }
	WORD	GetMonth(){ return m_stTime.wMonth; }
	WORD	GetDay(){ return m_stTime.wDay; }
	WORD	GetHour(){ return m_stTime.wHour; }
	WORD	GetMinute(){ return m_stTime.wMinute; }
	WORD	GetSecond(){ return m_stTime.wSecond; }
	WORD	GetMilliSeconds(){ return m_stTime.wMilliseconds; }

	CSystemTime(CSystemTime & st);
	CSystemTime(SYSTEMTIME & st);
	CSystemTime(const char * pszString);
	CSystemTime();

	DWORD	GetToTimeSecond(CSystemTime & st);
	CSystemTime & operator =(SYSTEMTIME & st);
	CSystemTime & operator =(CSystemTime & st);
	CSystemTime & operator =(const char * pszString);
	bool operator ==(CSystemTime & _st);
	bool operator > (CSystemTime & _st);
	bool operator < (CSystemTime & _st);
	bool operator >= (CSystemTime & _st);
	bool operator <= (CSystemTime & _st);
	bool operator != (CSystemTime & _st);
	void GetDesc(char* date);
};