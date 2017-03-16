#pragma once
//-------------------------------------------------------------
#include "../Utils/Scheduler.h"
#include "../Utils/ProduceConsumerQueue.h"
#include "../Base/ServerConfig.h"
#include "../Base/BaseDepend.h"
#include "ClientSession.h"
//-------------------------------------------------------------
void CALLBACK TimerProc(LPVOID lpArg, DWORD dwTimerLowValue, DWORD dwTimerHighValue)
{
	s_pScheduler->DoTasks();
	g_pSvr->GetClientManager()->OnPeriodWork();
}
//-------------------------------------------------------------
unsigned int WINAPI ClientHandlingThread(LPVOID lpParam)
{
	s_pScheduler = new CScheduler;

	PendingAcceptList* pAcceptList = (PendingAcceptList*)lpParam;

	HANDLE hTimer = CreateWaitableTimer(nullptr, FALSE, nullptr);
	if (hTimer == NULL)
		return -1;

	LARGE_INTEGER llDueTime;
	llDueTime.QuadPart = -10000000;
	if (!SetWaitableTimer(hTimer, &llDueTime, DEFAULT_TIMER_INTERVAL, TimerProc, nullptr, TRUE))
		return -1;

	while (true)
	{
		SOCKET acceptSocket = NULL;
		if (pAcceptList->Consume(acceptSocket, false))
		{
			CClientSession* pClient = g_pSvr->GetClientManager()->CreateClient(acceptSocket);
			SOCKADDR_IN clientAddr;
			int nAddrLen = sizeof(clientAddr);
			getpeername(acceptSocket, (SOCKADDR*)&clientAddr, &nAddrLen);
			if (!pClient->OnConnect(&clientAddr))
			{
				pClient->Disconnect();
			}
			continue;
		}

		g_pSvr->GetClientManager()->FlushClientSend();
		SleepEx(INFINITE, TRUE);
	}

	CloseHandle(hTimer);
	return 0;
}
//-------------------------------------------------------------
unsigned int WINAPI DBHandlingThread(LPVOID lpParam)
{
	g_pSvr->GetDBJobManager()->ExecuteDBJobs();
	return 0;
}
//-------------------------------------------------------------
unsigned int WINAPI ListenHandlingThread(LPVOID lpParam)
{
	while (true)
	{
		SOCKET acceptSocket = accept(g_pSvr->GetListenSocket(), NULL, NULL);
		if (acceptSocket == INVALID_SOCKET)
		{
			printf("Accept a invalid socket.\n");
			continue;
		}
		((PendingAcceptList*)lpParam)->Produce(acceptSocket);
	}
	return 0;
}
//-------------------------------------------------------------