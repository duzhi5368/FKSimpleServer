#include "BaseServer.h"
#include "../Utils/TimeFunc.h"
#include "../Utils/Exception.h"
#include "../Utils/DBHelper.h"
#include "../Base/BaseMacro.h"
#include "../Utils/StringsExtracter.h"
//-------------------------------------------------------------
CBaseServer* g_pSvr = nullptr;
//-------------------------------------------------------------
CBaseServer::CBaseServer()
{
	GetLocalTime(&m_stStartTime);
	m_bStateInfoUpdated = FALSE;
	m_fLoopTime = 0.0f;
	m_dwLoopTimes = 0;
	m_dwLoopStartTime = 0;
	m_dwConnectedTimes = 0;
	m_dwDisconnectTimes = 0;
	m_dwLoopTimesAll = 0;
	srand(timeGetTime());
	m_pIoConsole = nullptr;

	m_pClientMgr = nullptr;

	Init();
}
//-------------------------------------------------------------
CBaseServer::CBaseServer(CIOConsole* pIOConsole)
{
	GetLocalTime(&m_stStartTime);
	m_bStateInfoUpdated = FALSE;
	m_fLoopTime = 0.0f;
	m_dwLoopTimes = 0;
	m_dwLoopStartTime = 0;
	m_dwConnectedTimes = 0;
	m_dwDisconnectTimes = 0;
	m_dwLoopTimesAll = 0;
	srand(timeGetTime());
	m_pIoConsole = pIOConsole;

	m_pClientMgr = nullptr;
}
//-------------------------------------------------------------
CBaseServer::~CBaseServer()
{
	Destory();
}
//-------------------------------------------------------------
bool CBaseServer::Init()
{
	if (!InitCoreModuleManagers())
	{
		return false;
	}
	if (!InitListenSocket())
	{
		ERROR_MSG_LOG_WITH_TIME(L"创建监听套接字失败.");
		return false;
	}
	if (!InitWorkThreads())
	{
		return false;
	}

	INFO_MSG_LOG_WITH_TIME(L"服务器逻辑初始化完成.");
	return true;
}
//-------------------------------------------------------------
void CBaseServer::Destory()
{
	m_pIoConsole = nullptr;

	SAFE_DELETE(m_pClientListenerThread);
	SAFE_DELETE(m_pClientMgrThread);
	SAFE_DELETE(m_pDBJobMgrThread);
	SAFE_DELETE(m_pDBJobMgr);
	SAFE_DELETE(m_pClientMgr);
}
//-------------------------------------------------------------
bool CBaseServer::InitListenSocket( WORD port )
{
	m_pListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_pListenSocket == INVALID_SOCKET)
		return false;

	int nOpt = 1;
	setsockopt(m_pListenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&nOpt, sizeof(int));

	SOCKADDR_IN svrAddr;
	ZeroMemory(&svrAddr, sizeof(svrAddr));
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_port = htons(port);
	svrAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int nRet = bind(m_pListenSocket, (SOCKADDR*)&svrAddr, sizeof(svrAddr));
	if (nRet == SOCKET_ERROR)
		return false;

	nRet = listen(m_pListenSocket, SOMAXCONN);
	if (nRet == SOCKET_ERROR)
		return false;

	INFO_MSG_LOG_WITH_TIME(L"当前监听端口：port = %d", port);
	return true;
}
//-------------------------------------------------------------
bool CBaseServer::InitWorkThreads()
{
	m_pClientMgrThread = new CClientMgrThread();
	if (m_pClientMgrThread == nullptr)
	{
		ERROR_MSG_LOG_WITH_TIME(L"创建客户端连接管理线程失败.");
		return false;
	}
	if (!m_pClientMgrThread->Start((LPVOID)&m_ListPendingAccept, true, L"客户端连接管理线程"))
	{
		ERROR_MSG_LOG_WITH_TIME(L"客户端连接管理线程启动失败.");
		return false;
	}
	SET_THREAD_NAME(m_pClientMgrThread->getThreadID(), "客户端连接管理线程");

	m_pDBJobMgrThread = new CDBJobMgrThread();
	if (m_pDBJobMgrThread == nullptr)
	{
		ERROR_MSG_LOG_WITH_TIME(L"创建数据库任务管理线程失败.");
		return false;
	}
	if (!m_pDBJobMgrThread->Start(NULL, true, L"数据库连接管理线程") )
	{
		ERROR_MSG_LOG_WITH_TIME(L"数据库任务管理线程启动失败.");
		return false;
	}
	SET_THREAD_NAME(m_pDBJobMgrThread->getThreadID(), "数据库连接管理线程");

	m_pClientListenerThread = new CClientListenerThread();
	if (m_pClientListenerThread == nullptr)
	{
		ERROR_MSG_LOG_WITH_TIME(L"创建客户端连接监听线程失败.");
		return false;
	}
	if (!m_pClientListenerThread->Start((LPVOID)&m_ListPendingAccept, true, L"客户端连接监听线程"))
	{
		ERROR_MSG_LOG_WITH_TIME(L"客户端连接监听线程启动失败.");
		return false;
	}
	SET_THREAD_NAME(m_pClientListenerThread->getThreadID(), "客户端连接监听线程");

	return true;
}
//-------------------------------------------------------------
bool CBaseServer::InitCoreModuleManagers()
{
	m_pClientMgr = new CClientManager();
	if (m_pClientMgr == nullptr)
	{
		ERROR_MSG_LOG_WITH_TIME(L"创建客户端连接管理器失败.");
		return false;
	}
	m_pDBJobMgr = new CDBJobManager();
	if (m_pDBJobMgr == nullptr)
	{
		ERROR_MSG_LOG_WITH_TIME(L"创建数据库任务管理器失败.");
		return false;
	}
	return true;
}
//-------------------------------------------------------------
void CBaseServer::UpdateTotalInfo()
{
	DWORD	dwCurTime = timeGetTime();
	m_dwLoopTimes++;
	m_dwLoopTimesAll++;
	if (m_dwLoopTimes >= 100)
	{
		m_dwLoopTimes = 0;
		m_fLoopTime = (dwCurTime - m_dwLoopStartTime) / 1000.0f;
		m_dwLoopStartTime = dwCurTime;
		m_bStateInfoUpdated = TRUE;
	}
}
//-------------------------------------------------------------
void CBaseServer::GetServerState(SServerState & state)
{
	state.bUpdated = m_bStateInfoUpdated;
	m_bStateInfoUpdated = false;
	SYSTEMTIME	stNow;
	GetLocalTime(&stNow);
	state.dwRunningSeconds = GetT1toT2Second(m_stStartTime, stNow);
	state.bUpdated = m_bStateInfoUpdated;
	state.dwRecvBytes = 0;
	state.dwSendBytes = 0;
	state.fLoopTime = m_fLoopTime;
	state.numConnected = (int)m_dwConnectedTimes;
	state.numDisconnected = (int)m_dwDisconnectTimes;
	state.numConnection = m_dwConnectedTimes - m_dwDisconnectTimes;
	state.dwLoopTimes = m_dwLoopTimesAll;
		wsprintf(m_szServerState, L"运行时间: %d天%d小时%d分%d秒",
		state.dwRunningSeconds / 86400, 
		(state.dwRunningSeconds / 3600) % 24,
		(state.dwRunningSeconds / 60) % 60,
		state.dwRunningSeconds % 60);
	state.pServerDescript = m_szServerState;
}
//-------------------------------------------------------------
bool CBaseServer::Start(CSettingFile &)
{ 
	INFO_MSG_LOG_WITH_TIME(L"================== [启动服务器] ==================");
	if (!CDBHelper::Initialize(DEFAULT_DB_FILE))
	{
		return false;
	}

	if (m_pClientMgrThread && m_pClientMgrThread->IsStarted())
	{
		m_pClientMgrThread->Resume();
	}
	if (m_pDBJobMgrThread && m_pDBJobMgrThread->IsStarted())
	{
		m_pDBJobMgrThread->Resume();
	}
	if (m_pClientListenerThread && m_pClientListenerThread->IsStarted())
	{
		m_pClientListenerThread->Resume();
	}

	m_dwLoopStartTime = timeGetTime();

	if (!IsStarted())
	{
		// 仅初始化一次
		CThread::Start(this, false, L"BaseServer线程");
	}
	else
	{
		// 如果挂起则唤醒
		if (IsSuspend())
		{
			Resume();
		}
	}

	return true; 
}
//-------------------------------------------------------------
bool CBaseServer::Stop()
{ 
	INFO_MSG_LOG_WITH_TIME(L"================== [挂起服务器] ==================");
	if (m_pClientListenerThread && m_pClientListenerThread->IsStarted() && !m_pClientListenerThread->IsSuspend())
	{
		m_pClientListenerThread->Suspend();
	}
	if (m_pDBJobMgrThread && m_pDBJobMgrThread->IsStarted() && !m_pDBJobMgrThread->IsSuspend())
	{
		m_pDBJobMgrThread->Suspend();
	}
	if (m_pClientMgrThread && m_pClientMgrThread->IsStarted() && !m_pClientMgrThread->IsSuspend())
	{
		m_pClientMgrThread->Suspend();
	}

	if (IsStarted() && !IsSuspend())
	{
		Suspend();
	}

	CDBHelper::Finalize();
	return true; 
}
//-------------------------------------------------------------
void CBaseServer::OnEvent(CEventSender * pSender, int iEvent, int iParam,
		LPVOID lpParam)
{
}
//-------------------------------------------------------------
void CBaseServer::OnInput(const TCHAR * pszString)
{
	INFO_MSG_LOG(L"命令：%s", pszString);
	/*
	TCHAR	szLine[256];
	o_strncpy(szLine, pszString, 250);
	TStringsExtracter<16> cmd(szLine, " \t,", " \t");
	if (cmd.getCount() > 0)
	{
		if (stricmp(cmd[0], "重新加载BanList") == 0)
		{
		}
		else if (stricmp(cmd[0], "重新加载TrustList") == 0)
		{
		}
		else
		{
			INFO_MSG_LOG(L"未识别的命令：%s", pszString);
		}
	}
	*/
}
//-------------------------------------------------------------
//	线程的主运行体
void CBaseServer::Execute(LPVOID lpParam)
{
	while (!IsTerminated())
	{
		Sleep(1);	// 无论线程是否挂起，都执行
		if (!IsSuspend())
		{
			Update();
			UpdateTotalInfo();
		}
	}
}
//-------------------------------------------------------------
void CBaseServer::OnTerminated()
{

}
//-------------------------------------------------------------
CIOConsole* CBaseServer::GetIoConsole()
{ 
	return m_pIoConsole; 
}
//-------------------------------------------------------------
void CBaseServer::SetIoConsole(const CIOConsole * pIoConsole)
{ 
	m_pIoConsole = (CIOConsole*)pIoConsole; 
}
//-------------------------------------------------------------
CClientManager*	CBaseServer::GetClientManager()
{
	return m_pClientMgr;
}
//-------------------------------------------------------------
CDBJobManager* CBaseServer::GetDBJobManager()
{
	return m_pDBJobMgr;
}
//-------------------------------------------------------------
SOCKET CBaseServer::GetListenSocket()
{
	return m_pListenSocket;
}
//-------------------------------------------------------------