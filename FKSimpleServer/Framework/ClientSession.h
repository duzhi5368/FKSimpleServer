#pragma once
//-------------------------------------------------------------
#include "../Base/BaseDepend.h"
#include "../Utils/RefCountable.h"
#include "../Utils/ObjectPool.h"
#include "../Utils/CircularBuffer.h"
#include "../Base/Packet.h"
//-------------------------------------------------------------
class CClientSession;
class CClientManager;
struct SDBJobContext;
//-------------------------------------------------------------
struct SOverlappedIO : public OVERLAPPED
{
	SOverlappedIO()
		: m_pObject(nullptr)
	{}
public:
	CClientSession*		m_pObject;
};
//-------------------------------------------------------------
class CClientSession : public CRefCountable, public TObjectPool < CClientSession >
{
public:
	CClientSession(SOCKET sock);
	virtual ~CClientSession();
public:
	// 希望子类继承实现
	virtual	void		SubOnLogicTick(){}
	virtual void		SubOnDBTick(){}
public:
	int					GetPlayerID() const;
	SOCKET				GetSocket() const;

	bool				IsConnected() const;
	void				OnTick();
	void				OnDBUpdate();

	void				OnRead(size_t unLen);
	void				OnWriteComplete(size_t unLen);

	bool				OnConnect(SOCKADDR_IN* pAddr);
	bool				PostRecv();
	bool				SendRequest(SPacketHeader* pack);
	bool				Broadcast(SPacketHeader* pack);

	void				Disconnect();
	bool				SendFlush();
	void				DBJobDone(SDBJobContext* pResult);

	template<class PACKET_TYPE>
	bool				ParsePacket(PACKET_TYPE& pkt)
	{
		return m_RecvBuffer.Read((char*)&pkt, pkt.m_usPacketSize);
	}
	SOCKADDR_IN			GetAddr()const{ return m_ClientAddr; }
private:
	bool				m_bIsConnected;
	bool				m_bIsLogin;
	SOCKET				m_Socket;
	int					m_nPlayerID;
	SOCKADDR_IN			m_ClientAddr;
	CCircularBuffer		m_SendBuffer;
	CCircularBuffer		m_RecvBuffer;
	SOverlappedIO		m_SendOverlapped;
	SOverlappedIO		m_RecvOverlapped;
public:
	friend class CClientManager;
};
//-------------------------------------------------------------
void CALLBACK RecvCompletion(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
void CALLBACK SendCompletion(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
//-------------------------------------------------------------