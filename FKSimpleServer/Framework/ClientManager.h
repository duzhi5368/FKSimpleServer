#pragma once
//-------------------------------------------------------------
#include "../Base/BaseDepend.h"
//-------------------------------------------------------------
class CClientSession;
struct SPacketHeader;
struct SDBJobContext;
//-------------------------------------------------------------
class CClientManager
{
public:
	CClientManager();
	~CClientManager();
public:
	CClientSession*		CreateClient(SOCKET sock);

	void				BroadcastPacket(CClientSession* pFrom, SPacketHeader* pPack);
	void				OnPeriodWork();
	void				FlushClientSend();
private:
	void				__CollectGarbageSessions();
	void				__DispatchDBJobResults();
private:
	typedef std::map<SOCKET, CClientSession*>		ClientMap;
	ClientMap			m_mapClientList;
	DWORD				m_dwLastGCTick;
};
//-------------------------------------------------------------