#include "ClientListenerThread.h"
#include "../Utils/Scheduler.h"
#include "../Utils/ProduceConsumerQueue.h"
#include "../Base/ServerConfig.h"
#include "../Base/BaseDepend.h"
#include "../Base/BaseMacro.h"
#include "ClientSession.h"
#include "BaseServer.h"
//------------------------------------------------------------------------
///�̵߳���������
void CClientListenerThread::Execute(LPVOID lpParam)
{
	while ( !CThread::IsTerminated() )
	{
		if (g_pSvr == nullptr)
			break;
		SOCKET acceptSocket = accept(g_pSvr->GetListenSocket(), NULL, NULL);
		if (acceptSocket == INVALID_SOCKET)
		{
			ERROR_MSG_LOG_WITH_TIME(L"�յ�һ����Ч�׽���.");
			continue;
		}
		((PendingAcceptList*)lpParam)->Produce(acceptSocket);
	}
	return;
}
//------------------------------------------------------------------------
///Terminate�¼�
void CClientListenerThread::OnTerminated(void)
{
	if (g_pSvr)
	{
		if (g_pSvr->GetListenSocket() != INVALID_SOCKET)
		{
			int ierr = closesocket(g_pSvr->GetListenSocket());
			if (ierr == SOCKET_ERROR)
			{
				ERROR_MSG_LOG_WITH_TIME(L"�ͻ��˼����߳�closesocket()ʧ�ܣ������� = %d.", ierr);
				CONFIM_MESSAGE_BOX(L"����", L"�ͻ��˼����߳�closesocket()ʧ��.");
			}
		}
	}
	Sleep(3000);
}
//------------------------------------------------------------------------