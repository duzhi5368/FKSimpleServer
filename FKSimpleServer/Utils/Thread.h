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

	/// �����߳�
	bool Start(LPVOID lpParam = nullptr, bool bSuspended = false, const TCHAR* threadName = nullptr);
	/// �����߳�
	bool Resume(void);
	/// �����߳�
	bool Suspend(void);
	/// ������ֹ�߳�
	void SubmitTerminateRequest(void);
	/// �ж��߳��Ƿ�����ֹ
	bool IsTerminated(void);
	/// �߳��Ƿ�������
	bool IsStarted(void);
	/// ��ȫ�˳��߳�
	void SafeTerminate(DWORD dwWaitTime);

	TCHAR*	GetThreadName(){ return m_szThreadName; }
	HANDLE	getHandle(){ return m_hThread; }
	DWORD	getThreadID(){ return m_dwThreadID; }
	bool	IsSuspend(){ return m_bIsSuspended; }
protected:
	///�̵߳���������
	virtual void Execute(LPVOID lpParam){}
	///Terminate�¼�
	virtual void OnTerminated(void){};
	///�����¼�
	virtual void OnStart();
private:
	volatile bool m_bTerminated;			// �̹߳����ʾ
	LPVOID		m_lpParam;					// �߳���������
	DWORD		m_dwThreadID;				// �߳�ID��
	HANDLE		m_hThread;					// �߳̾��
	bool		m_bIsSuspended;				// �߳��Ƿ����
	TCHAR		m_szThreadName[MAX_PATH];	// �߳���
	HANDLE		m_hExitEvent;				// �˳���Ϣ�¼�
	///�߳�����
	static unsigned int WINAPI ThreadProc(LPVOID pParam);
};
