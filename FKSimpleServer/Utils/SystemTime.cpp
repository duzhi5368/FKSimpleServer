#include "SystemTime.h"
#include "TimeFunc.h"
//------------------------------------------------------------------------
CSystemTime::CSystemTime(CSystemTime & st)
{
	m_stTime = st.m_stTime;
}
//------------------------------------------------------------------------
CSystemTime::CSystemTime(SYSTEMTIME & st)
{
	ZeroMemory(&m_stTime, sizeof(SYSTEMTIME));
	m_stTime = st;
}
//------------------------------------------------------------------------
CSystemTime::CSystemTime(const char * pszString)
{
	ZeroMemory(&m_stTime, sizeof(SYSTEMTIME));
	GetTimeFromString(m_stTime, pszString);
}
//------------------------------------------------------------------------
CSystemTime::CSystemTime()
{
	ZeroMemory(&m_stTime, sizeof(SYSTEMTIME));
	GetLocalTime(&m_stTime);
}
//------------------------------------------------------------------------
DWORD CSystemTime::GetToTimeSecond(CSystemTime & st)
{
	return GetT1toT2Second(m_stTime, st.m_stTime);
}
//------------------------------------------------------------------------
CSystemTime & CSystemTime::operator =(SYSTEMTIME & st)
{
	m_stTime = st;
	return (*this);
}
//------------------------------------------------------------------------
CSystemTime & CSystemTime::operator =(CSystemTime & st)
{
	m_stTime = st.m_stTime;
	return (*this);
}
//------------------------------------------------------------------------
CSystemTime & CSystemTime::operator =(const char * pszString)
{
	GetTimeFromString(m_stTime, pszString);
	return (*this);
}
//------------------------------------------------------------------------
bool CSystemTime::operator ==(CSystemTime & _st)
{
	SYSTEMTIME & st = _st.m_stTime;
	if (m_stTime.wMilliseconds != st.wMilliseconds)return false;
	if (m_stTime.wSecond != st.wSecond)return false;
	if (m_stTime.wMinute != st.wMinute)return false;
	if (m_stTime.wHour != st.wHour)return false;
	if (m_stTime.wDay != st.wDay)return false;
	if (m_stTime.wMonth != st.wMonth)return false;
	if (m_stTime.wYear != st.wYear)return false;
	return true;
}
//------------------------------------------------------------------------
bool CSystemTime::operator > (CSystemTime & _st)
{
	SYSTEMTIME	& st = _st.m_stTime;
	if (m_stTime.wYear != st.wYear)return (m_stTime.wYear > st.wYear);
	if (m_stTime.wMonth != st.wMonth)return (m_stTime.wMonth > st.wMonth);
	if (m_stTime.wDay != st.wDay)return (m_stTime.wDay > st.wDay);
	if (m_stTime.wHour != st.wHour)return (m_stTime.wHour > st.wHour);
	if (m_stTime.wMinute != st.wMinute)return (m_stTime.wMinute > st.wMinute);
	if (m_stTime.wSecond != st.wSecond)return (m_stTime.wSecond > st.wSecond);
	if (m_stTime.wMilliseconds != st.wMilliseconds)return (m_stTime.wMilliseconds > st.wMilliseconds);
	return false;
}
//------------------------------------------------------------------------
bool CSystemTime::operator < (CSystemTime & _st)
{
	SYSTEMTIME	& st = _st.m_stTime;
	if (m_stTime.wYear != st.wYear)return (m_stTime.wYear < st.wYear);
	if (m_stTime.wMonth != st.wMonth)return (m_stTime.wMonth < st.wMonth);
	if (m_stTime.wDay != st.wDay)return (m_stTime.wDay < st.wDay);
	if (m_stTime.wHour != st.wHour)return (m_stTime.wHour < st.wHour);
	if (m_stTime.wMinute != st.wMinute)return (m_stTime.wMinute < st.wMinute);
	if (m_stTime.wSecond != st.wSecond)return (m_stTime.wSecond < st.wSecond);
	if (m_stTime.wMilliseconds != st.wMilliseconds)return (m_stTime.wMilliseconds < st.wMilliseconds);
	return false;
}
//------------------------------------------------------------------------
bool CSystemTime::operator >= (CSystemTime & _st)
{
	return !(operator <(_st));
}
//------------------------------------------------------------------------
bool CSystemTime::operator <= (CSystemTime & _st)
{
	return !(operator > (_st));
}
//------------------------------------------------------------------------
bool CSystemTime::operator != (CSystemTime & _st)
{
	return !(operator == (_st));
}
//------------------------------------------------------------------------
void CSystemTime::GetDesc(char* date)
{
	//06  5 2009 10:18AM
	if (m_stTime.wHour >= 12)
	{
		sprintf(date, "%2d %2d %4d %2d:%02dPM", m_stTime.wMonth, m_stTime.wDay, m_stTime.wYear, m_stTime.wHour - 12, m_stTime.wMinute);
	}
	else
	{
		sprintf(date, "%2d %2d %4d %2d:%02dAM", m_stTime.wMonth, m_stTime.wDay, m_stTime.wYear, m_stTime.wHour, m_stTime.wMinute);
	}
}
//------------------------------------------------------------------------