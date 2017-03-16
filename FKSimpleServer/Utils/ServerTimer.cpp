#include "ServerTimer.h"
//------------------------------------------------------------------------
DWORD CServerTimer::m_dwAllowTimeJump = 0;
//------------------------------------------------------------------------
CServerTimer::CServerTimer() 
	: m_dwSavedTime(0)
	, m_dwTimeoutTime(0)
{

}
//------------------------------------------------------------------------
void CServerTimer::Savetime()
{
	m_dwSavedTime = timeGetTime();
}
//------------------------------------------------------------------------
void CServerTimer::Savetime(DWORD newTimeOut)
{
	SetTimeOut(newTimeOut);
	Savetime();
}
//------------------------------------------------------------------------
bool CServerTimer::IsTimeOut(DWORD dwTimeOut)
{
	DWORD	dwTime = timeGetTime();
	if (GetTimeToTime(m_dwSavedTime, dwTime) >= dwTimeOut)
	{
		return true;
	}
	return false;
}
//------------------------------------------------------------------------
void CServerTimer::SetTimeOut(DWORD	dwTimeOut)
{
	m_dwSavedTime = timeGetTime();
	m_dwTimeoutTime = dwTimeOut;
}
//------------------------------------------------------------------------
bool CServerTimer::IsTimeOut()
{
	DWORD	dwTime = timeGetTime();
	if (GetTimeToTime(m_dwSavedTime, dwTime) >= m_dwTimeoutTime)
		return true;
	return false;
}
//------------------------------------------------------------------------
DWORD CServerTimer::GetTimeOut()
{ 
	return m_dwTimeoutTime; 
}
//------------------------------------------------------------------------
DWORD CServerTimer::GetSavedTime()
{ 
	return m_dwSavedTime; 
}
//------------------------------------------------------------------------
void CServerTimer::SetSavedTime(DWORD dwTime)
{ 
	m_dwSavedTime = dwTime; 
}
//------------------------------------------------------------------------