#include "ClientSession.h"
#include "../Utils/Scheduler.h"
#include "../Base/ServerConfig.h"
#include "../Base/DBJobContext.h"
#include "ClientManager.h"
#include "BaseServer.h"
#include "../Base/BaseMacro.h"
#include "../Utils/StringFunc.h"
//-------------------------------------------------------------
CClientSession::CClientSession(SOCKET sock)
	: m_bIsConnected( false )
	, m_bIsLogin( false )
	, m_Socket( sock )
	, m_SendBuffer(DEFAULT_BUF_SIZE)
	, m_RecvBuffer(DEFAULT_BUF_SIZE)
	, m_nPlayerID( 0 )
{
	memset(&m_ClientAddr, 0, sizeof(SOCKADDR_IN));
}
//-------------------------------------------------------------
CClientSession::~CClientSession()
{
}
//-------------------------------------------------------------
int CClientSession::GetPlayerID() const
{
	return m_nPlayerID;
}
//-------------------------------------------------------------
SOCKET CClientSession::GetSocket() const
{
	return m_Socket;
}
//-------------------------------------------------------------
bool CClientSession::IsConnected() const
{
	return m_bIsConnected;
}
//-------------------------------------------------------------
void CClientSession::OnTick()
{
	if (!IsConnected())
		return;

	SubOnLogicTick();

	CallFuncAfter(DEFAULT_CLIENT_TICK_INTERAL, this, &CClientSession::OnTick);
}
//-------------------------------------------------------------
void CClientSession::OnDBUpdate()
{
	if (!IsConnected())
		return;

	SubOnDBTick();

	CallFuncAfter(DEFAULT_CLIENT_DB_TICK_INTERAL, this, &CClientSession::OnDBUpdate);
}
//-------------------------------------------------------------
void CClientSession::OnRead(size_t unLen)
{
	m_RecvBuffer.Commit(unLen);

	while (true)
	{
		SPacketHeader header;
		if (m_RecvBuffer.Peek((char*)&header, sizeof(SPacketHeader)))
			return;

		if (m_RecvBuffer.GetStoredSize() < (size_t)header.m_usPacketSize)
			return;

		if (header.m_usPacketType == MSG_TYPE_UnknownMsg)
		{
			printf("[DEBUG] Invalid packet type, PlayerID: %d\n", GetPlayerID());
			Disconnect();
			return;
		}

		g_HandlerTable[header.m_usPacketType](this);
	}
}
//-------------------------------------------------------------
void CClientSession::OnWriteComplete(size_t unLen)
{
	m_SendBuffer.Remove(unLen);
}
//-------------------------------------------------------------
bool CClientSession::OnConnect(SOCKADDR_IN* pAddr)
{
	memcpy(&m_ClientAddr, pAddr, sizeof(SOCKADDR_IN));

	u_long ulArg = 1;
	ioctlsocket(m_Socket, FIONBIO, &ulArg);
	int nOpt = 1;
	setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&nOpt, sizeof(int));

	INFO_MSG(L"创建客户端连接: id = %d, ip = %s, port = %d", GetPlayerID(),
		MBytesToWString(inet_ntoa(GetAddr().sin_addr)).c_str(), ntohs(GetAddr().sin_port));

	m_bIsConnected = true;
	return PostRecv();
}
//-------------------------------------------------------------
bool CClientSession::PostRecv()
{
	if (!IsConnected())
		return false;

	DWORD dwRecvBytes = 0;
	DWORD dwFlags = 0;
	WSABUF buf;
	buf.len = (ULONG)m_RecvBuffer.GetFreeSpaceSize();
	buf.buf = (char*)m_RecvBuffer.GetBuffer();

	memset(&m_RecvOverlapped, 0, sizeof(SOverlappedIO));
	m_RecvOverlapped.m_pObject = this;

	if (SOCKET_ERROR == WSARecv(m_Socket, &buf, 1, &dwRecvBytes, &dwFlags, &m_RecvOverlapped, RecvCompletion))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			return false;
	}

	IncRefCount();
	return true;
}
//-------------------------------------------------------------
bool CClientSession::SendRequest(SPacketHeader* pack)
{
	if (!IsConnected())
		return false;

	if (!m_SendBuffer.Write((char*)pack, pack->m_usPacketSize))
	{
		Disconnect();
		return false;
	}
	return true;
}
//-------------------------------------------------------------
bool CClientSession::Broadcast(SPacketHeader* pack)
{
	if (!SendRequest(pack))
		return false;
	if (!IsConnected())
		return false;

	g_pSvr->GetClientManager()->BroadcastPacket(this, pack);
	return true;
}
//-------------------------------------------------------------
void CClientSession::Disconnect()
{
	if (!IsConnected())
	{
		return;
	}

	INFO_MSG(L"客户端断开连接: id = %d, ip = %s, port = %d", GetPlayerID(),
		MBytesToWString(inet_ntoa(GetAddr().sin_addr)).c_str(), ntohs(GetAddr().sin_port));

	g_pSvr->AddDisConnectedClientNum();

	LINGER lingerOption;
	lingerOption.l_onoff = 1;
	lingerOption.l_linger = 0;
	if (SOCKET_ERROR == setsockopt(m_Socket, SOL_SOCKET, SO_LINGER, (char*)&lingerOption, sizeof(LINGER)))
	{
		printf_s("[DEBUG] setsockopt linger option error: %d\n", GetLastError());
		return;
	}

	closesocket(m_Socket);
	m_bIsConnected = false;
}
//-------------------------------------------------------------
bool CClientSession::SendFlush()
{
	if (!IsConnected())
		return false;

	if (m_SendBuffer.GetContiguiousSize() == 0)
		return true;

	DWORD dwSendBytes = 0;
	DWORD dwFlags = 0;

	WSABUF buf;
	buf.len = (ULONG)m_SendBuffer.GetContiguiousSize();
	buf.buf = (char*)m_SendBuffer.GetBufferStart();

	memset(&m_SendOverlapped, 0, sizeof(SOverlappedIO));
	m_SendOverlapped.m_pObject = this;

	if (SOCKET_ERROR == WSASend(m_Socket, &buf, 1, &dwSendBytes, dwFlags, &m_SendOverlapped, SendCompletion))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			return false;
	}

	IncRefCount();
	return true;
}
//-------------------------------------------------------------
void CClientSession::DBJobDone(SDBJobContext* pResult)
{
	FK_CRASH_ASSERT(m_Socket == pResult->m_SocketKey);

	const type_info& typeInfo = typeid(*pResult);


	if (typeInfo == typeid(STestDBJobContext))
	{
		STestDBJobContext* pTest = dynamic_cast<STestDBJobContext*>(pResult);
		if (pTest)
		{
			m_bIsLogin = true;
			OnTick();
			OnDBUpdate();
		}
	}
	else
	{
		FK_CRASH_ASSERT(false);
	}
}
//-------------------------------------------------------------
void CALLBACK RecvCompletion(DWORD dwError, DWORD cbTransferred, 
	LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	CClientSession* pFrom = static_cast<SOverlappedIO*>(lpOverlapped)->m_pObject;
	pFrom->DecRefCount();

	if (!pFrom->IsConnected())
		return;

	if (dwError || cbTransferred == 0)
	{
		pFrom->Disconnect();
		return;
	}

	pFrom->OnRead(cbTransferred);

	if (!pFrom->PostRecv())
	{
		pFrom->Disconnect();
		return;
	}
}
//-------------------------------------------------------------
void CALLBACK SendCompletion(DWORD dwError, DWORD cbTransferred, 
	LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	CClientSession* pFrom = static_cast<SOverlappedIO*>(lpOverlapped)->m_pObject;
	pFrom->DecRefCount();

	if (!pFrom->IsConnected())
		return;

	if (dwError || cbTransferred == 0)
	{
		pFrom->Disconnect();
		return;
	}

	pFrom->OnWriteComplete(cbTransferred);
}
//-------------------------------------------------------------