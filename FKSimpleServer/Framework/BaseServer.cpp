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
		ERROR_MSG_LOG_WITH_TIME(L"���������׽���ʧ��.");
		return false;
	}
	if (!InitWorkThreads())
	{
		return false;
	}

	INFO_MSG_LOG_WITH_TIME(L"�������߼���ʼ�����.");
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

	INFO_MSG_LOG_WITH_TIME(L"��ǰ�����˿ڣ�port = %d", port);
	return true;
}
//-------------------------------------------------------------
bool CBaseServer::InitWorkThreads()
{
	m_pClientMgrThread = new CClientMgrThread();
	if (m_pClientMgrThread == nullptr)
	{
		ERROR_MSG_LOG_WITH_TIME(L"�����ͻ������ӹ����߳�ʧ��.");
		return false;
	}
	if (!m_pClientMgrThread->Start((LPVOID)&m_ListPendingAccept, true, L"�ͻ������ӹ����߳�"))
	{
		ERROR_MSG_LOG_WITH_TIME(L"�ͻ������ӹ����߳�����ʧ��.");
		return false;
	}
	SET_THREAD_NAME(m_pClientMgrThread->getThreadID(), "�ͻ������ӹ����߳�");

	m_pDBJobMgrThread = new CDBJobMgrThread();
	if (m_pDBJobMgrThread == nullptr)
	{
		ERROR_MSG_LOG_WITH_TIME(L"�������ݿ���������߳�ʧ��.");
		return false;
	}
	if (!m_pDBJobMgrThread->Start(NULL, true, L"���ݿ����ӹ����߳�") )
	{
		ERROR_MSG_LOG_WITH_TIME(L"���ݿ���������߳�����ʧ��.");
		return false;
	}
	SET_THREAD_NAME(m_pDBJobMgrThread->getThreadID(), "���ݿ����ӹ����߳�");

	m_pClientListenerThread = new CClientListenerThread();
	if (m_pClientListenerThread == nullptr)
	{
		ERROR_MSG_LOG_WITH_TIME(L"�����ͻ������Ӽ����߳�ʧ��.");
		return false;
	}
	if (!m_pClientListenerThread->Start((LPVOID)&m_ListPendingAccept, true, L"�ͻ������Ӽ����߳�"))
	{
		ERROR_MSG_LOG_WITH_TIME(L"�ͻ������Ӽ����߳�����ʧ��.");
		return false;
	}
	SET_THREAD_NAME(m_pClientListenerThread->getThreadID(), "�ͻ������Ӽ����߳�");

	return true;
}
//-------------------------------------------------------------
bool CBaseServer::InitCoreModuleManagers()
{
	m_pClientMgr = new CClientManager();
	if (m_pClientMgr == nullptr)
	{
		ERROR_MSG_LOG_WITH_TIME(L"�����ͻ������ӹ�����ʧ��.");
		return false;
	}
	m_pDBJobMgr = new CDBJobManager();
	if (m_pDBJobMgr == nullptr)
	{
		ERROR_MSG_LOG_WITH_TIME(L"�������ݿ����������ʧ��.");
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
		wsprintf(m_szServerState, L"����ʱ��: %d��%dСʱ%d��%d��",
		state.dwRunningSeconds / 86400, 
		(state.dwRunningSeconds / 3600) % 24,
		(state.dwRunningSeconds / 60) % 60,
		state.dwRunningSeconds % 60);
	state.pServerDescript = m_szServerState;
}
//-------------------------------------------------------------
bool CBaseServer::Start(CSettingFile &)
{ 
	INFO_MSG_LOG_WITH_TIME(L"================== [����������] ==================");
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
		// ����ʼ��һ��
		CThread::Start(this, false, L"BaseServer�߳�");
	}
	else
	{
		// �����������
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
	INFO_MSG_LOG_WITH_TIME(L"================== [���������] ==================");
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
	INFO_MSG_LOG(L"���%s", pszString);
	/*
	TCHAR	szLine[256];
	o_strncpy(szLine, pszString, 250);
	TStringsExtracter<16> cmd(szLine, " \t,", " \t");
	if (cmd.getCount() > 0)
	{
		if (stricmp(cmd[0], "���¼���BanList") == 0)
		{
		}
		else if (stricmp(cmd[0], "���¼���TrustList") == 0)
		{
		}
		else
		{
			INFO_MSG_LOG(L"δʶ������%s", pszString);
		}
	}
	*/
}
//-------------------------------------------------------------
//	�̵߳���������
void CBaseServer::Execute(LPVOID lpParam)
{
	while (!IsTerminated())
	{
		Sleep(1);	// �����߳��Ƿ���𣬶�ִ��
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