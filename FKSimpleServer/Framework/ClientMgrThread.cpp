#include "ClientMgrThread.h"
#include "../Utils/Scheduler.h"
#include "../Utils/ProduceConsumerQueue.h"
#include "../Base/ServerConfig.h"
#include "../Base/BaseDepend.h"
#include "../Base/BaseMacro.h"
#include "ClientSession.h"
#include "BaseServer.h"
//------------------------------------------------------------------------
void CALLBACK TimerProc(LPVOID lpArg, DWORD dwTimerLowValue, DWORD dwTimerHighValue)
{
	if (s_pScheduler == NULL)
		return;
	s_pScheduler->DoTasks();
	if (g_pSvr && g_pSvr->GetClientManager())
	{
		g_pSvr->GetClientManager()->OnPeriodWork();
	}
}
//-------------------------------------------------------------
void CClientMgrThread::Execute(LPVOID lpParam)
{
	s_pScheduler = new CScheduler;

	PendingAcceptList* pAcceptList = (PendingAcceptList*)lpParam;

	HANDLE hTimer = CreateWaitableTimer(nullptr, FALSE, nullptr);
	if (hTimer == NULL)
		return;

	LARGE_INTEGER llDueTime;
	llDueTime.QuadPart = -10000000;
	if (!SetWaitableTimer(hTimer, &llDueTime, DEFAULT_TIMER_INTERVAL, TimerProc, nullptr, TRUE))
		return;

	while (!CThread::IsTerminated())
	{
		SOCKET acceptSocket = NULL;
		if (pAcceptList->Consume(acceptSocket, false))
		{
			CClientSession* pClient = g_pSvr->GetClientManager()->CreateClient(acceptSocket);
			if (pClient != NULL)
			{
				g_pSvr->AddConnectedClientNum();
			}
			SOCKADDR_IN clientAddr;
			int nAddrLen = sizeof(clientAddr);
			getpeername(acceptSocket, (SOCKADDR*)&clientAddr, &nAddrLen);
			if (!pClient->OnConnect(&clientAddr))
			{
				pClient->Disconnect();
			}
			continue;
		}

		if (g_pSvr && g_pSvr->GetClientManager())
			g_pSvr->GetClientManager()->FlushClientSend();
		SleepEx(INFINITE, TRUE);
	}

	CloseHandle(hTimer);
	return;
}
//------------------------------------------------------------------------
void CClientMgrThread::OnTerminated(void)
{
}
//------------------------------------------------------------------------