#include "Packet.h"
#include "../Framework/ClientSession.h"
//-------------------------------------------------------------
static void DefaultHandler(CClientSession* pSession)
{
	printf("[DEBUG] Invalid packet handler, PlayerId: %d\n", pSession->GetPlayerID());
	pSession->Disconnect();
}
//-------------------------------------------------------------
struct SInitHandlers
{
	SInitHandlers()
	{
		for (int i = 0; i < MSG_TYPE_MAX; ++i)
			g_HandlerTable[i] = DefaultHandler;
	}
} g_InitHandlers;
struct SRegisterHandler
{
	SRegisterHandler(MSG_TYPE type, HandlerFunc handler)
	{
		g_HandlerTable[type] = handler;
	}
};
#define REGISTER_HANDLER(PKT_TYPE)	\
	static void Handler_##PKT_TYPE(CClientSession* pSession); \
	static SRegisterHandler _register_##PKT_TYPE(PKT_TYPE, Handler_##PKT_TYPE); \
	static void Handler_##PKT_TYPE(CClientSession* pSession)
//-------------------------------------------------------------