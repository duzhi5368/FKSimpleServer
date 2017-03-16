#include "Thread.h"
#include "../Base/BaseMacro.h"
//-------------------------------------------------------------
__declspec(thread)	CScheduler*		s_pScheduler = nullptr;
//-------------------------------------------------------------
CThread::CThread(void) 
	: m_dwThreadID(0)
	, m_hThread(INVALID_HANDLE_VALUE)
	, m_bTerminated(false)
	, m_bIsSuspended(false)
	, m_lpParam(nullptr)
	, m_hExitEvent(NULL)
{
	memset(m_szThreadName, 0, sizeof(m_szThreadName));
}
//-------------------------------------------------------------
CThread::~CThread(void)
{
	SubmitTerminateRequest();
	if (m_hThread != INVALID_HANDLE_VALUE)
	{
		SafeTerminate(DEFAULT_THREAD_SAFT_TERMINATE_TIME);
	}

	m_lpParam = nullptr;
	m_dwThreadID = 0;
	m_bIsSuspended = false;
	m_bTerminated = false;
	m_hThread = INVALID_HANDLE_VALUE;
	memset(m_szThreadName, 0, sizeof(m_szThreadName));
}
//-------------------------------------------------------------
/// 安全退出线程
void CThread::SafeTerminate(DWORD dwWaitTime)
{
	if (m_hExitEvent == NULL || m_hThread == NULL)
		return;
	INFO_MSG_LOG_WITH_TIME(L"%s 线程安全退出等待 %d ms：线程ID = [%d]", GetThreadName(), dwWaitTime, getThreadID());
	SetEvent(m_hExitEvent);
	if (WaitForSingleObject(m_hThread, dwWaitTime) == WAIT_TIMEOUT)
	{
		/*
		// TODO: 此处可能死循环
		DWORD dwRet = WaitForSingleObject(m_hThread, INFINITE);
		if (dwRet == WAIT_TIMEOUT)
		{
			ERROR_MSG_LOG_WITH_TIME(L"%s 线程 [%d] 退出时等待超时.", GetThreadName(), getThreadID());
		}
		else if (dwRet == WAIT_FAILED)
		{
			ERROR_MSG_LOG_WITH_TIME(L"%s 线程 [%d] 退出时等待出现错误：错误码 = %d.", GetThreadName(), getThreadID(), GetLastError());
		}
		else if (dwRet == WAIT_OBJECT_0)
		{
			INFO_MSG_LOG_WITH_TIME(L"%s 线程 [%d] 退出完成.", GetThreadName(), getThreadID());
		}
		*/
		// 暴力结束线程
		TerminateThread(m_hThread, 0);
	}
	return;
}
//-------------------------------------------------------------
void CThread::OnStart()
{
	//	初始化随机数种子
	srand(GetCurrentThreadId() + timeGetTime());
	m_hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	ResetEvent(m_hExitEvent);
}
//-------------------------------------------------------------
bool CThread::Start(LPVOID lpParam, bool bSuspended, const TCHAR* threadName)
{
	if (m_hThread != INVALID_HANDLE_VALUE)
		return false;
	unsigned int dwThreadID;
	int nFlag = 0;
	m_lpParam = lpParam;
	if (bSuspended)
	{
		nFlag = CREATE_SUSPENDED;
		m_bIsSuspended = true;
	}
	else
		m_bIsSuspended = false;

	HANDLE hThread = (HANDLE)CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(void*)this,
		nFlag,
		(LPDWORD)&dwThreadID);
	if (hThread == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	wcscpy(m_szThreadName, threadName);
	INFO_MSG_LOG_WITH_TIME(L"创建 %s 线程成功：线程ID = [%d]", GetThreadName(), dwThreadID);

	m_dwThreadID = dwThreadID;
	m_hThread = hThread;
	return true;
}
//-------------------------------------------------------------
bool CThread::Resume(void)
{
	if (ResumeThread(m_hThread) != (DWORD)-1)
	{
		INFO_MSG_LOG_WITH_TIME(L"%s 线程激活：线程ID = [%d]", GetThreadName(), getThreadID());
		m_bIsSuspended = false;
		return true;
	}
	else
		return false;
}
//-------------------------------------------------------------
bool CThread::Suspend(void)
{
	if (SuspendThread(m_hThread) != (DWORD)-1)
	{
		INFO_MSG_LOG_WITH_TIME(L"%s 线程挂起：线程ID = [%d]", GetThreadName(), getThreadID());
		m_bIsSuspended = true;
		return true;
	}
	else
		return false;
}
//-------------------------------------------------------------
bool CThread::IsTerminated(void)
{
	return m_bTerminated;
}
//-------------------------------------------------------------
bool CThread::IsStarted(void)
{
	return m_hThread != INVALID_HANDLE_VALUE;
}
//-------------------------------------------------------------
void CThread::SubmitTerminateRequest(void)
{
	::InterlockedExchange((LONG *)&m_bTerminated, TRUE);
}
//-------------------------------------------------------------
unsigned WINAPI CThread::ThreadProc(LPVOID pParam)
{
	CThread* pThread = (CThread*)pParam;
	::InterlockedExchange((LONG *)&(pThread->m_bTerminated), FALSE);

	pThread->OnStart();
	pThread->Execute(pThread->m_lpParam);
	pThread->OnTerminated();

	INFO_MSG_LOG_WITH_TIME(L"%s 线程 [%d] 退出.", pThread->GetThreadName(), pThread->getThreadID());
	CloseHandle(pThread->m_hThread);
	pThread->m_hThread = INVALID_HANDLE_VALUE;
	pThread->m_dwThreadID = 0;
	pThread->m_bTerminated = TRUE;
	return 0;
}
//-------------------------------------------------------------