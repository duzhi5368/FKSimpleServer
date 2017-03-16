#pragma once
//------------------------------------------------------------------------
// 是否是闰年
inline	bool	IsRunYear(int Year)
{
	if ((Year % 100) == 0)
	{
		if ((Year % 400) != 0)
			return false;
		return true;
	}
	else if ((Year & 0x3) != 0)
		return false;
	return true;
}
//------------------------------------------------------------------------
// 从字符串解析时间
inline void	GetTimeFromString(SYSTEMTIME & t, const char * pszString)
{
	try
	{
		char temp[10];
		sscanf(pszString, "%hu %hu %hu %hu:%hu%s", &t.wMonth, &t.wDay, &t.wYear, &t.wHour, &t.wMinute, temp);
		if (strcmp(temp, "AM") == 0)
		{

		}
		else if (strcmp(temp, "PM") == 0)
		{
			t.wMonth += 12;
		}
	}
	catch (...)
	{
		memset(&t, 0, sizeof(t));
	}
}
//------------------------------------------------------------------------
#define STONEDAY	86400
#define STONEYEAR299	31622400
#define	STONEYEARNO299	31536000 
static DWORD	stMonth_no299[12] =
{
	0,																				//	1
	STONEDAY * (31),																//	2
	STONEDAY * (31 + 28),															//	3
	STONEDAY * (31 + 28 + 31),														//	4
	STONEDAY * (31 + 28 + 31 + 30),													//	5
	STONEDAY * (31 + 28 + 31 + 30 + 31),											//	6
	STONEDAY * (31 + 28 + 31 + 30 + 31 + 30),										//	7
	STONEDAY * (31 + 28 + 31 + 30 + 31 + 30 + 31),									//	8
	STONEDAY * (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31),								//	9
	STONEDAY * (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30),						//	10
	STONEDAY * (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31),					//	11
	STONEDAY * (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30),				//	12
};

static DWORD	stMonth_299[12] =
{
	0,																	//	1
	STONEDAY * (31),													//	2
	STONEDAY * (31 + 29),												//	3
	STONEDAY * (31 + 29 + 31),											//	4
	STONEDAY * (31 + 29 + 31 + 30),										//	5
	STONEDAY * (31 + 29 + 31 + 30 + 31),								//	6
	STONEDAY * (31 + 29 + 31 + 30 + 31 + 30),							//	7
	STONEDAY * (31 + 29 + 31 + 30 + 31 + 30 + 31),						//	8
	STONEDAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31),					//	9
	STONEDAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30),			//	10
	STONEDAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31),		//	11
	STONEDAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30),	//	12
};
//------------------------------------------------------------------------
inline DWORD	GetT1toT2Second(SYSTEMTIME & t1, SYSTEMTIME & t2)// SYSTEMTIME & tresult )
{
	DWORD	st1 = 0, st2 = 0;
	if (t1.wYear == t2.wYear)
	{
		if (IsRunYear(t1.wYear))
		{
			st1 = stMonth_299[t1.wMonth - 1] + (t1.wDay - 1) * STONEDAY + (t1.wHour) * 3600 + (t1.wMinute) * 60 + t1.wSecond;
			st2 = stMonth_299[t2.wMonth - 1] + (t2.wDay - 1) * STONEDAY + (t2.wHour) * 3600 + (t2.wMinute) * 60 + t2.wSecond;
		}
		else
		{
			st1 = stMonth_no299[t1.wMonth - 1] + t1.wDay * STONEDAY + t1.wHour * 3600 + t1.wMinute * 60 + t1.wSecond;
			st2 = stMonth_no299[t2.wMonth - 1] + t2.wDay * STONEDAY + t2.wHour * 3600 + t2.wMinute * 60 + t2.wSecond;
		}
		st2 -= st1;
		//	计算t1过了一年的多少秒，t2过了多少秒，然后t2 - t1;
	}
	else
	{
		//	计算t1这一年剩下多少秒，t2过了一年的多少秒，t1+t2

		if (IsRunYear(t1.wYear))
			st1 = STONEYEAR299 - (stMonth_299[t1.wMonth - 1] + (t1.wDay - 1) * STONEDAY + (t1.wHour) * 3600 + (t1.wMinute) * 60 + t1.wSecond);
		else
			st1 = STONEYEARNO299 - (stMonth_no299[t1.wMonth - 1] + (t1.wDay - 1) * STONEDAY + (t1.wHour) * 3600 + (t1.wMinute) * 60 + t1.wSecond);
		if (IsRunYear(t2.wYear))
			st2 = (stMonth_299[t2.wMonth - 1] + (t2.wDay - 1) * STONEDAY + (t2.wHour) * 3600 + (t2.wMinute) * 60 + t2.wSecond);
		else
			st2 = (stMonth_no299[t2.wMonth - 1] + (t2.wDay - 1) * STONEDAY + (t2.wHour) * 3600 + (t2.wMinute) * 60 + t2.wSecond);
		st2 += st1;
		for (int i = t1.wYear + 1; i < t2.wYear; i++)
		{
			if (IsRunYear(i))
				st2 += STONEYEAR299;
			else
				st2 += STONEYEARNO299;
		}

	}
	return st2;
}
//------------------------------------------------------------------------
inline WORD	GetMonthDays(WORD year, WORD month)
{
	static WORD wDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (month == 0 || month > 12)return 0;
	WORD	wRet = wDays[month - 1];
	if (month == 2 && IsRunYear(year))
		wRet++;
	return wRet;
}
//------------------------------------------------------------------------
inline VOID stPlusDay(SYSTEMTIME & st, WORD wDay)
{
	if (65535 - st.wDay < wDay)
	{
		DWORD dwDays = st.wDay + wDay;
		WORD wMonthDays = GetMonthDays(st.wYear, st.wMonth);
		while (dwDays > wMonthDays)
		{
			dwDays -= wMonthDays;
			st.wMonth++;
			if (st.wMonth > 12)
			{
				st.wMonth = 1;
				st.wYear++;
			}
			wMonthDays = GetMonthDays(st.wYear, st.wMonth);
		}
		st.wDay = static_cast<WORD>(dwDays);
	}
	else
	{
		st.wDay += wDay;
		WORD wMonthDays = GetMonthDays(st.wYear, st.wMonth);
		while (st.wDay > wMonthDays)
		{
			st.wDay -= wMonthDays;
			st.wMonth++;
			if (st.wMonth > 12)
			{
				st.wMonth = 1;
				st.wYear++;
			}
			wMonthDays = GetMonthDays(st.wYear, st.wMonth);
		}
	}
}
//------------------------------------------------------------------------