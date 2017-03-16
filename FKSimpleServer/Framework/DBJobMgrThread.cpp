#include "DBJobMgrThread.h"
#include "../Utils/Scheduler.h"
#include "../Utils/ProduceConsumerQueue.h"
#include "../Base/ServerConfig.h"
#include "../Base/BaseDepend.h"
#include "../Base/BaseMacro.h"
#include "ClientSession.h"
#include "BaseServer.h"
//------------------------------------------------------------------------
///线程的主运行体
void CDBJobMgrThread::Execute(LPVOID lpParam)
{
	while (!CThread::IsTerminated())
	{
		if (g_pSvr && g_pSvr->GetDBJobManager())
		{
			g_pSvr->GetDBJobManager()->ExecuteDBJobs();
		}
	}
}
//------------------------------------------------------------------------
///Terminate事件
void CDBJobMgrThread::OnTerminated(void)
{
}
//------------------------------------------------------------------------