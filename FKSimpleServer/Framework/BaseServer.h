#pragma once
//-------------------------------------------------------------
#include "VirtualServer.h"
#include "../Utils/EventBase.h"
#include "../Utils/IOConsole.h"
#include "../Utils/Thread.h"
#include "ClientManager.h"
#include "DBJobManager.h"
#include "../Utils/ProduceConsumerQueue.h"
#include "ClientMgrThread.h"
#include "DBJobMgrThread.h"
#include "ClientListenerThread.h"
//-------------------------------------------------------------
class CBaseServer :
	public CVirtualServer, public CEventListener, public CInputListener, public CThread
{
public:
	CBaseServer();
	CBaseServer(CIOConsole* pIOConsole);
	virtual ~CBaseServer();
public:
	bool			Init();
	virtual void	GetServerState(SServerState & state);
	virtual bool	Start(CSettingFile &);
	virtual bool	Stop();
	virtual void	OnEvent(CEventSender * pSender, int iEvent, int iParam,
		LPVOID lpParam);
	virtual void	OnInput(const TCHAR * pszString);
	// 逻辑主循环
	void			Update(){};
public:
	CIOConsole *	GetIoConsole();
	void			SetIoConsole(const CIOConsole * pIoConsole);
	CClientManager*	GetClientManager();
	CDBJobManager*	GetDBJobManager();
	SOCKET			GetListenSocket();
	void			AddConnectedClientNum(){ m_dwConnectedTimes++; }
	void			AddDisConnectedClientNum(){ m_dwDisconnectTimes++; }
private:
	void			Destory();
	bool			InitListenSocket( WORD port = DEFAULT_LISTEN_PORT );
	bool			InitWorkThreads();
	bool			InitCoreModuleManagers();
	void			UpdateTotalInfo();
	// 线程的主运行体
	virtual void	Execute(LPVOID lpParam);
	// 退出
	virtual void	OnTerminated();
private:
	bool					m_bStateInfoUpdated;
	SYSTEMTIME				m_stStartTime;
	DWORD					m_dwConnectedTimes;
	DWORD					m_dwDisconnectTimes;
	DWORD					m_dwLoopTimesAll;
	TCHAR					m_szServerState[1024];
	DWORD					m_dwLoopTimes;
	DWORD					m_dwLoopStartTime;
	FLOAT					m_fLoopTime;

	CIOConsole*				m_pIoConsole;
	CClientManager*			m_pClientMgr;
	CDBJobManager*			m_pDBJobMgr;
	SOCKET					m_pListenSocket;

	CClientMgrThread*		m_pClientMgrThread;
	CDBJobMgrThread*		m_pDBJobMgrThread;
	CClientListenerThread*	m_pClientListenerThread;
	PendingAcceptList		m_ListPendingAccept;
};
//-------------------------------------------------------------
extern CBaseServer* g_pSvr;
//-------------------------------------------------------------