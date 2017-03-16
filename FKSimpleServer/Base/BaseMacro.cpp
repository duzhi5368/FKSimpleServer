#include "BaseMacro.h"
#include "../Framework/BaseServer.h"
#include "../Utils/LogFile.h"
#include "../Framework/ServerForm.h"
//------------------------------------------------------------------------
void INFO_MSG(const TCHAR* x, ...)
{
	TCHAR	szBuff[4096];
	va_list	vl;
	va_start(vl, x);
	vswprintf(szBuff, x, vl);
	va_end(vl);
	if (g_pSvr && g_pSvr->GetIoConsole())
	{
		g_pSvr->GetIoConsole()->OutPut(INFO_COLOR, szBuff);
	}
	else
	{
		OutputDebugString(szBuff);
		OutputDebugString(L"\n");
	}
}
//------------------------------------------------------------------------
void INFO_MSG_LOG(const TCHAR* x, ...)
{
	TCHAR	szBuff[4096];
	va_list	vl;
	va_start(vl, x);
	vswprintf(szBuff, x, vl);
	va_end(vl);
	if (g_pSvr && g_pSvr->GetIoConsole())
	{
		g_pSvr->GetIoConsole()->OutPut(INFO_COLOR, szBuff);
	}
	else
	{
		OutputDebugString(szBuff);
		OutputDebugString(L"\n");
	}
	if (CLogFile::GetInstance()->IsHadInit())
	{
		CLogFile::GetInstance()->LogText(szBuff);
		CLogFile::GetInstance()->LogText(L"\n");
	}
}
//------------------------------------------------------------------------
void INFO_MSG_LOG_WITH_TIME(const TCHAR* x, ...)
{
	TCHAR	szBuff[4096];
	va_list	vl;
	va_start(vl, x);
	vswprintf(szBuff, x, vl);
	va_end(vl);
	if (g_pSvr && g_pSvr->GetIoConsole())
	{
		g_pSvr->GetIoConsole()->OutPut(INFO_COLOR, szBuff);
	}
	else
	{
		OutputDebugString(szBuff);
		OutputDebugString(L"\n");
	}
	if (CLogFile::GetInstance()->IsHadInit())
	{
		CLogFile::GetInstance()->LogTextWithDate(szBuff);
	}
}
//------------------------------------------------------------------------
void ERROR_MSG_LOG_WITH_TIME(const TCHAR* x, ...)
{
	TCHAR	szBuff[4096];
	va_list	vl;
	va_start(vl, x);
	vswprintf(szBuff, x, vl);
	va_end(vl);
	if (g_pSvr && g_pSvr->GetIoConsole())
	{
		g_pSvr->GetIoConsole()->OutPut(ERROR_COLOR, szBuff);
	}
	else
	{
		OutputDebugString(szBuff);
		OutputDebugString(L"\n");
	}
	if (CLogFile::GetInstance()->IsHadInit())
	{
		CLogFile::GetInstance()->LogTextWithDate(szBuff);
	}
}
//------------------------------------------------------------------------
bool CONFIM_MESSAGE_BOX(TCHAR * pszCaption, TCHAR * pszText)
{
	if (g_pSvrForm == nullptr)
		return false;
	return g_pSvrForm->ConfirmMessageBox(pszCaption, pszText);
}
//------------------------------------------------------------------------
//
// Usage: SetThreadName ((DWORD)-1, "MainThread");
//
const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)
void SET_THREAD_NAME(DWORD dwThreadID, const char* threadName) {
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;
#pragma warning(push)
#pragma warning(disable: 6320 6322)
	__try{
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER){
	}
#pragma warning(pop)
}
//------------------------------------------------------------------------