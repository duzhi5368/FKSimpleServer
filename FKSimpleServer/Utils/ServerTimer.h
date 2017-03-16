#pragma once
//------------------------------------------------------------------------
#include "../Base/BaseDepend.h"
//------------------------------------------------------------------------
#define MAXTIME	(DWORD(0xffffffff))
inline DWORD	GetTimeToTime(DWORD t1, DWORD t2)
{
	return (t1 <= t2 ? (t2 - t1) : (MAXTIME - t1 + t2));
}
//------------------------------------------------------------------------
class CServerTimer
{
public:
	CServerTimer();
	static inline DWORD	GetTime()
	{
		return timeGetTime();
	}
	static bool	IsTimeOut(DWORD starttime, DWORD timeout)
	{
		DWORD	dwTime = timeGetTime();
		if (GetTimeToTime(starttime, dwTime) >= timeout)
		{
			return true;
		}
		return false;
	}
	static DWORD GetTimeJump()
	{
		return m_dwAllowTimeJump; 
	}
	static void	SetTimeJump(DWORD dwTimeJump)
	{ 
		m_dwAllowTimeJump = dwTimeJump;
	}

	void	Savetime();
	void	Savetime(DWORD newTimeOut);
	bool	IsTimeOut(DWORD dwTimeOut);
	void	SetTimeOut(DWORD dwTimeOut);
	bool	IsTimeOut();
	DWORD	GetTimeOut();
	DWORD	GetSavedTime();
	void	SetSavedTime(DWORD dwTime);
	// 距离上次保存，过了多少时间
	DWORD   GetTimeFromSave()
	{
		return (GetTimeToTime(m_dwSavedTime, timeGetTime()));
	}
private:
	static DWORD m_dwAllowTimeJump;
	DWORD	m_dwSavedTime;
	DWORD	m_dwTimeoutTime;
};