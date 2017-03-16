#include "ClientListenerThread.h"
#include "../Utils/Scheduler.h"
#include "../Utils/ProduceConsumerQueue.h"
#include "../Base/ServerConfig.h"
#include "../Base/BaseDepend.h"
#include "../Base/BaseMacro.h"
#include "ClientSession.h"
#include "BaseServer.h"
//------------------------------------------------------------------------
///线程的主运行体
void CClientListenerThread::Execute(LPVOID lpParam)
{
	while ( !CThread::IsTerminated() )
	{
		if (g_pSvr == nullptr)
			break;
		SOCKET acceptSocket = accept(g_pSvr->GetListenSocket(), NULL, NULL);
		if (acceptSocket == INVALID_SOCKET)
		{
			ERROR_MSG_LOG_WITH_TIME(L"收到一个无效套接字.");
			continue;
		}
		((PendingAcceptList*)lpParam)->Produce(acceptSocket);
	}
	return;
}
//------------------------------------------------------------------------
///Terminate事件
void CClientListenerThread::OnTerminated(void)
{
	if (g_pSvr)
	{
		if (g_pSvr->GetListenSocket() != INVALID_SOCKET)
		{
			int ierr = closesocket(g_pSvr->GetListenSocket());
			if (ierr == SOCKET_ERROR)
			{
				ERROR_MSG_LOG_WITH_TIME(L"客户端监听线程closesocket()失败：错误码 = %d.", ierr);
				CONFIM_MESSAGE_BOX(L"错误", L"客户端监听线程closesocket()失败.");
			}
		}
	}
	Sleep(3000);
}
//------------------------------------------------------------------------