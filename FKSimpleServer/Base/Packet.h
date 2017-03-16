#pragma once
//-------------------------------------------------------------
typedef unsigned short		MSG_TYPE;
//-------------------------------------------------------------
const static MSG_TYPE	MSG_TYPE_UnknownMsg = 0;
const static MSG_TYPE	MSG_TYPE_Test = 1;
const static MSG_TYPE	MSG_TYPE_MAX = 0xff;
//-------------------------------------------------------------
#pragma pack(push, 1)
//-------------------------------------------------------------
struct SPacketHeader
{
	SPacketHeader()
		: m_usPacketType(MSG_TYPE_UnknownMsg)
	{
		m_usPacketSize = sizeof(SPacketHeader);
	}

public:
	unsigned short		m_usPacketSize;
	MSG_TYPE			m_usPacketType;
};
//-------------------------------------------------------------
struct STestPacket : public SPacketHeader
{
	STestPacket()
	{
		m_usPacketSize = sizeof(STestPacket);
		m_nPlaceHolder = -1;
		m_usPacketType = MSG_TYPE_Test;
	}

	int m_nPlaceHolder;
};

//-------------------------------------------------------------
#pragma pack(pop)
//-------------------------------------------------------------
class CClientSession;
typedef void(*HandlerFunc)(CClientSession* pSession);
static HandlerFunc g_HandlerTable[MSG_TYPE_MAX];
//-------------------------------------------------------------