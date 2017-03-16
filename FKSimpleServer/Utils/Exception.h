#pragma once
//-------------------------------------------------------------
#include "../Base/BaseDepend.h"
//-------------------------------------------------------------
LONG WINAPI ExceptionFilter(EXCEPTION_POINTERS* pExceptionInfo);
//-------------------------------------------------------------
#define FK_CRASH_ASSERT(x)	CrashAssert(x)
//-------------------------------------------------------------
inline void CrashAssert(bool bIsOk)
{
	if (bIsOk)
		return;

	int* pCrashVal = 0;
	*pCrashVal = 0xDEADBEEF;
}
//-------------------------------------------------------------