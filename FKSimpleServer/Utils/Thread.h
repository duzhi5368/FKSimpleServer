#pragma once
//-------------------------------------------------------------
#include "../Base/BaseDepend.h"
//-------------------------------------------------------------
class CScheduler;
//-------------------------------------------------------------
extern __declspec(thread)	CScheduler*		s_pScheduler;
//-------------------------------------------------------------
class CThread
{
public:
	CThread(void);
	virtual ~CThread(void);

	/// 启动线程
	bool Start(LPVOID lpParam = nullptr, bool bSuspended = false, const TCHAR* threadName = nullptr);
	/// 继续线程
	bool Resume(void);
	/// 挂起线程
	bool Suspend(void);
	/// 申请终止线程
	void SubmitTerminateRequest(void);
	/// 判断线程是否已终止
	bool IsTerminated(void);
	/// 线程是否已启动
	bool IsStarted(void);
	/// 安全退出线程
	void SafeTerminate(DWORD dwWaitTime);

	TCHAR*	GetThreadName(){ return m_szThreadName; }
	HANDLE	getHandle(){ return m_hThread; }
	DWORD	getThreadID(){ return m_dwThreadID; }
	bool	IsSuspend(){ return m_bIsSuspended; }
protected:
	///线程的主运行体
	virtual void Execute(LPVOID lpParam){}
	///Terminate事件
	virtual void OnTerminated(void){};
	///启动事件
	virtual void OnStart();
private:
	volatile bool m_bTerminated;			// 线程挂起标示
	LPVOID		m_lpParam;					// 线程启动参数
	DWORD		m_dwThreadID;				// 线程ID号
	HANDLE		m_hThread;					// 线程句柄
	bool		m_bIsSuspended;				// 线程是否挂起
	TCHAR		m_szThreadName[MAX_PATH];	// 线程名
	HANDLE		m_hExitEvent;				// 退出消息事件
	///线程例程
	static unsigned int WINAPI ThreadProc(LPVOID pParam);
};
