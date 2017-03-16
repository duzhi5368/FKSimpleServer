#include "ClientManager.h"
#include "ClientSession.h"
#include "../Base/ServerConfig.h"
#include "../Utils/Thread.h"
#include "../Base/DBJobContext.h"
#include "DBJobManager.h"
#include "BaseServer.h"
#include "../Base/BaseMacro.h"
#include "../Utils/StringFunc.h"
//-------------------------------------------------------------
CClientManager::CClientManager()
	: m_dwLastGCTick( 0 )
{

}
//-------------------------------------------------------------
CClientManager::~CClientManager()
{
}
//-------------------------------------------------------------
CClientSession* CClientManager::CreateClient(SOCKET sock)
{
	CClientSession* pClient = new CClientSession(sock);
	if (pClient == nullptr)
	{
		ERROR_MSG_LOG_WITH_TIME(L"创建客户端失败: %d", sock);
	}
	m_mapClientList.insert(ClientMap::value_type(sock, pClient));
	return pClient;
}
//-------------------------------------------------------------
void CClientManager::BroadcastPacket(CClientSession* pFrom, SPacketHeader* pPack)
{
	for (ClientMap::const_iterator Ite = m_mapClientList.begin();
		Ite != m_mapClientList.end(); ++Ite)
	{
		CClientSession* pClient = Ite->second;
		if ( pClient == pFrom )
			continue;

		pClient->SendRequest(pPack);
	}
}
//-------------------------------------------------------------
void CClientManager::OnPeriodWork()
{
	DWORD dwCurTick = GetTickCount();
	if (dwCurTick - m_dwLastGCTick >= DEFAULT_GC_TICK_INTERAL)
	{
		__CollectGarbageSessions();
		m_dwLastGCTick = dwCurTick;
	}

	__DispatchDBJobResults();
}
//-------------------------------------------------------------
void CClientManager::FlushClientSend()
{
	for (auto& ite : m_mapClientList)
	{
		CClientSession* pClient = ite.second;
		if (!pClient->SendFlush())
		{
			pClient->Disconnect();
		}
	}
}
//-------------------------------------------------------------
void CClientManager::__CollectGarbageSessions()
{
	std::vector<CClientSession*>	vecDisconnectSessions;

	std::for_each(m_mapClientList.begin(), m_mapClientList.end(),
		[&](ClientMap::const_reference it){
		CClientSession* pClient = it.second;
		if (pClient->IsConnected() == false && pClient->GetRefCount() == 0)
		{
			vecDisconnectSessions.push_back(pClient);
		}
	});

	for (size_t i = 0; i < vecDisconnectSessions.size(); ++i)
	{
		CClientSession* pClient = vecDisconnectSessions[i];
		if (pClient != nullptr)
		{
			INFO_MSG(L"清除客户端连接: id = %d, ip = %s, port = %d", pClient->GetPlayerID(),
				MBytesToWString(inet_ntoa(pClient->GetAddr().sin_addr)).c_str(), 
				ntohs(pClient->GetAddr().sin_port));
			m_mapClientList.erase(pClient->GetSocket());
			delete pClient;
		}
	}
}
//-------------------------------------------------------------
void CClientManager::__DispatchDBJobResults()
{
	SDBJobContext* pDBResult = nullptr;
	while (g_pSvr->GetDBJobManager()->PopDBJobResult(pDBResult))
	{
		if (!pDBResult->m_bIsSuccess)
		{
			ERROR_MSG_LOG_WITH_TIME(L"执行DB工作失败");
		}
		else
		{
			if (typeid(*pDBResult) == typeid(STestDBJobContext))
			{
				INFO_MSG(L"执行TestDB成功.");
			}
			else
			{
				auto& Ite = m_mapClientList.find(pDBResult->m_SocketKey);
				if (Ite != m_mapClientList.end() &&
					Ite->second->IsConnected())
				{
					Ite->second->DBJobDone(pDBResult);
				}
			}
		}

		SDBJobContext* pNeedToBeDelete = pDBResult;
		delete pNeedToBeDelete;
	}
}
//-------------------------------------------------------------